<#
  .SYNOPSIS
  Launch a Developer Command Prompt

  .DESCRIPTION
   This script will create the necessary build environment for the project so that the
   needed environment variables for using MSVC are set.

  .PARAMETER Arch
   Specifies the target architecture you're building for (x86 or amd64)

  .PARAMETER HostArch
   Specified the host architecture that the build tools should run on (x86 or amd64). It's
   recommended to use amd64 unless you have a specific reason for using x86

  .PARAMETER PlatformToolset
   Specifies the platform toolset version to use for building (14.2x, 14.3x, etc). Currently
   defaults to "14.29", which is the latest VS2019 compiler toolset at the time of writing.


  .EXAMPLE
   PS> scripts/Launch-DeveloperShell.ps1 -TargetArch x86

  .EXAMPLE
   PS> scripts/Launch-DeveloperShell.ps1 -TargetArch x86 -PlatformToolset 14.35

#>


param (
   [string] $Arch = "amd64",
   [string] $HostArch = "amd64"
)

$ErrorActionPreference = 'Stop'

$VS2022Cmds = "$Env:ProgramFiles\Microsoft Visual Studio\2022\Professional\Common7\Tools\Launch-VsDevShell.ps1", `
              "$Env:ProgramFiles\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1", `
              "$Env:ProgramFiles\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\Launch-VsDevShell.ps1"

Write-Output "`nLoooking for Visual Studio installation for Launch-VsDevShell.ps1..."
$found = $false
foreach ($path in $VS2022Cmds)
{
   if (Test-Path($path))
   {
      $found = $true
      Write-Output "Found [$path]"

      # run the script to set up the environment
      $params = " -Arch $Arch =HostArg $HostArch "
      & $path $params

      Break
   }
   else {
      Write-Output "[$path] not found."
   }
}

if (-not $found)
{
   Write-Warning "`nCould not find Visual Studio 2022. If you get build errors, you may need to manually launch VS Developer
                  Command Prompt powershell before running build. Make sure you run the developer shell for the correct target architecture"
}

