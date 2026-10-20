param(
    [Parameter(Mandatory = $true)][string]$SourceGame,
    [Parameter(Mandatory = $true)][string]$SourceSave,
    [Parameter(Mandatory = $true)][string]$TargetGame,
    [Parameter(Mandatory = $true)][string]$TargetSave,
    [switch]$ReplaceExisting,
    [switch]$Plan
)

$ErrorActionPreference = 'Stop'

function Full-Directory([string]$Path) {
    $full = [IO.Path]::GetFullPath($Path)
    if ($full -eq [IO.Path]::GetPathRoot($full)) { return $full }
    return $full.TrimEnd('\', '/')
}

$SourceGame = Full-Directory (Resolve-Path -LiteralPath $SourceGame).Path
$SourceSave = Full-Directory (Resolve-Path -LiteralPath $SourceSave).Path
$TargetGame = Full-Directory $TargetGame
$TargetSave = Full-Directory $TargetSave
$roots = @($SourceGame, $SourceSave, $TargetGame, $TargetSave)
for ($i = 0; $i -lt $roots.Count; $i++) {
    for ($j = $i + 1; $j -lt $roots.Count; $j++) {
        if ($roots[$i] -ieq $roots[$j] -or
            $roots[$i].StartsWith($roots[$j] + '\', [StringComparison]::OrdinalIgnoreCase) -or
            $roots[$j].StartsWith($roots[$i] + '\', [StringComparison]::OrdinalIgnoreCase)) {
            throw 'Source and target directories must be separate and must not contain one another.'
        }
    }
}
function Assert-PlainPath([string]$Path) {
    $ancestor = Full-Directory $Path
    while ($ancestor) {
        if ((Test-Path -LiteralPath $ancestor) -and
            ((Get-Item -LiteralPath $ancestor -Force).Attributes -band [IO.FileAttributes]::ReparsePoint)) {
            throw "Profile paths must not traverse a symbolic link or junction: $ancestor"
        }
        $ancestor = Split-Path $ancestor -Parent
    }
}
foreach ($root in $roots) { Assert-PlainPath $root }
foreach ($root in $roots) {
    if ((Test-Path -LiteralPath $root) -and -not (Test-Path -LiteralPath $root -PathType Container)) {
        throw "A profile root must be a directory: $root"
    }
}
foreach ($process in @(Get-Process -Name 'GeometryDash*' -ErrorAction SilentlyContinue)) {
    if ($process.Path -and [IO.Path]::GetDirectoryName($process.Path) -iin @($SourceGame, $TargetGame)) {
        throw 'Close the source and target game copies before importing a profile.'
    }
}

$files = [Collections.Generic.List[object]]::new()
function Add-Tree([string]$Source, [string]$Target, [string]$Category) {
    if (-not (Test-Path -LiteralPath $Source)) { return }
    Assert-PlainPath $Source
    $entries = @(Get-ChildItem -LiteralPath $Source -Force -Recurse)
    foreach ($entry in $entries) {
        if ($entry.Attributes -band [IO.FileAttributes]::ReparsePoint) {
            throw "Profile trees must not contain symbolic links or junctions: $($entry.FullName)"
        }
        if ($entry.PSIsContainer) { continue }
        $relative = $entry.FullName.Substring($Source.Length + 1)
        $destination = Join-Path $Target $relative
        Assert-PlainPath $destination
        $digest = (Get-FileHash -LiteralPath $entry.FullName -Algorithm SHA256).Hash
        $previousHash = $null
        if (Test-Path -LiteralPath $destination) {
            if (-not (Test-Path -LiteralPath $destination -PathType Leaf)) {
                throw "A directory occupies a destination file: $destination"
            }
            $previousHash = (Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash
            if ($digest -eq $previousHash) { continue }
            if (-not $ReplaceExisting) {
                throw "A different target file exists: $destination. Use -ReplaceExisting to back it up and replace it."
            }
        }
        $files.Add([pscustomobject]@{
            source = $entry.FullName; destination = $destination; sha256 = $digest
            previousSha256 = $previousHash; category = $Category; relative = $relative
        })
    }
}

Add-Tree "$SourceGame/geode/mods" "$TargetGame/geode/mods" 'packages'
Add-Tree "$SourceGame/geode/config" "$TargetGame/geode/config" 'config'
# Loader resources must always come from the matching loader package.
$resources = "$SourceGame/geode/resources"
if (Test-Path -LiteralPath $resources) {
    foreach ($resource in @(Get-ChildItem -LiteralPath $resources -Directory -Force)) {
        if ($resource.Name -eq 'geode.loader') { continue }
        Add-Tree $resource.FullName "$TargetGame/geode/resources/$($resource.Name)" "resources/$($resource.Name)"
    }
}
Add-Tree $SourceSave $TargetSave 'saves'
if ($Plan) {
    $files | Select-Object category, relative, sha256, previousSha256
    return
}
if (-not $files.Count) { Write-Output 'Profile is already imported.'; return }

$backup = Join-Path $TargetGame ('geode/alpha-backups/profile-' + [guid]::NewGuid().ToString('N'))
$completed = [Collections.Generic.List[object]]::new()
Assert-PlainPath $backup
New-Item -ItemType Directory -Path $backup -Force | Out-Null
try {
    # Stage and verify the entire source before changing any destination file.
    foreach ($file in $files) {
        $staged = Join-Path $backup "incoming/$($file.category)/$($file.relative)"
        New-Item -ItemType Directory -Path (Split-Path $staged) -Force | Out-Null
        Copy-Item -LiteralPath $file.source -Destination $staged
        if ((Get-FileHash -LiteralPath $staged -Algorithm SHA256).Hash -ne $file.sha256) {
            throw "Source changed during import: $($file.source)"
        }
        $file | Add-Member -NotePropertyName staged -NotePropertyValue $staged
        $previous = Join-Path $backup "previous/$($file.category)/$($file.relative)"
        $file | Add-Member -NotePropertyName previous -NotePropertyValue $previous
        if ($file.previousSha256) {
            New-Item -ItemType Directory -Path (Split-Path $previous) -Force | Out-Null
            Copy-Item -LiteralPath $file.destination -Destination $previous
            if ((Get-FileHash -LiteralPath $previous -Algorithm SHA256).Hash -ne $file.previousSha256) {
                throw "Destination changed during import: $($file.destination)"
            }
        }
    }
    foreach ($file in $files) {
        Assert-PlainPath $file.destination
        $present = Test-Path -LiteralPath $file.destination
        if (($file.previousSha256 -and (-not $present -or
                (Get-FileHash -LiteralPath $file.destination -Algorithm SHA256).Hash -ne $file.previousSha256)) -or
            (-not $file.previousSha256 -and $present)) {
            throw "Destination changed during import: $($file.destination)"
        }
    }
    foreach ($file in $files) {
        New-Item -ItemType Directory -Path (Split-Path $file.destination) -Force | Out-Null
        $completed.Add($file)
        Copy-Item -LiteralPath $file.staged -Destination $file.destination -Force
        if ((Get-FileHash -LiteralPath $file.destination -Algorithm SHA256).Hash -ne $file.sha256) {
            throw "Imported file failed verification: $($file.destination)"
        }
    }
    $files | Select-Object category, relative, sha256, previousSha256 |
        ConvertTo-Json -Depth 4 | Set-Content -LiteralPath "$backup/manifest.json" -Encoding UTF8
    Write-Output "Imported $($files.Count) files. Backup: $backup"
}
catch {
    foreach ($file in $completed) {
        if ($file.previousSha256) {
            Copy-Item -LiteralPath $file.previous -Destination $file.destination -Force
        }
        elseif (Test-Path -LiteralPath $file.destination -PathType Leaf) {
            Remove-Item -LiteralPath $file.destination
        }
    }
    throw
}
