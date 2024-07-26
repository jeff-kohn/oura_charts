<#
  .SYNOPSIS
  Build project using the specified CMake Preset

  .DESCRIPTION
   This script will build the specified preset/configuration from the command line.
   Cmake should be in your path, if not you should run this script from a VS Developer
   Command Prompt.

  .PARAMETER Preset
  Specifies the CMake preset to use. Defaults to "win-msvc", currently the only windows preset

  .PARAMETER Clean
  If this switch is supplied, the --clean-first build will be passed to cmake to for a clean build
  Clean build DOES not delete CMake cache or re-run configure, it just deletes existing build binaries.

  .PARAMETER Tests
  This switch causes cmake to run unit tests

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 -Config="win-release"

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-debug -Configure -Build

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-release -Rebuild -Tests

#>


param
(
   [string] $Preset = "release",
   [string] $Target = "all",
   [switch] $Rebuild,
   [switch] $RunTests
)

$ErrorActionPreference = 'Stop'

$RepoDir = Split-Path $PSScriptRoot
$saved_location = Get-Location
Set-Location $RepoDir

try
{
   Write-Host "Building project for $Preset using repo dir $RepoDir..." -ForegroundColor Cyan

   if ( !(Test-Path "$RepoDir/builds/CMakeCache.txt") )
   {
      Write-Host "`r`nBuild directory not found. Use  the '-Configure' switch to run configure before building.`r`n"
      exit (-1)
   }

   if ( $Rebuild )
   {
      cmake --build --preset=$Preset --target=$Target --clean-first
   }
   else
   {
      cmake --build --preset=$Preset --target=$Target
   }

   if ( $Tests )
   {
      ctest --preset $Preset --output-on-failure --output-junit "$Preset.test_results.xml"
   }
}
finally
{
   Set-Location $saved_location
}
