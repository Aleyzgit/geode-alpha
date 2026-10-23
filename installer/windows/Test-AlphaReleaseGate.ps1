param(
    [Parameter(Mandatory = $true)][string]$MakeNsis,
    [Parameter(Mandatory = $true)][string]$OutputDirectory
)

$ErrorActionPreference = 'Stop'
$compiler = (Resolve-Path -LiteralPath $MakeNsis).Path
$directory = (New-Item -ItemType Directory -Force -Path $OutputDirectory).FullName
$guard = Join-Path $PSScriptRoot 'AlphaReleaseGuard.nsh'
$cases = @(
    @{ Name = 'MissingBindings'; Missing = 1; Ready = 0; Error = 'unresolved Windows native bindings' },
    @{ Name = 'IncompletePort'; Missing = 0; Ready = 0; Error = 'native layout and compatibility validation' },
    @{ Name = 'VerifiedPort'; Missing = 0; Ready = 1; Error = $null }
)

foreach ($case in $cases) {
    $source = Join-Path $directory ($case.Name + '.nsi')
    $output = Join-Path $directory ($case.Name + '.exe')
    @"
!define ALPHA_UNRESOLVED_WINDOWS_BINDINGS $($case.Missing)
!define ALPHA_RELEASE_READY $($case.Ready)
!include "$guard"
Name "Alpha release gate fixture"
Unicode true
RequestExecutionLevel user
OutFile "$output"
Section
SectionEnd
"@ | Set-Content -LiteralPath $source -Encoding utf8
    $diagnostics = (& $compiler -WX -V2 $source 2>&1 | Out-String)
    $code = $LASTEXITCODE
    if ($case.Error) {
        if ($code -eq 0 -or $diagnostics -notmatch [regex]::Escape($case.Error)) {
            throw "$($case.Name) did not reject the fixture correctly: $diagnostics"
        }
    }
    elseif ($code -ne 0 -or -not (Test-Path -LiteralPath $output -PathType Leaf)) {
        throw "$($case.Name) did not compile the empty guard fixture: $diagnostics"
    }
    Write-Output "PASS: $($case.Name) release gate."
}
