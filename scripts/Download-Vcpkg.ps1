<#
  .SYNOPSIS
  Set up VCPKG for C++ library management

  .DESCRIPTION
  This script will clone to VCPKG repo to a folder of your choosing and set up the
  VCPKG_INSTALLATION_ROOT environment variable to reference it. If the specified folder
  already contains a repo, the script will pull latest.

  .PARAMETER GithubUrl
  The URL to clone the VCPKG repo from. should probably just be able to take the default
  unless they move it or you want to use SSH.

  .PARAMETER Bootstrap
  If this switch is set, we will run the boostrap-vcpkg script which will download the
  latest EXE of VCPKG (or build it if it can't be downloaded)

  .PARAMETER RunIntegrate
  If this switch is set, we'll run the "vcpkg integrate install" command after downloading
  VCPKG. This ensures that VCPKG can be used by Visual Studio (or VSCode)

  .INPUTS
  None.

  .OUTPUTS
  None.

  .EXAMPLE
  PS> scripts/Launch-DeveloperShell.ps1 -TargetArch x86

  .EXAMPLE
  PS> scripts/Launch-DeveloperShell.ps1 -TargetArch x86 -PlatformToolset 14.35
#>


param(
   [string] $GithubUrl = "https://github.com/microsoft/vcpkg",
   [switch] $Bootstrap,
   [switch] $RunIntegrate
)


# This script downloads VCPKG to ./external/vcpkg and sets the environment
# variable VCPKG_INSTALLATION_ROOT to this location so that project builds can find it.
#
# If the vcpkg repo already exists at the specified location, we'll just pull latest

$ErrorActionPreference = 'Stop'
$saved_location = Get-Location

try {
   $repo_dir = Split-Path $PSScriptRoot

   write-host "Repo dir is $repo_dir"
   $vcpkg_dir = Join-Path $repo_dir "external" | Join-Path -ChildPath "vcpkg"
   $git_path = Join-Path $vcpkg_dir ".git"

   Set-Location $RepoDir

   Write-Host "`nDownloading VCPKG to $vcpkg_dir" -ForegroundColor Cyan
   if (Test-Path $git_path)
   {
      Write-Host "`nVCPKG repo already exists in the specified location, pulling latest" -ForegroundColor Cyan
      Set-Location $vcpkg_dir
      git pull
      Write-Host "...done" -ForegroundColor Cyan
   }
   else{
      Write-Host "`nCloning VCPKG into $vcpkg_dir..." -ForegroundColor Cyan
      Set-Location (Join-Path $repo_dir "external")
      git clone $GithubUrl
      Write-Host "...done" -ForegroundColor Cyan
   }

   if ($PSVersionTable.Platform -eq "Unix")
   {
      [Environment]::SetEnvironmentVariable("VCPKG_INSTALLATION_ROOT", $vcpkg_dir, "User")
      echo "export VCPKG_INSTALLATION_ROOT=$vcpkg_dir" >> ~/.bashrc
      if ($Bootstrap)
      {
         Set-Location $vcpkg_dir
         ./bootstrap-vcpkg.sh
      }
   }
   else
   {
      [Environment]::SetEnvironmentVariable("VCPKG_INSTALLATION_ROOT", $vcpkg_dir, "User")
      if ($Bootstrap)
      {
         Set-Location $vcpkg_dir
         ./bootstrap-vcpkg
      }
   }

   if ($RunIntegrateCmd)
   {
      Set-Location $vcpkg_dir
      ./vcpkg integrate install
   }
   Write-Host "`nVCPKG setup complete. You'll need to close and reopen any open CMD/Powershell consoles before the VCPKG_INSTALLATIN_ROOT environment variable will be available (on linue just run `"source ~/.bashrc`")"
}
catch{
   foreach ($err in $Error)
   {
      Write-Error $err.ToString()
   }

}
finally{
   Set-Location $saved_location
}


