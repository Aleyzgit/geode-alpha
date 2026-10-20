$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.IO.Compression.FileSystem
$repair = Join-Path $PSScriptRoot 'Repair-AlphaMods.ps1'
$parent = [IO.Path]::GetFullPath([IO.Path]::GetTempPath())
$root = Join-Path $parent ('geode-alpha-repair-tests-' + [Guid]::NewGuid())
$game = Join-Path $root 'game'
$save = Join-Path $root 'save'
$mods = Join-Path $game 'geode/mods'
New-Item -ItemType Directory -Force -Path $mods | Out-Null
$prism = Join-Path $mods 'firee.prism.geode'
$failures = 0

function Check([string]$Name, [scriptblock]$Test) {
    try { & $Test; Write-Output "PASS: $Name" }
    catch { $script:failures++; Write-Output "FAIL: ${Name}: $_" }
}

function Assert($Condition, [string]$Message) {
    if (-not $Condition) { throw $Message }
}

function Write-Package([string]$Path, $Entries) {
    if (Test-Path -LiteralPath $Path) { Remove-Item -LiteralPath $Path }
    $archive = [IO.Compression.ZipFile]::Open($Path, 'Create')
    try {
        foreach ($name in $Entries.Keys) {
            $writer = [IO.StreamWriter]::new($archive.CreateEntry($name).Open())
            try { $writer.Write($Entries[$name]) }
            finally { $writer.Dispose() }
        }
    }
    finally { $archive.Dispose() }
}

function Read-Entry([string]$Path, [string]$Name) {
    $archive = [IO.Compression.ZipFile]::OpenRead($Path)
    try {
        $reader = [IO.StreamReader]::new($archive.GetEntry($Name).Open())
        try { return $reader.ReadToEnd() }
        finally { $reader.Dispose() }
    }
    finally { $archive.Dispose() }
}

function Run-Repair {
    & $repair -GameDirectory $game -SaveDirectory $save | Out-Null
}

try {
    $original = '{"id":"firee.prism","version":"v1.10.9","settings":{"values":{"type":"custom:hack-values"}}}'
    Write-Package $prism @{'mod.json' = $original; 'firee.prism.dll' = 'binary fixture'; 'resource.txt' = 'resource fixture'}
    Check 'Prism default, unchanged payload and recoverable backup' {
        Run-Repair
        $metadata = (Read-Entry $prism 'mod.json') | ConvertFrom-Json
        Assert ($metadata.settings.values.PSObject.Properties.Name -contains 'default') 'Default missing.'
        Assert ($metadata.settings.values.default -is [Array]) 'Default is not an array.'
        Assert ($metadata.settings.values.default.Count -eq 0) 'Default is not empty.'
        Assert ((Read-Entry $prism 'firee.prism.dll') -eq 'binary fixture') 'DLL changed.'
        Assert ((Read-Entry $prism 'resource.txt') -eq 'resource fixture') 'Resource changed.'
        $backup = Get-ChildItem (Join-Path $game 'geode/alpha-backups') -Filter '*.geode'
        Assert (@($backup).Count -eq 1) 'Expected one backup.'
        Assert ((Read-Entry $backup.FullName 'mod.json') -eq $original) 'Backup changed.'
    }
    Check 'Repeated repair leaves the archive unchanged' {
        $before = (Get-FileHash -LiteralPath $prism).Hash
        Run-Repair
        Assert ((Get-FileHash -LiteralPath $prism).Hash -eq $before) 'Repeated rewrite.'
    }
    Check 'Missing theme and cache are initialized; existing data is preserved' {
        Write-Package (Join-Path $mods 'eclipse.eclipse-menu.geode') @{
            'mod.json' = '{"id":"eclipse.eclipse-menu","version":"v1.9.4"}'
            'resources/eclipse.eclipse-menu/megaoverlay.json' = '{"details":{"name":"fixture"}}'
        }
        Write-Package (Join-Path $mods 'cvolton.betterinfo.geode') @{
            'mod.json' = '{"id":"cvolton.betterinfo","version":"v4.4.11"}'
        }
        Run-Repair
        $theme = Join-Path $save 'geode/mods/eclipse.eclipse-menu/theme.json'
        $cache = Join-Path $save 'geode/mods/cvolton.betterinfo/cache_v2.json'
        Assert (((Get-Content $theme -Raw) | ConvertFrom-Json).details.name -eq 'fixture') 'Theme missing.'
        Assert ($null -ne ((Get-Content $cache -Raw) | ConvertFrom-Json).levelCache) 'Cache missing.'
        Set-Content -LiteralPath $theme -Value '{"custom":true}'
        Set-Content -LiteralPath $cache -Value '{"existing":true}'
        Run-Repair
        Assert (((Get-Content $theme -Raw) | ConvertFrom-Json).custom -eq $true) 'Theme overwritten.'
        Assert (((Get-Content $cache -Raw) | ConvertFrom-Json).existing -eq $true) 'Cache overwritten.'
    }
    Check 'Unrecognized Prism versions are untouched' {
        Write-Package $prism @{'mod.json' = $original.Replace('v1.10.9', 'v1.10.10')}
        $before = (Get-FileHash -LiteralPath $prism).Hash
        Run-Repair
        Assert ((Get-FileHash -LiteralPath $prism).Hash -eq $before) 'Unknown version rewritten.'
    }
    Check 'Unexpected setting types fail before changing the package' {
        Write-Package $prism @{'mod.json' = $original.Replace('custom:hack-values', 'custom:other')}
        $before = (Get-FileHash -LiteralPath $prism).Hash
        $rejected = $false
        try { Run-Repair } catch { $rejected = $true }
        Assert $rejected 'Expected rejection.'
        Assert ((Get-FileHash -LiteralPath $prism).Hash -eq $before) 'Package changed after rejection.'
    }
}
finally {
    $absolute = [IO.Path]::GetFullPath($root)
    $prefix = $parent.TrimEnd('\', '/') + [IO.Path]::DirectorySeparatorChar
    if (-not $absolute.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase) -or
        [IO.Path]::GetFileName($absolute) -notlike 'geode-alpha-repair-tests-*') {
        throw 'Refusing cleanup outside the temporary test directory.'
    }
    Remove-Item -LiteralPath $absolute -Recurse -Force
}
if ($failures) { throw "$failures mod repair tests failed." }
