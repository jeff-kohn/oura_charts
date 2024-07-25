
param
(
   [string] $Preset = "win-debug"
)

$ErrorActionPreference = 'Stop'

$RepoDir = Split-Path $PSScriptRoot
$BinDir = Join-Path $RepoDir  "build\$Preset"
$TestsDir = Join-Path $BinDir "tests"

$saved_location = Get-Location
Set-Location $TestsDir

try
{
   Write-Host "running OpenCppCoverage with the folllowing values: RepoDir=$RepoDir, BinDir=$BinDir, TestsDir=$TestsDir"
   
   & $env:ProgramFiles\OpenCppCoverage\opencppcoverage.exe --verbose `
      --modules $BinDir\tests\tests.exe                              `
      --sources $RepoDir\src                                         `
      --sources $ReporDir\include                                    `
      --excluded_sources $env:ProgramFiles                           `
      --excluded_sources "D:\a\_work\1"                              `
      --excluded_sources $env:VCPKG_INSTALLATION_ROOT                `
      --excluded_sources $BinDir\vcpkg_installed                     `
      --excluded_sources $BinDir\_deps                               `
      --export_type=cobertura:"..\code_coverage.xml"                 `
      --export_type=html:"..\code_coverage_html"                     `
      -- tests.exe
}
finally
{
   Set-Location $saved_location
}
