param(
    [Parameter(Mandatory = $true)][string]$CurrentExe,
    [Parameter(Mandatory = $true)][string]$LegacyExe,
    [Parameter(Mandatory = $true)][string]$Output,
    [ValidatePattern('^[0-9a-fA-F]{64}$')]
    [string]$LegacyLoaderSha256 = '0c34f50454c102637038cae48c8c7dd685da3582fe474358024b1eb6833684d3',
    [switch]$Force
)

$ErrorActionPreference = 'Stop'
$outputPath = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($Output)
if ((Test-Path -LiteralPath $outputPath) -and -not $Force) {
    throw 'Configuration exists. Use -Force to replace it after verification.'
}

function Get-RuntimeHash([string]$Path) {
    $stream = [IO.File]::OpenRead($Path)
    $algorithm = [Security.Cryptography.SHA256]::Create()
    try { return [BitConverter]::ToString($algorithm.ComputeHash($stream)).Replace('-', '') }
    finally { $algorithm.Dispose(); $stream.Dispose() }
}

function New-RuntimeProfile($Id, $Executable, $GameVersion, $GameHash, $EngineHash) {
    $executable = (Resolve-Path -LiteralPath $Executable).Path
    if ([IO.Path]::GetExtension($executable) -ne '.exe') {
        throw 'The game path must name an EXE file.'
    }
    $directory = Split-Path $executable
    $gameDigest = Get-RuntimeHash $executable
    $engineDigest = Get-RuntimeHash "$directory/libcocos2d.dll"
    if ($gameDigest -ne $GameHash -or $engineDigest -ne $EngineHash) {
        throw "The $Id game and engine do not match the tested GD $GameVersion files."
    }
    $loaderDigest = Get-RuntimeHash "$directory/Geode.dll"
    $version = (Get-Content -LiteralPath `
        "$directory/geode/resources/geode.loader/version" -Raw).Trim()
    if ($Id -eq 'Legacy') {
        $expectedLoader = $LegacyLoaderSha256
        if ($version -ne '5.11.2' -or $loaderDigest -ne $expectedLoader) {
            throw 'Legacy requires Geode 5.11.2 resources and the explicitly pinned loader hash.'
        }
    }
    elseif ($version -notmatch '^6\.\d+\.\d+') {
        throw 'Current requires Geode Alpha 6 resources matching the local build.'
    }
    return [ordered]@{
        id = $Id
        executable = $executable
        gameVersion = $GameVersion
        geodeVersion = $version
        gameSha256 = $gameDigest.ToLowerInvariant()
        engineSha256 = $engineDigest.ToLowerInvariant()
        loaderSha256 = $loaderDigest.ToLowerInvariant()
    }
}

$current = New-RuntimeProfile 'Current' $CurrentExe '2.2091' `
    '4eee06d5c2e2213b4ba2b1f6233dd426bbdd0e9b29d7c375e499516b6f8f69d5' `
    '79c2f6567e5851225520803f460c6c5f6f49a2eac9afb8f3bd88d4c5697bef4e'
$legacy = New-RuntimeProfile 'Legacy' $LegacyExe '2.2081' `
    'fc5a16c292278bc2e8e078fb1d5023c2bd658322dd72712767ea70c2dd9ec6d0' `
    'eb43e5e6d6e2ce4d3109dcb11fbb6634b16fb7204da1532bbf62a359dc98b94c'
if ((Split-Path $current.executable) -eq (Split-Path $legacy.executable)) {
    throw 'Use different game directories for separate runtimes.'
}
if ([IO.Path]::GetFileNameWithoutExtension($current.executable) -eq
    [IO.Path]::GetFileNameWithoutExtension($legacy.executable)) {
    throw 'Use different EXE names so runtimes have separate save folders.'
}
$json = @{schemaVersion = 1; runtimes = @($current, $legacy)} |
    ConvertTo-Json -Depth 5
[IO.File]::WriteAllText($outputPath, $json, (New-Object Text.UTF8Encoding($true)))
Write-Output $outputPath
