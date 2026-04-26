param(
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$outDir = Join-Path $root "dist"
$packageDir = Join-Path $outDir "CamelotPad"

New-Item -ItemType Directory -Force -Path $packageDir | Out-Null

Copy-Item -Force -Path (Join-Path $root "camelotpad-lua") -Destination $packageDir -Recurse
Copy-Item -Force -Path (Join-Path $root "camelotpad-shared") -Destination $packageDir -Recurse
Copy-Item -Force -Path (Join-Path $root "docs") -Destination $packageDir -Recurse
Copy-Item -Force -Path (Join-Path $root "README.md") -Destination $packageDir

Write-Host "Package assembled at: $packageDir"
