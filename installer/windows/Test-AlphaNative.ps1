param([Parameter(Mandatory = $true)][string]$Executable)

$ErrorActionPreference = 'Stop'
$game = (Resolve-Path -LiteralPath $Executable).Path
$directory = Split-Path $game
$name = [IO.Path]::GetFileNameWithoutExtension($game)
if (Get-Process -Name $name -ErrorAction SilentlyContinue) {
    throw 'Close this test game before running native validation.'
}
$expected = @(
    'Native application scene delegate layout',
    'Native application setting accessors',
    'Native music search state layout',
    'Native music tag and artist filters',
    'Native music future filter counts',
    'SDK single music filter helpers',
    'Native music match control',
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
    'Native particle allocation boundary',
    'Native single button configuration reset',
    'Native shader construction and split tweens',
    'Native trigger toggle controls and delegate guard',
    'Native find popup group value boundary',
    'SDK find popup native window initialization',
    'Native gameplay event dispatch and frame cache',
    'Native level option initialization and callbacks',
    'Native middleground construction boundary',
    'Native middleground initialization',
    'Native middleground tile resizing',
    'Native enter easing buffers and cache',
    'SDK enter easing buffer regeneration',
    'Native custom enter effect registration',
    'Native custom enter effect removal',
    'Native extra game layer transforms',
    'Native screen button positioning',
    'Native achievement unlock application',
    'Native item information factories',
    'Native local level deletion',
    'Native cached level deletion',
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
    'Native option page counts',
    'Native active path selection',
    'Native video option construction',
    'Native parental option construction',
    'Native video page counts',
    'Native parental option page counts',
    'Native particle numeric records',
    'Native paged base construction',
    'Native level upload callback',
    'Native user info callback',
    'Native admin queue callbacks',
    'Native button background sizing',
    'Native upload popup factory',
    'Native local key bindings',
    'Native global key lookup dispatch',
    'Native editor window base',
    'Native scroll base ownership',
    'Native template browser construction',
    'Native guidelines construction',
    'Native text popup factory',
    'Native local level ordering',
    'Native music resource cache',
    'Native music manager construction',
    'Native audio mixer restart',
    'Native suspended audio channels',
    'Native level search filters',
    'Native search page construction',
    'Native request cell boundary',
    'Native score layout',
    'Native score construction boundary',
    'Native score dictionary and request selection',
    'Native reward stat keys',
    'Native cached user lists',
    'Native editor layer layout',
    'Native editor construction boundary',
    'Native fresh editor construction',
    'Native editor object creation and removal',
    'Native editor minimum X transform',
    'Native object common initialization',
    'Native object custom color child',
    'Native object reserved color blending',
    'Native editor object selection filters',
    'Native multiplayer download cache removal',
    'Native number input layout',
    'Native number input construction boundary',
    'Native number input controls and delegate',
    'Native arithmetic gate construction',
    'Native animated counter updates',
    'Native gameplay counter labels',
    'Native manager save callback slots',
    'Native gameplay virtual slots',
    'Native SFX delegate virtual slots',
    'Native gameplay virtual dispatch',
    'Native typed SFX delegate callbacks',
    'Native tab and browser layouts',
    'Native tab and browser construction boundaries',
    'Native game settings tabs',
    'Native keybinding popup construction',
    'Native browser page and folder labels',
    'Native empty timeline positions',
    'Native timeline positions match editor',
    'Native keybinding tabs and search',
    'Native typed keybinding settings entries',
    'Native keybinding settings entry controls',
    'Native level list reordering',
    'Native level list parsing',
    'Native paged reload dispatch',
    'Native popup virtual boundaries',
    'Native typed picker contracts',
    'Native popup touch priority lifetime',
    'Native arithmetic gate SDK construction',
    'Native effect manager layout',
    'Native triggered ID removal',
    'Native effect manager SDK construction',
    'Native options data and popup layouts',
    'Native options factory and row',
    'Native options SDK construction',
    'Native options search filters',
    'Native options SDK initialization'
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
