
param
(
   [string] $Preset
)

$ErrorActionPreference = 'Stop'

$RepoDir = Split-Path $PSScriptRoot
$BinDir = Join-Path $RepoDir  "build\out\$Preset"
$TestsDir = Join-Path $BinDir "tests"

$saved_location = Get-Location
Set-Location $TestsDir

try
{
   & $env:ProgramFiles\OpenCppCoverage\opencppcoverage.exe --verbose `
      --modules $BinDir\tests\tests.exe                              `
      --sources $RepoDir\src                                         `
      --sources $ReporDir\include                                    `
      --excluded_sources $env:ProgramFiles                           `
      --excluded_sources "D:\"                                       `
      --excluded_sources $env:VCPKG_INSTALLATION_ROOT                `
      --excluded_sources $BinDir\vcpkg_installed                     `
      --export_type=cobertura:"../code_coverage.xml"                 `
      -- tests.exe
}
finally
{
   Set-Location $saved_location
}
