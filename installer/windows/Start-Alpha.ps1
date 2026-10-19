param(
    [Parameter(Mandatory = $true)][string]$Config,
    [ValidateSet('Auto', 'Current', 'Legacy')][string]$Runtime = 'Auto',
    [switch]$SafeMode,
    [switch]$Experimental,
    [switch]$Plan
)

$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.IO.Compression.FileSystem
$configPath = (Resolve-Path -LiteralPath $Config).Path
$configRoot = Split-Path $configPath
$settings = Get-Content -LiteralPath $configPath -Raw | ConvertFrom-Json
if ($settings.schemaVersion -ne 1) { throw 'Unsupported runtime configuration.' }

function Resolve-ProfilePath([string]$Path) {
    if ([IO.Path]::IsPathRooted($Path)) { return [IO.Path]::GetFullPath($Path) }
    return [IO.Path]::GetFullPath((Join-Path $configRoot $Path))
}

function Assert-FileHash([string]$Path, [string]$Expected) {
    if ($Expected -notmatch '^[0-9a-fA-F]{64}$') {
        throw "Missing SHA-256 for $Path."
    }
    $stream = [IO.File]::OpenRead($Path)
    $algorithm = [Security.Cryptography.SHA256]::Create()
    try { $actual = [BitConverter]::ToString($algorithm.ComputeHash($stream)).Replace('-', '') }
    finally { $algorithm.Dispose(); $stream.Dispose() }
    if ($actual -ne $Expected) {
        throw "Runtime file changed: $Path. Verify the runtime before starting."
    }
}

function Read-ModMetadata([string]$Path) {
    $archive = $null
    try {
        $archive = [IO.Compression.ZipFile]::OpenRead($Path)
        $entry = $archive.GetEntry('mod.json')
        if (-not $entry) { throw "Missing mod.json in $Path." }
        $reader = New-Object IO.StreamReader($entry.Open())
        try { return $reader.ReadToEnd() | ConvertFrom-Json }
        finally { $reader.Dispose() }
    }
    catch { throw "Unable to read mod metadata from ${Path}: $_" }
    finally { if ($archive) { $archive.Dispose() } }
}

$profiles = @($settings.runtimes)
if ($profiles.Count -eq 0) { throw 'No runtimes are configured.' }
$names = @{}
$saveNames = @{}
$directories = @{}
foreach ($profile in $profiles) {
    if ($profile.id -notin @('Current', 'Legacy') -or $names[$profile.id]) {
        throw 'Runtime IDs must be unique: Current or Legacy.'
    }
    $names[$profile.id] = $true
    $executable = Resolve-ProfilePath $profile.executable
    $saveName = [IO.Path]::GetFileNameWithoutExtension($executable)
    if ($saveNames[$saveName]) {
        throw 'Use different EXE names so runtimes have separate save folders.'
    }
    $saveNames[$saveName] = $true
    $directory = Split-Path $executable
    if ($directories[$directory]) {
        throw 'Use different game directories for separate runtimes.'
    }
    $directories[$directory] = $true
    $modsPath = Join-Path $directory 'geode/mods'
    $packages = @()
    if (Test-Path -LiteralPath $modsPath) {
        $packages = @(Get-ChildItem -LiteralPath $modsPath -Filter '*.geode' -File)
    }
    $profile | Add-Member -NotePropertyName resolvedExecutable `
        -NotePropertyValue $executable
    $profile | Add-Member -NotePropertyName packages -NotePropertyValue $packages
}

if ($Runtime -eq 'Auto') {
    $withMods = @($profiles | Where-Object { $_.packages.Count -gt 0 })
    if ($withMods.Count -gt 1) {
        throw 'Both runtimes contain mods. Choose -Runtime Current or Legacy.'
    }
    if ($withMods.Count -eq 1) { $selected = $withMods[0] }
    else {
        $selected = $profiles | Where-Object id -EQ 'Current' |
            Select-Object -First 1
        if (-not $selected) { $selected = $profiles[0] }
    }
}
else {
    $selected = $profiles | Where-Object id -EQ $Runtime | Select-Object -First 1
}
if (-not $selected) { throw "Runtime $Runtime is not configured." }
if ($Experimental -and $selected.id -ne 'Current') {
    throw 'Experimental version overrides are only available in Current.'
}

$executable = $selected.resolvedExecutable
$directory = Split-Path $executable
Assert-FileHash $executable $selected.gameSha256
Assert-FileHash (Join-Path $directory 'libcocos2d.dll') $selected.engineSha256
Assert-FileHash (Join-Path $directory 'Geode.dll') $selected.loaderSha256
$resourceVersion = Get-Content -LiteralPath `
    (Join-Path $directory 'geode/resources/geode.loader/version') -Raw
if ($resourceVersion.Trim() -ne $selected.geodeVersion) {
    throw 'Loader resources do not match the configured Geode version.'
}
if ($selected.geodeVersion -notmatch '^(\d+)\.') {
    throw 'Invalid Geode version in runtime configuration.'
}
$sdkMajor = [int]$Matches[1]
if (-not $SafeMode) {
    foreach ($package in $selected.packages) {
        $metadata = Read-ModMetadata $package.FullName
        if ($metadata.geode -notmatch '^(\d+)\.') {
            throw "Invalid Geode version in $($package.Name)."
        }
        $modMajor = [int]$Matches[1]
        if ($metadata.gd -is [string]) { $gameVersion = $metadata.gd }
        else { $gameVersion = $metadata.gd.win }
        if (-not $gameVersion -or $gameVersion -eq '0.000') {
            throw "$($package.Name) does not support Windows."
        }
        $gameMatches = $gameVersion -eq '*' -or
            $gameVersion -eq $selected.gameVersion
        if (-not $Experimental -and
            ($modMajor -ne $sdkMajor -or -not $gameMatches)) {
            throw "$($package.Name) targets Geode $($metadata.geode) / " +
                "GD $gameVersion. Use a separate matching runtime."
        }
    }
}

$arguments = @()
if ($SafeMode) { $arguments += '--geode:safe-mode' }
if ($Experimental) { $arguments += '--geode:allow-incompatible-mods' }
$result = [ordered]@{
    runtime = $selected.id
    gameVersion = $selected.gameVersion
    geodeVersion = $selected.geodeVersion
    executable = $executable
    modCount = $selected.packages.Count
    arguments = $arguments
}
if ($Plan) { return [pscustomobject]$result }

$processName = [IO.Path]::GetFileNameWithoutExtension($executable)
$running = Get-Process -Name $processName -ErrorAction SilentlyContinue |
    Where-Object { $_.Path -eq $executable }
if ($running) { throw 'The selected runtime is already running.' }
$startOptions = @{ FilePath = $executable; WorkingDirectory = $directory }
if ($arguments.Count) { $startOptions.ArgumentList = $arguments }
$process = Start-Process @startOptions -PassThru
$result.processId = $process.Id
[pscustomobject]$result
