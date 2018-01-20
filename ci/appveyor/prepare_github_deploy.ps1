#Script for preparing for GitHub deploy.

#For prerelease branches:
#  Every commits to a prerelease branch is tagged and binaries are deployed to GitHub.
#For non-prerelease branches:
#  Each other branches (including master) are build after each commits.
#  One version of the latest binaries is kept per branch (allow 1 public nightly build per branch)
#  A dummy tag is created named: nightly-$(BRANCH_NAME). This seems to be required by GitHub.
#  The version number of a non-prerelease branch is truncated from format x.y.z.b to format x.y.z instead.
#  This is to allow release binaries collisions which overrides the previous uploads allowing to only keep the last commits.
#  It also prevents collisions with actual releases of prerelease branches.


Write-Output "Running prepare_github_deploy.ps1..."
Write-Output "Current branch: $env:APPVEYOR_REPO_BRANCH"
Write-Output "Current build version: $env:APPVEYOR_BUILD_VERSION"
If ("$env:APPVEYOR_REPO_BRANCH".contains("prerelease")) {
  Write-Output "Tagging each commits with a unique version..."
  Write-Output "Deploying each commits to GitHub..."

  $env:GITHUB_TAG_NAME="v$env:APPVEYOR_BUILD_VERSION-win32"
  $env:GITHUB_RELEASE_NAME="$env:APPVEYOR_PROJECT_NAME-v$env:APPVEYOR_BUILD_VERSION-win32"
}
Else
{
  #Truncating APPVEYOR_BUILD_VERSION to x.y.z instead of x.y.z.b
  $major, $minor, $patch, $build = $env:APPVEYOR_BUILD_VERSION.Split(".")
  #Strip out build number from $env:APPVEYOR_BUILD_VERSION
  $env:APPVEYOR_BUILD_VERSION = "$major.$minor.$patch"

  Write-Output "Truncating APPVEYOR_BUILD_VERSION environment variable to '$env:APPVEYOR_BUILD_VERSION'"
  
  # allow 1 public nightly build per branch
  $env:GITHUB_TAG_NAME="nightly-$env:APPVEYOR_REPO_BRANCH"
  $env:GITHUB_RELEASE_NAME="Nightly build (branch $env:APPVEYOR_REPO_BRANCH)"
}

#Show final values...
Write-Output "APPVEYOR_BUILD_VERSION=$env:APPVEYOR_BUILD_VERSION"
Write-Output "GITHUB_TAG_NAME=$env:GITHUB_TAG_NAME"
Write-Output "GITHUB_RELEASE_NAME=$env:GITHUB_RELEASE_NAME"
