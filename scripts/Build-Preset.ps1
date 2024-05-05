<#
  .SYNOPSIS
  Build project using the specified CMake Preset

  .DESCRIPTION
   This script will build the specified preset/configuration from the command line.
   cmake should be in your path, if not you should run this script from a VS Developer
   Command Prompt.

  .PARAMETER PresetName
  Required - Specifies the build preset to use. See CMakePresets.json for the full list.

  .PARAMETER SkipConfigure
  If set, caauses the 'configure' step to be skipped and just run the build command. You can set this
  flag if you're sure no CMakeLists.txt or CMakePresets changes have been and you just want
  to compile/build

  .PARAMETER Rebuild
  If this switch is supplied, the --clean-first build will be passed to cmake to for a clean build
  Note this is not the same as deleting the cache, which is required if you've made changes
  to CMakeLists.txt or CMakePresets.json, use scripts/reset-cache to for full reset of the casche

  .PARAMETER SkipUnitTests
  This switch causes cmake to skip running unit tests after building.

  .INPUTS
  None.

  .OUTPUTS
  None.

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-x64-release

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-x86-debug -SkipUnitTests

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 win-x64-release -RunConfigure $false -Rebuild

#>


param
(
   [string] $PresetName,
   [string] $Target = "all",
   [switch] $SkipConfigure,
   [switch] $Rebuild,
   [switch] $SkipUnitTests
)

$ErrorActionPreference = 'Stop'

$RepoDir = Split-Path $PSScriptRoot
$DevPromptCmd = Join-Path $PSScriptRoot Launch-DeveloperShell.ps1

$saved_location = Get-Location
Set-Location $RepoDir

try
{
   Write-Host "Building project for $PresetName..." -ForegroundColor Cyan
   & $DevPromptCmd

   if (! $SkipConfigure)
   {
      cmake -S $RepoDir --preset $PresetName
   }

   if ($Rebuild)
   {
      cmake --build --preset=$PresetName --target=$Target --clean-first
   }
   else
   {
      cmake --build --preset=$PresetName --target=$Target
   }

   if ( ! $SkipUnitTests)
   {
      ctest --preset $PresetName --output-on-failure --output-junit "$PresetName.test_results.xml"
   }
}
finally
{
   Set-Location $saved_location
}
