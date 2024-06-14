<#
  .SYNOPSIS
  Build project using the specified CMake Preset

  .DESCRIPTION
   This script will build the specified preset/configuration from the command line.
   Cmake should be in your path, if not you should run this script from a VS Developer
   Command Prompt.

  .PARAMETER PresetName
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
   [string] $PresetName,
   [string] $Target = "all",
   [switch] $Configure,
   [switch] $Build
   [switch] $Rebuild,
   [switch] $Tests
)

$ErrorActionPreference = 'Stop'

$RepoDir = Split-Path $PSScriptRoot
$saved_location = Get-Location
Set-Location $RepoDir

try
{
   Write-Host "Building project for $PresetName... using repo dir $RepoDir" -ForegroundColor Cyan

   if (!Configure -and !Build -and !Rebuild -and !Tests)
   {
      Build = $true
      Tests = $true
   }

   if ( $Configure )
   {
      cmake -S $RepoDir --preset $PresetName
   }

   if ( $Rebuild )
   {
      cmake --build --preset=$PresetName --target=$Target --clean-first
   }
   else if ( $Build )
   {
      cmake --build --preset=$PresetName --target=$Target
   }

   if ( $Tests )
   {
      ctest --preset $PresetName --output-on-failure --output-junit "$PresetName.test_results.xml"
   }
}
finally
{
   Set-Location $saved_location
}
