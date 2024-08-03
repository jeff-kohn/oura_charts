<#
  .SYNOPSIS
  Build project using the specified CMake Preset

  .DESCRIPTION
   This script will build the specified preset/configuration from the command line.
   Cmake should be in your path, if not you should run this script from a VS Developer
   Command Prompt.

  .PARAMETER Preset
  Optional - defaults to "win-msvc", the only preset currently defined for Windows

  .PARAMETER WipeClean
  Optional - pass true to delete contents of the build director for this preset before
  running configure.
  
  .PARAMETER Force
  Use with -WipeClean to avoid interactive prompt before cleaning build directory.

  .EXAMPLE
  PS> scripts/Run-Confiigure.ps1 win-release

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 -Preset "win-msvc" -WipeClean

  .EXAMPLE
  PS> scripts/Build-Preset.ps1 -WipeClean -Force
#>

param
(
   [string] $Preset = "win-msvc",
   [switch] $WipeClean,
   [switch] $Force
)

$ErrorActionPreference = 'Stop'

$RepoDir = Split-Path $PSScriptRoot
$saved_location = Get-Location
Set-Location $RepoDir

try
{
   Write-Host "Configuring project for $Preset... using repo dir $RepoDir" -ForegroundColor Cyan

   if ($WipeClean)
   {
      if ($Force)
      {
         scripts/Reset-Cache.ps1 -Force
      }
      else{
         scripts/Reset-Cache.ps1
      }
   }
   cmake -S $RepoDir --preset $Preset
}
finally
{
   Set-Location $saved_location
}
