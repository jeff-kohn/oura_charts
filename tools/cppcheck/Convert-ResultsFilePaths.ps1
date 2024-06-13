# CppCheck writes FQ/absolute paths to the scan results, which presents GHAS
# from showing the code when viewing the scan alerts on the GH website. This
# script just conversts the sourcefile paths to be repo-relative.
# Right now this script only works with path elements that contain backslashes,
# not forward slashes.

param(
    [string]$ResultsFile
)

# we want the path of the repo, eg ~/repo_name
$RepoDir = Split-Path( Split-Path $PSScriptRoot )

# need to remove path separator between repodir and subfolders also.
$RepoDir = $RepoDir + '\'

if (Test-Path $ResultsFile)
{
    $content = Get-Content $ResultsFile
    $content = $content -replace [regex]::escape($RepoDir), ""
    Set-Content -Path $ResultsFile -Value $content

    Write-Host "`n`nFile $ResultsFile succcessfully updated`n`n." -ForegroundColor Cyan
}
else
{
    Write-Error "`n`nFile $ResultsFile was not found"
}

