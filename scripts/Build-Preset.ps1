<#
  .SYNOPSIS
  Build project using the specified CMake Preset

  .DESCRIPTION
   This script will build the specified preset/configuration from the command line.
   Cmake should be in your path, if not you should run this script from a VS Developer
   Command Prompt.

  .PARAMETER Preset
  Required - Specifies the build preset to use. See CMakePresets.json for the full list.

  .PARAMETER Configure
  If set, caauses the 'configure' step to be skipped and just run the build command. You can set this
  flag if you're sure no CMakeLists.txt or CMakePresets changes have been and you just want
  to compile/build

  .PARAMETER Build
  If this switch is supplied, a regular build will be run

  .PARAMETER Rebuild
  If this switch is supplied, the --clean-first build will be passed to cmake to for a clean build
  Note this is not the same as deleting the cache, which is required if you've made changes
  to CMakeLists.txt or CMakePresets.json, use scripts/reset-cache to for full reset of the casche

  .PARAMETER Tests
  This switch causes cmake to run unit tests

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-release

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-debug -Configure -Build

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-release -Rebuild -Tests

#>


param
(
   [string] $Preset = "win-debug",
   [string] $Target = "all",
   [switch] $Configure,
   [switch] $Build,
   [switch] $Rebuild,
   [switch] $Tests
)

$ErrorActionPreference = 'Stop'

$RepoDir = Split-Path $PSScriptRoot
$saved_location = Get-Location
Set-Location $RepoDir

try
{
   Write-Host "Building project for $Preset... using repo dir $RepoDir" -ForegroundColor Cyan

   if (!$Configure -and !$Build -and !$Rebuild -and !$Tests)
   {
      if (Test-Path "$RepoDir/build/out/$Preset")
      {
         $Build = $true
         $Tests = $true
      }
      else{
         Write-Host "`r`nBuild directory not found. Use  the '-Configure' switch to run configure before building.`r`n"
         exit (-1)
      }
   }

   if ( $Configure )
   {
      cmake -S $RepoDir --preset $Preset
   }

   if ( $Rebuild )
   {
      cmake --build --preset=$Preset --target=$Target --clean-first
   }
   elseif ( $Build )
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
