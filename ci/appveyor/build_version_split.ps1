Write-Output "Splitting current build version: $env:APPVEYOR_BUILD_VERSION"
$major, $minor, $patch, $build = $env:APPVEYOR_BUILD_VERSION.Split(".")

#Define each fields as independent environment variables.
$env:BUILD_VERSION_MAJOR = "$major"
$env:BUILD_VERSION_MINOR = "$minor"
$env:BUILD_VERSION_PATCH = "$patch"
$env:BUILD_VERSION_BUILD = "$build"

#Show output variables...
Write-Output "BUILD_VERSION_MAJOR=$env:BUILD_VERSION_MAJOR"
Write-Output "BUILD_VERSION_MINOR=$env:BUILD_VERSION_MINOR"
Write-Output "BUILD_VERSION_PATCH=$env:BUILD_VERSION_PATCH"
Write-Output "BUILD_VERSION_BUILD=$env:BUILD_VERSION_BUILD"
