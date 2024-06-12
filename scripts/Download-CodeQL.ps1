<#
  .SYNOPSIS
  Downloads the lastest CodeQL CLI bundle to the specified folder location. 

  .DESCRIPTION
  The latest CodeQL bundle will be downloaded and extracted to a CodeQL subfolder in the 
  requested location. The appropriate version will be downloaded based on the target platform
  we're currently running on (currently supports linux/windows)

  .PARAMETER InstallFolder
  The folder that the CodeQL folder should be extract to. Defaults to the current directory,
  and if the specified folder does not exist it will be created.
  Note that the CodeQL zip contains a CodeQL subfolder, so if you specify C:\CodeQL, 
  the binaries will actually end up in C:\CodeQL\CodeQL. 

  .PARAMETER ForceOverwrite
  If this flag is specified, the code-ql bundle TAR will be extracted to the sepecified  
  location even if it already exists, overwriting any existing content. If this flag is 
  not specified and the target install folder already exists, the operation will be aborted.

  .EXAMPLE
  PS> scripts/Launch-DeveloperShell.ps1 -TargetArch x86 

  .EXAMPLE
  PS> scripts/Launch-DeveloperShell.ps1 -TargetArch x86 -PlatformToolset 14.35
#>

param(
   [string] $InstallFolder = ".",
   [switch] $ForceOverwrite
)

$ErrorActionPreference = 'Stop'

$saved_location = Get-Location
$zip_path = ""

try 
{
   if (-not (Test-path $InstallFolder) )
   {
      Write-Output "`nFolder $InstallFolder not found, creating..."
      New-Item -ItemType Directory -Force -Path $InstallFolder
   }
   Set-Location $InstallFolder

   # Should we overwrite existing?
   $codeql_folder = Join-Path $InstallFolder "codeql"
   if (Test-Path $codeql_folder)
   {
      if (-not $ForceOverwrite)
      {
         throw "Folder $codeql_folder already exists. If you want to overwrite this folder, use the -ForceOverwite switch.`n" 
      }
   }

   # Download the CodeQL CLI bundle for current platform
   if ($PSVersionTable.Platform -eq "Unix")
   {
      $filename = "codeql-bundle-linux64.tar.gz"
   }
   else {
      $filename = "codeql-bundle-win64.tar.gz"
   }
   $zip_path = Join-Path $InstallFolder $filename
   $codeql_url = "https://github.com/github/codeql-action/releases/latest/download/$filename"
   Write-Output "`nDownloading $codeql_url to $InstallFolder/codeql...`n" 
   Invoke-WebRequest -Uri $codeql_url -OutFile "$InstallFolder/$filename"

   # Extract the CodeQL CLI bundle
   try
   {
      if (Test-Path $codeql_folder)
      {
         # We manually delete, because on Windows the tar command doesn't support --overwrite
         Write-Output "`nRemoving previous installation from $codeql_folder"
         Remove-Item $codeql_folder -Recurse 
      }
      Write-Output "`nExtacting $filename...`n" 
      tar -xf "$zip_path"   
   }
   catch{
      throw "TAR extraction failed, if you're on an oder version of Windows you'll need to use WinRAR or 7-Zip to extract. $_"
   }  

   Write-Output "`n`nCodeQL installed to $InstallFolder/codeql`n" 
}
catch
{
   Write-Error "$_"
}
finally
{
   if (Test-Path $zip_path)
   {
      Remove-Item $zip_path
   }
   Set-Location $saved_location
}
