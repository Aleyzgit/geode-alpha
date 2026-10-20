param(
    [Parameter(Mandatory = $true)][string]$GameDirectory,
    [Parameter(Mandatory = $true)][string]$SaveDirectory
)

$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
$GameDirectory = (Resolve-Path -LiteralPath $GameDirectory).Path
$SaveDirectory = [IO.Path]::GetFullPath($SaveDirectory)
foreach ($process in @(Get-Process -Name 'GeometryDash*' -ErrorAction SilentlyContinue)) {
    if ($process.Path -and
        [IO.Path]::GetDirectoryName($process.Path) -ieq $GameDirectory) {
        throw 'Close this game copy before repairing its mods.'
    }
}

function Read-ZipText($Archive, [string]$Name) {
    $entry = $Archive.GetEntry($Name)
    if (-not $entry) { throw "Missing archive entry: $Name" }
    $reader = [IO.StreamReader]::new($entry.Open())
    try { return $reader.ReadToEnd() }
    finally { $reader.Dispose() }
}

function Get-PayloadHashes($Archive) {
    $hashes = @{}
    foreach ($entry in $Archive.Entries) {
        if ($entry.FullName -eq 'mod.json' -or $entry.FullName.EndsWith('/')) {
            continue
        }
        if ($hashes.ContainsKey($entry.FullName)) {
            throw "Duplicate archive entry: $($entry.FullName)"
        }
        $stream = $entry.Open()
        $sha = [Security.Cryptography.SHA256]::Create()
        try {
            $hashes[$entry.FullName] = [BitConverter]::ToString($sha.ComputeHash($stream))
        }
        finally { $sha.Dispose(); $stream.Dispose() }
    }
    return $hashes
}

function Repair-Prism([string]$Package) {
    if (-not (Test-Path -LiteralPath $Package)) { return }
    $archive = [IO.Compression.ZipFile]::OpenRead($Package)
    try {
        $metadata = (Read-ZipText $archive 'mod.json') | ConvertFrom-Json
        if ($metadata.id -ne 'firee.prism' -or $metadata.version -ne 'v1.10.9') {
            Write-Output 'Prism: skipped unrecognized version.'
            return
        }
        $setting = $metadata.settings.values
        if ($setting.type -ne 'custom:hack-values') {
            throw 'Unexpected Prism setting type.'
        }
        if ($setting.PSObject.Properties.Name -contains 'default') {
            Write-Output 'Prism: default already present.'
            return
        }
        $hashes = Get-PayloadHashes $archive
    }
    finally { $archive.Dispose() }

    $setting | Add-Member -NotePropertyName 'default' -NotePropertyValue @()
    $backupDirectory = Join-Path $GameDirectory 'geode/alpha-backups'
    New-Item -ItemType Directory -Force -Path $backupDirectory | Out-Null
    $token = [Guid]::NewGuid().ToString('N')
    $temporary = Join-Path $backupDirectory "prism-$token.tmp"
    $backup = Join-Path $backupDirectory "firee.prism-$token.geode"
    Copy-Item -LiteralPath $Package -Destination $temporary
    try {
        $archive = [IO.Compression.ZipFile]::Open($temporary, 'Update')
        try {
            $archive.GetEntry('mod.json').Delete()
            $entry = $archive.CreateEntry('mod.json')
            $writer = [IO.StreamWriter]::new($entry.Open(), [Text.UTF8Encoding]::new($false))
            try { $writer.Write(($metadata | ConvertTo-Json -Depth 100)) }
            finally { $writer.Dispose() }
        }
        finally { $archive.Dispose() }

        $archive = [IO.Compression.ZipFile]::OpenRead($temporary)
        try {
            $actual = Get-PayloadHashes $archive
            if ($actual.Count -ne $hashes.Count) { throw 'Archive payload changed.' }
            foreach ($key in $hashes.Keys) {
                if ($actual[$key] -ne $hashes[$key]) { throw "Payload changed: $key" }
            }
        }
        finally { $archive.Dispose() }
        [IO.File]::Replace($temporary, $Package, $backup)
        Write-Output "Prism: added empty array default; backup: $backup"
    }
    finally {
        if (Test-Path -LiteralPath $temporary) {
            Remove-Item -LiteralPath $temporary
        }
    }
}

function Initialize-MissingFile([string]$Path, [string]$Text) {
    if (Test-Path -LiteralPath $Path) { return }
    New-Item -ItemType Directory -Force -Path ([IO.Path]::GetDirectoryName($Path)) | Out-Null
    $stream = [IO.File]::Open($Path, 'CreateNew', 'Write', 'None')
    $writer = [IO.StreamWriter]::new($stream, [Text.UTF8Encoding]::new($false))
    try { $writer.Write($Text) }
    finally { $writer.Dispose() }
    Write-Output "Initialized: $Path"
}

$mods = Join-Path $GameDirectory 'geode/mods'
Repair-Prism (Join-Path $mods 'firee.prism.geode')

$eclipse = Join-Path $mods 'eclipse.eclipse-menu.geode'
if (Test-Path -LiteralPath $eclipse) {
    $archive = [IO.Compression.ZipFile]::OpenRead($eclipse)
    try {
        $metadata = (Read-ZipText $archive 'mod.json') | ConvertFrom-Json
        if ($metadata.id -eq 'eclipse.eclipse-menu' -and $metadata.version -eq 'v1.9.4') {
            $theme = Read-ZipText $archive 'resources/eclipse.eclipse-menu/megaoverlay.json'
            $null = $theme | ConvertFrom-Json
            Initialize-MissingFile (Join-Path $SaveDirectory 'geode/mods/eclipse.eclipse-menu/theme.json') $theme
        }
    }
    finally { $archive.Dispose() }
}

$betterInfo = Join-Path $mods 'cvolton.betterinfo.geode'
if (Test-Path -LiteralPath $betterInfo) {
    $archive = [IO.Compression.ZipFile]::OpenRead($betterInfo)
    try {
        $metadata = (Read-ZipText $archive 'mod.json') | ConvertFrom-Json
        if ($metadata.id -eq 'cvolton.betterinfo' -and $metadata.version -eq 'v4.4.11') {
            $cache = '{"userScoreCache":{},"levelCache":{},"levelFailures":{},"levelDateCache":{},"usernameCache":{},"levelDateStringCache":{}}'
            Initialize-MissingFile (Join-Path $SaveDirectory 'geode/mods/cvolton.betterinfo/cache_v2.json') $cache
        }
    }
    finally { $archive.Dispose() }
}
