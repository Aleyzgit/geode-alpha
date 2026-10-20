param([Parameter(Mandatory = $true)][string]$Executable)

$ErrorActionPreference = 'Stop'
$game = (Resolve-Path -LiteralPath $Executable).Path
$directory = Split-Path $game
$name = [IO.Path]::GetFileNameWithoutExtension($game)
if (Get-Process -Name $name -ErrorAction SilentlyContinue) {
    throw 'Close this test game before running native validation.'
}
$expected = @(
    'Native PVR loader rejects invalid headers',
    'Native level-manager constructor',
    'Native level-manager caches',
    'Native easing',
    'Native integer dictionary merge',
    'Native enhanced object constructor',
    'Native robot factory',
    'Native lightning properties and strike',
    'Native Google Play singleton',
    'Unresolved hook rejected',
    'Native level progress layout',
    'Native integer variable',
    'Native local level manager',
    'Boolean setting activation',
    'Incompatible version gates',
    'Invalid platform rejected',
    'Async mod save',
    'Native achievement lookup',
    'Native animation cache',
    'Native download state',
    'Native saved lists',
    'Native user coin state',
    'Native completed level keys',
    'Native secret chests',
    'Native oriented bounds',
    'Native item counts',
    'Native audio state and channels',
    'Native coin key namespaces',
    'Native pulse colors',
    'Native editor object offsets',
    'Native scroll pages and dots',
    'Native user identity caches',
    'Native setup dictionaries',
    'Native color interpolation',
    'Native color channel labels',
    'Native hue color matrix',
    'Native animation priority',
    'Native completion reset',
    'Native options containers',
    'Native particle numeric records',
    'Native paged base construction',
    'Native level upload callback',
    'Native user info callback',
    'Native admin queue callbacks'
)

foreach ($mode in @('Strict', 'Experimental')) {
    $arguments = @('--geode:alpha-self-test', '--geode:alpha-self-test-exit')
    if ($mode -eq 'Experimental') { $arguments += '--geode:allow-incompatible-mods' }
    $started = Get-Date
    $process = Start-Process -FilePath $game -WorkingDirectory $directory `
        -WindowStyle Hidden -ArgumentList $arguments -PassThru
    if (-not $process.WaitForExit(30000)) {
        $process.Kill()
        throw "$mode native test timed out."
    }
    $log = Get-ChildItem -LiteralPath (Join-Path $directory 'geode/logs') -Filter '*.log' |
        Sort-Object LastWriteTime -Descending | Select-Object -First 1
    if (-not $log -or $log.LastWriteTime -lt $started) {
        throw "$mode did not produce a test log."
    }
    $text = Get-Content -LiteralPath $log.FullName -Raw
    foreach ($check in $expected) {
        if ($text -notmatch ([regex]::Escape("[ALPHA TEST] ${check}: PASS"))) {
            throw "$mode failed check '$check'. Inspect $($log.FullName)"
        }
    }
    if ($text -match '\[ALPHA TEST\].*: FAIL|(?m)^\d\d:\d\d:\d\d ERROR\b' -or
        $text -notmatch 'Shutdown complete\.') {
        throw "$mode failed to shut down cleanly. Inspect $($log.FullName)"
    }
    $crashDirectory = Join-Path $directory 'geode/crashlogs'
    if (Test-Path -LiteralPath $crashDirectory) {
        $crashes = @(Get-ChildItem -LiteralPath $crashDirectory -File |
            Where-Object LastWriteTime -GE $started)
        if ($crashes.Count) { throw "$mode produced a crash report." }
    }
    Write-Output "PASS: $mode, $($expected.Count) native checks and clean shutdown."
    Write-Output $log.FullName
}
