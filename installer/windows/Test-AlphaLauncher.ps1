$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.IO.Compression.FileSystem
$launcher = Join-Path $PSScriptRoot 'Start-Alpha.ps1'
$tempParent = [IO.Path]::GetFullPath([IO.Path]::GetTempPath())
$testRoot = Join-Path $tempParent ('geode-alpha-tests-' + [guid]::NewGuid())
New-Item -ItemType Directory -Path $testRoot | Out-Null
$config = Join-Path $testRoot 'runtimes.json'
$failures = 0

function Check([string]$Name, [scriptblock]$Test) {
    try { & $Test; Write-Output "PASS: $Name" }
    catch { $script:failures++; Write-Output "FAIL: ${Name}: $_" }
}

function Reject([string]$Pattern, [scriptblock]$Test) {
    try { & $Test | Out-Null }
    catch {
        if ($_.Exception.Message -notlike $Pattern) { throw }
        return
    }
    throw "Expected rejection matching $Pattern."
}

function Write-Mod([string]$Runtime, [string]$Sdk, [string]$Game) {
    $path = Join-Path $testRoot "$Runtime/geode/mods/test.geode"
    if (Test-Path -LiteralPath $path) { Remove-Item -LiteralPath $path }
    $archive = [IO.Compression.ZipFile]::Open($path, 'Create')
    try {
        $entry = $archive.CreateEntry('mod.json')
        $writer = New-Object IO.StreamWriter($entry.Open())
        try {
            $metadata = @{ id = 'alpha.test'; geode = $Sdk; gd = @{win = $Game} }
            $writer.Write(($metadata | ConvertTo-Json -Depth 5))
        }
        finally { $writer.Dispose() }
    }
    finally { $archive.Dispose() }
}

function Save-Config($Profiles) {
    @{schemaVersion = 1; runtimes = @($Profiles)} | ConvertTo-Json -Depth 5 |
        Set-Content -LiteralPath $config -Encoding UTF8
}

try {
    $profiles = @()
    foreach ($id in @('Current', 'Legacy')) {
        $directory = Join-Path $testRoot $id
        $mods = Join-Path $directory 'geode/mods'
        $resources = Join-Path $directory 'geode/resources/geode.loader'
        New-Item -ItemType Directory -Path $mods, $resources -Force | Out-Null
        foreach ($file in @("$id.exe", 'Geode.dll', 'libcocos2d.dll')) {
            Set-Content -LiteralPath (Join-Path $directory $file) -Value $id
        }
        $sdk = if ($id -eq 'Current') { '6.0.0-alpha.1' } else { '5.11.2' }
        $game = if ($id -eq 'Current') { '2.2091' } else { '2.2081' }
        Set-Content -LiteralPath "$resources/version" -Value $sdk
        $digest = (Get-FileHash -LiteralPath "$directory/$id.exe").Hash
        $profiles += [pscustomobject]@{
            id = $id; executable = "$id/$id.exe"; gameVersion = $game
            geodeVersion = $sdk; gameSha256 = $digest
            engineSha256 = $digest; loaderSha256 = $digest
        }
    }
    Save-Config $profiles
    Check 'Empty profiles prefer Current without launching' {
        $result = & $launcher -Config $config -Plan
        if ($result.runtime -ne 'Current' -or $result.modCount -ne 0) { throw $result }
    }
    Write-Mod 'Legacy' '5.10.1' '2.2081'
    Check 'Auto chooses the populated legacy profile' {
        $result = & $launcher -Config $config -Plan
        if ($result.runtime -ne 'Legacy' -or $result.modCount -ne 1) { throw $result }
    }
    Write-Mod 'Current' '6.0.0-alpha.1' '2.2091'
    Check 'Two populated profiles require a choice' {
        Reject '*Both runtimes contain mods*' { & $launcher -Config $config -Plan }
    }
    Check 'Explicit Current resolves the ambiguity' {
        if ((& $launcher -Config $config -Runtime Current -Plan).runtime -ne 'Current') {
            throw 'Wrong runtime.'
        }
    }
    Write-Mod 'Legacy' '6.0.0' '2.2081'
    Check 'Legacy rejects a newer SDK mod' {
        Reject '*targets Geode*' { & $launcher -Config $config -Runtime Legacy -Plan }
    }
    Write-Mod 'Legacy' '5.10.1' '2.2074'
    Check 'Legacy rejects a different game version' {
        Reject '*targets Geode*' { & $launcher -Config $config -Runtime Legacy -Plan }
    }
    Write-Mod 'Current' '5.10.1' '2.2081'
    Check 'Current experiments preserve the explicit override' {
        $result = & $launcher -Config $config -Runtime Current -Experimental -Plan
        if ($result.arguments -notcontains '--geode:allow-incompatible-mods') {
            throw 'Missing experimental argument.'
        }
    }
    Write-Mod 'Current' '5.10.1' '0.000'
    Check 'Experimental mode still rejects unsupported Windows packages' {
        Reject '*does not support Windows*' {
            & $launcher -Config $config -Runtime Current -Experimental -Plan
        }
    }
    Set-Content -LiteralPath "$testRoot/Legacy/geode/mods/test.geode" -Value 'broken'
    Check 'Safe mode can recover from a corrupt package' {
        $result = & $launcher -Config $config -Runtime Legacy -SafeMode -Plan
        if ($result.arguments -notcontains '--geode:safe-mode') { throw 'Missing safe mode.' }
    }
    Check 'Normal mode reports a corrupt package' {
        Reject '*Unable to read mod metadata*' {
            & $launcher -Config $config -Runtime Legacy -Plan
        }
    }
    Check 'Legacy does not imply experimental support' {
        Reject '*only available in Current*' {
            & $launcher -Config $config -Runtime Legacy -Experimental -Plan
        }
    }
    $profiles[1].gameSha256 = '0' * 64
    Save-Config $profiles
    Check 'Changed binaries are rejected even in safe mode' {
        Reject '*Runtime file changed*' {
            & $launcher -Config $config -Runtime Legacy -SafeMode -Plan
        }
    }
    $profiles[1].executable = 'Legacy/Current.exe'
    Save-Config $profiles
    Check 'Save folders cannot collide' {
        Reject '*different EXE names*' { & $launcher -Config $config -Plan }
    }
    $profiles[1].executable = 'Current/Legacy.exe'
    Save-Config $profiles
    Check 'Runtime directories cannot collide' {
        Reject '*different game directories*' { & $launcher -Config $config -Plan }
    }
    $profiles[1].executable = 'Legacy/Legacy.exe'
    $profiles[1].gameSha256 = $profiles[1].loaderSha256
    Save-Config $profiles
    Write-Mod 'Current' '6.0.0-alpha.1' '2.2091'
    Set-Content -LiteralPath "$testRoot/Current/geode/resources/geode.loader/version" `
        -Value '5.11.2'
    Check 'Wrong resource versions are rejected' {
        Reject '*resources do not match*' {
            & $launcher -Config $config -Runtime Current -Plan
        }
    }
    Set-Content -LiteralPath "$testRoot/Current/geode/resources/geode.loader/version" `
        -Value '6.0.0-alpha.1'
    Check 'Process handoff preserves working directory and safe-mode arguments' {
        function Start-Process {
            param($FilePath, $WorkingDirectory, $ArgumentList, [switch]$PassThru)
            if ($FilePath -ne (Join-Path $testRoot 'Current/Current.exe') -or
                $WorkingDirectory -ne (Join-Path $testRoot 'Current') -or
                @($ArgumentList).Count -ne 1 -or
                $ArgumentList[0] -ne '--geode:safe-mode' -or -not $PassThru) {
                throw 'Incorrect process handoff.'
            }
            return [pscustomobject]@{Id = 4242}
        }
        $result = & $launcher -Config $config -Runtime Current -SafeMode
        if ($result.processId -ne 4242) { throw 'Process result not propagated.' }
    }
    Check 'A duplicate process is rejected before starting another' {
        function Get-Process {
            param($Name, $ErrorAction)
            return [pscustomobject]@{Path = (Join-Path $testRoot 'Current/Current.exe')}
        }
        function Start-Process { throw 'A duplicate process must not be started.' }
        Reject '*already running*' { & $launcher -Config $config -Runtime Current }
    }
}
finally {
    $resolved = [IO.Path]::GetFullPath($testRoot)
    if ((Split-Path $resolved) -ne $tempParent.TrimEnd('\', '/') -or
        (Split-Path $resolved -Leaf) -notlike 'geode-alpha-tests-*') {
        throw 'Refusing to remove an unexpected test directory.'
    }
    Remove-Item -LiteralPath $resolved -Recurse -Force
}
if ($failures) { throw "$failures launcher checks failed." }
