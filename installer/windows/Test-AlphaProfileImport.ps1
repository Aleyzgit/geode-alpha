$ErrorActionPreference = 'Stop'
$importer = Join-Path $PSScriptRoot 'Import-AlphaProfile.ps1'
$tempParent = [IO.Path]::GetFullPath([IO.Path]::GetTempPath())
$root = Join-Path $tempParent ('alpha-import-tests-' + [guid]::NewGuid().ToString('N'))
$failures = 0
function Check([string]$Name, [scriptblock]$Test) {
    try { & $Test; Write-Output "PASS: $Name" }
    catch { $script:failures++; Write-Output "FAIL: ${Name}: $_" }
}
function Assert([bool]$Value) { if (-not $Value) { throw 'Assertion failed.' } }
function Reject([string]$Pattern, [scriptblock]$Test) {
    try { & $Test | Out-Null }
    catch {
        if ($_.Exception.Message -notlike $Pattern) { throw }
        return
    }
    throw "Expected rejection: $Pattern"
}
function Put([string]$Path, [string]$Value) {
    New-Item -ItemType Directory -Path (Split-Path $Path) -Force | Out-Null
    [IO.File]::WriteAllText($Path, $Value)
}
function Read([string]$Path) { return [IO.File]::ReadAllText($Path) }
$paths = @{
    SourceGame = "$root/source-game"; SourceSave = "$root/source-save"
    TargetGame = "$root/target-game"; TargetSave = "$root/target-save"
}
try {
    Put "$root/source-game/geode/mods/old-mod.geode" 'original mod bytes'
    Put "$root/source-game/geode/config/old-mod/settings.json" '{"enabled":true}'
    Put "$root/source-game/geode/resources/old-mod/icon.png" 'mod resource'
    Put "$root/source-game/geode/resources/geode.loader/version" '5.11.2'
    Put "$root/source-game/geode/temp/ignored.dat" 'stale cache'
    Put "$root/source-game/Geode.dll" 'old loader'
    Put "$root/source-save/CCGameManager.dat" 'game progress'
    Put "$root/source-save/geode/mods/old-mod/data.json" 'mod progress'
    Put "$root/target-game/geode/resources/geode.loader/version" '6.0.0'
    Put "$root/target-game/Geode.dll" 'new loader'
    Check 'Plan makes no profile changes' {
        $plan = @(& $importer @paths -Plan)
        Assert ($plan.Count -eq 5)
        Assert (-not (Test-Path -LiteralPath "$root/target-save"))
        Assert (-not (Test-Path -LiteralPath "$root/target-game/geode/alpha-backups"))
    }
    Check 'Packages, settings, resources and saves retain exact bytes' {
        & $importer @paths | Out-Null
        Assert ((Read "$root/target-game/geode/mods/old-mod.geode") -eq 'original mod bytes')
        Assert ((Read "$root/target-game/geode/config/old-mod/settings.json") -eq '{"enabled":true}')
        Assert ((Read "$root/target-game/geode/resources/old-mod/icon.png") -eq 'mod resource')
        Assert ((Read "$root/target-save/CCGameManager.dat") -eq 'game progress')
        Assert ((Read "$root/target-save/geode/mods/old-mod/data.json") -eq 'mod progress')
        Assert ((Read "$root/source-save/CCGameManager.dat") -eq 'game progress')
    }
    Check 'Loader binaries, loader resources and caches remain untouched' {
        Assert ((Read "$root/target-game/Geode.dll") -eq 'new loader')
        Assert ((Read "$root/target-game/geode/resources/geode.loader/version") -eq '6.0.0')
        Assert (-not (Test-Path -LiteralPath "$root/target-game/geode/temp"))
    }
    Check 'Repeated imports do not create backups or change files' {
        $before = @(Get-ChildItem "$root/target-game/geode/alpha-backups" -Directory).Count
        & $importer @paths | Out-Null
        Assert (@(Get-ChildItem "$root/target-game/geode/alpha-backups" -Directory).Count -eq $before)
    }
    Check 'Conflicts reject the complete import before mutation' {
        Put "$root/source-game/geode/mods/new-mod.geode" 'new package'
        Put "$root/target-save/CCGameManager.dat" 'target progress'
        Reject '*different target file*' { & $importer @paths }
        Assert (-not (Test-Path -LiteralPath "$root/target-game/geode/mods/new-mod.geode"))
        Assert ((Read "$root/target-save/CCGameManager.dat") -eq 'target progress')
    }
    Check 'Explicit replacement keeps recoverable previous saves' {
        & $importer @paths -ReplaceExisting | Out-Null
        $saved = @(Get-ChildItem "$root/target-game/geode/alpha-backups" -Recurse -Filter 'CCGameManager.dat' |
            Where-Object { $_.FullName -like '*previous*' })
        Assert ($saved.Count -eq 1)
        Assert ((Read $saved[0].FullName) -eq 'target progress')
        Assert ((Read "$root/target-save/CCGameManager.dat") -eq 'game progress')
        Assert ((Read "$root/target-game/geode/mods/new-mod.geode") -eq 'new package')
    }
    Check 'Identical and nested source/target roots reject' {
        $nested = $paths.Clone(); $nested.TargetSave = "$root/source-save/nested"
        Reject '*must be separate*' { & $importer @nested }
        $identical = $paths.Clone(); $identical.TargetGame = $identical.SourceGame
        Reject '*must be separate*' { & $importer @identical }
    }
    Check 'Destination junctions reject before writing outside the profile' {
        $linkGame = "$root/link-game"
        New-Item -ItemType Directory -Path "$root/outside", "$linkGame/geode" -Force | Out-Null
        New-Item -ItemType Junction -Path "$linkGame/geode/mods" -Target "$root/outside" | Out-Null
        $linked = $paths.Clone(); $linked.TargetGame = $linkGame
        try { Reject '*symbolic link or junction*' { & $importer @linked } }
        finally { [IO.Directory]::Delete("$linkGame/geode/mods") }
        Assert (@(Get-ChildItem "$root/outside" -Force).Count -eq 0)
    }
}
finally {
    $resolved = [IO.Path]::GetFullPath($root)
    if ($resolved.StartsWith($tempParent.TrimEnd('\') + '\', [StringComparison]::OrdinalIgnoreCase)) {
        Remove-Item -LiteralPath $resolved -Recurse -Force
    }
}
if ($failures) { throw "$failures profile import tests failed." }
