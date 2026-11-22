# Build and optionally publish the standalone Windows x64 map editor.
# Example: .\packaging\Build-EditorRelease.ps1 -Version 0.1.0 -WxRoot C:\vcpkg\installed\x64-windows
# Example: .\packaging\Build-EditorRelease.ps1 -Version 0.1.0 -Publish
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern('^[0-9]+\.[0-9]+\.[0-9]+(?:\.[0-9]+)?(?:-(?:alpha|beta|rc)[0-9]+)?$')]
    [string]$Version,
    [string]$WxRoot = '',
    [string]$OutputDir = '',
    [switch]$SkipBuild,
    [switch]$Publish
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
if ([Environment]::OSVersion.Platform -ne [PlatformID]::Win32NT) {
    throw 'Windows and MSVC are required.'
}
if ($Publish -and $SkipBuild) { throw '-Publish requires a fresh build; omit -SkipBuild.' }

$root = (Resolve-Path -LiteralPath (Join-Path $PSScriptRoot '..')).Path
$project = Join-Path $root 'source\spellcross_map_editor.sln'
$exe = Join-Path $root 'builds\x64\Release\spellcross_map_editor.exe'
$tool = Join-Path $root 'source\tools\render_unit_model.py'
$assets = Join-Path $root 'data'
$repo = 'luboshorak/spellcross-reloaded-editor'
$tag = "editor-v$Version"
$archiveName = "Spellcross_Map_Editor_v$($Version)_Windows_x64.zip"

function Assert-File([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "Missing required file: $Path"
    }
}
function Find-Tool([string]$Name, [string]$VsWhereGlob) {
    $vswhere = Join-Path ([Environment]::GetFolderPath('ProgramFilesX86')) 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path -LiteralPath $vswhere -PathType Leaf) {
        foreach ($candidate in @(& $vswhere -latest -products '*' -find $VsWhereGlob 2>$null)) {
            if ($candidate -and (Test-Path -LiteralPath $candidate -PathType Leaf)) {
                return $candidate
            }
        }
    }
    $cmd = Get-Command $Name -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    return $null
}

Assert-File $project
Assert-File $tool
Assert-File (Join-Path $root 'LICENSE')
$assetFiles = @('COMMON.fs', 'DEVAST.con', 'DEVAST.fs', 'PUST.con',
                'PUST.fs', 'T11.FS', 'T11.con', 'font_spellcross_7pix.fnt',
                'font_spellcross_14pix.fnt', 'units.fsa')
foreach ($f in $assetFiles) { Assert-File (Join-Path $assets $f) }
if (-not (Test-Path -LiteralPath (Join-Path $assets 'tree_randomizer') -PathType Container)) {
    throw 'Required editor data/tree_randomizer folder is missing.'
}

if (-not $WxRoot) {
    foreach ($candidate in @($env:wxwin, (Join-Path $HOME 'vcpkg\installed\x64-windows'))) {
        if ($candidate -and (Test-Path -LiteralPath $candidate -PathType Container)) {
            $WxRoot = $candidate
            break
        }
    }
}
if (-not $WxRoot) {
    throw 'wxWidgets root not found. Set wxwin or specify -WxRoot.'
}
$WxRoot = (Resolve-Path -LiteralPath $WxRoot).Path -replace '[\\/]+$', ''
$wxBin = Join-Path $WxRoot 'bin'
if (-not (Test-Path -LiteralPath $wxBin -PathType Container)) {
    throw "Expected wxWidgets/vcpkg runtime DLL directory not found: $wxBin"
}
$msbuild = Find-Tool 'MSBuild.exe' 'MSBuild\Current\Bin\MSBuild.exe'
$dumpbin = Find-Tool 'dumpbin.exe' 'VC\Tools\MSVC\*\bin\Hostx64\x64\dumpbin.exe'
if (-not $dumpbin) { throw 'dumpbin.exe not found. Install Visual Studio C++ tools.' }
if (-not $SkipBuild -and -not $msbuild) { throw 'MSBuild.exe not found.' }

# Publishing creates a tag/release in the EDITOR repository; it never pushes
# any game commits and it never modifies the Spellcross Reloaded submodule.
if ($Publish) {
    foreach ($name in @('git.exe', 'gh.exe')) {
        if (-not (Get-Command $name -ErrorAction SilentlyContinue)) {
            throw "$name is required to publish an editor-only GitHub Release."
        }
    }
    $branch = (& git -C $root branch --show-current).Trim()
    if ($LASTEXITCODE -ne 0 -or $branch -ne 'main') {
        throw 'Publishing is allowed only from the editor repository main branch.'
    }
    $dirty = @(& git -C $root status --porcelain --untracked-files=normal)
    if ($LASTEXITCODE -ne 0 -or $dirty.Count -gt 0) {
        throw 'The editor repository must have a clean working tree before publishing.'
    }
    $sha = (& git -C $root rev-parse HEAD).Trim()
    if ($LASTEXITCODE -ne 0) { throw 'Cannot determine source revision.' }
    $remote = @(& git -C $root ls-remote origin refs/heads/main)
    if ($LASTEXITCODE -ne 0 -or $remote.Count -ne 1 -or $remote[0] -notmatch "^$sha\s") {
        throw 'Push the editor main branch first. Local HEAD must equal origin/main.'
    }
    $tagExists = @(& git -C $root ls-remote --tags origin "refs/tags/$tag")
    if ($LASTEXITCODE -ne 0) { throw 'Unable to check editor tags.' }
    if ($tagExists.Count -gt 0) { throw "Tag $tag exists already. Choose a new version." }
}

if (-not $SkipBuild) {
    Write-Host 'Rebuilding standalone map editor (Release|x64)...' -ForegroundColor Cyan
    & $msbuild $project '/m' '/t:Rebuild' '/p:Configuration=Release' '/p:Platform=x64' ("/p:wxwin=$WxRoot\") '/v:minimal' '/nologo'
    if ($LASTEXITCODE -ne 0) { throw 'MSBuild failed. No release was created.' }
}
Assert-File $exe

if (-not $OutputDir) { $OutputDir = Join-Path $root 'dist' }
$OutputDir = [System.IO.Path]::GetFullPath($OutputDir)
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
$stage = Join-Path $OutputDir "_stage_editor_v$Version"
$zip = Join-Path $OutputDir $archiveName
if (Test-Path -LiteralPath $stage) { Remove-Item -LiteralPath $stage -Recurse -Force }
New-Item -ItemType Directory -Path $stage -Force | Out-Null
$stageData = Join-Path $stage 'data'
New-Item -ItemType Directory -Path $stageData -Force | Out-Null
Copy-Item -LiteralPath $exe -Destination (Join-Path $stage 'spellcross_map_editor.exe')
Copy-Item -LiteralPath $tool -Destination (Join-Path $stage 'render_unit_model.py')
Copy-Item -LiteralPath (Join-Path $root 'LICENSE') -Destination (Join-Path $stage 'LICENSE.txt')
foreach ($f in $assetFiles) {
    Copy-Item -LiteralPath (Join-Path $assets $f) -Destination (Join-Path $stageData $f)
}
Copy-Item -LiteralPath (Join-Path $assets 'tree_randomizer') -Destination $stageData -Recurse

# Resolve transitive dependencies only from the wxWidgets/vcpkg runtime.
# Never copy DLLs from an installed Spellcross Reloaded game.
$copiedDlls = New-Object 'System.Collections.Generic.HashSet[string]' ([System.StringComparer]::OrdinalIgnoreCase)
function Copy-ImportedDlls([string]$InitialBinary) {
    $queue = New-Object 'System.Collections.Generic.Queue[string]'
    $visited = New-Object 'System.Collections.Generic.HashSet[string]' ([System.StringComparer]::OrdinalIgnoreCase)
    $queue.Enqueue($InitialBinary)
    while ($queue.Count -gt 0) {
        $binary = $queue.Dequeue()
        if (-not $visited.Add($binary)) { continue }
        $imports = & $dumpbin '/nologo' '/dependents' $binary 2>&1
        if ($LASTEXITCODE -ne 0) { throw "dumpbin.exe failed for $binary" }
        foreach ($line in $imports) {
            if ([string]$line -notmatch '^\s*([A-Za-z0-9_.-]+\.dll)\s*$') { continue }
            $dllName = $Matches[1]
            $source = Join-Path $wxBin $dllName
            if (Test-Path -LiteralPath $source -PathType Leaf) {
                Copy-Item -LiteralPath $source -Destination (Join-Path $stage $dllName) -Force
                [void]$copiedDlls.Add($dllName)
                $queue.Enqueue($source)
            } elseif ($dllName -match '^wx') {
                throw "Missing wxWidgets DLL $dllName under $wxBin"
            }
            # Other dependencies are system DLLs / the Visual C++ runtime.
        }
    }
}
Copy-ImportedDlls (Join-Path $stage 'spellcross_map_editor.exe')

$licenseDir = Join-Path $stage 'third-party-licenses'
New-Item -ItemType Directory -Path $licenseDir -Force | Out-Null
$wxLicense = Join-Path $WxRoot 'share\wxwidgets\copyright'
if (Test-Path -LiteralPath $wxLicense -PathType Leaf) {
    Copy-Item -LiteralPath $wxLicense -Destination (Join-Path $licenseDir 'wxwidgets.txt')
}
$vcpkgInfo = Join-Path $WxRoot 'vcpkg\info'
if (Test-Path -LiteralPath $vcpkgInfo -PathType Container) {
    foreach ($manifest in (Get-ChildItem -LiteralPath $vcpkgInfo -Filter '*_x64-windows.list' -File)) {
        $providesDll = $false
        foreach ($line in [System.IO.File]::ReadLines($manifest.FullName)) {
            if ($line -match '/bin/([^/]+\.dll)$' -and $copiedDlls.Contains($Matches[1])) {
                $providesDll = $true
                break
            }
        }
        if (-not $providesDll) { continue }
        if ($manifest.BaseName -notmatch '^(.+?)_[0-9]') { continue }
        $port = $Matches[1]
        $notice = Join-Path $WxRoot "share\$port\copyright"
        if (Test-Path -LiteralPath $notice -PathType Leaf) {
            Copy-Item -LiteralPath $notice -Destination (Join-Path $licenseDir "$port.txt") -Force
        } else { Write-Warning "No vcpkg copyright notice for $port" }
    }
}

$editorHash = (Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant()
$commit = 'unknown (non-Git build)'
if (Get-Command git.exe -ErrorAction SilentlyContinue) {
    $commitCandidate = & git -C $root rev-parse HEAD 2>$null
    if ($LASTEXITCODE -eq 0) { $commit = "$commitCandidate".Trim() }
}

@"
SPELLCROSS MAP EDITOR - STANDALONE WINDOWS X64
Version: $Version

Run spellcross_map_editor.exe from this extracted directory.
At first start choose your own original Spellcross files (COMMON.FS,
UNITS.FSU and TEXTS.FS), or import from a mounted CD/ISO with INSTALL.DTA.

The included data/COMMON.fs is auxiliary EDITOR data, NOT the retail
game COMMON.FS. No commercial game data or Reloaded game executable
are included. The editor stores its config.ini beside the EXE; back up
your existing config.ini before extracting a new version.

Optional 3D model import needs Blender 4.x/5.x.
Some PCs also need the x64 Visual C++ runtime:
https://aka.ms/vc14/vc_redist.x64.exe

Repository: https://github.com/$repo
"@ | Set-Content -LiteralPath (Join-Path $stage 'README-FIRST.txt') -Encoding UTF8

@"
Project: Spellcross Reloaded Map Editor (standalone)
Version label: $Version
Tag: $tag
Repository: https://github.com/$repo
Source commit: $commit
Editor EXE SHA256: $editorHash
Bundled wx/vcpkg DLLs: $($copiedDlls.Count)
No game executable, retail data, or SR repository changes.
"@ | Set-Content -LiteralPath (Join-Path $stage 'BUILD-INFO.txt') -Encoding UTF8

if (Test-Path -LiteralPath $zip -PathType Leaf) { Remove-Item -LiteralPath $zip -Force }
Compress-Archive -Path (Join-Path $stage '*') -DestinationPath $zip -CompressionLevel Optimal
$zipHash = (Get-FileHash -LiteralPath $zip -Algorithm SHA256).Hash.ToLowerInvariant()
$checksumFile = "$zip.sha256"
"$zipHash  $archiveName" | Set-Content -LiteralPath $checksumFile -Encoding ASCII
Remove-Item -LiteralPath $stage -Recurse -Force
Write-Host "Standalone editor ZIP: $zip" -ForegroundColor Green
Write-Host "SHA256: $zipHash" -ForegroundColor Green
Write-Host "Bundled wx/vcpkg DLLs: $($copiedDlls.Count)" -ForegroundColor Cyan

if ($Publish) {
    Write-Host "Publishing $tag in the EDITOR repository only..." -ForegroundColor Cyan
    & gh release create $tag $zip $checksumFile --repo $repo --target $commit --title "Spellcross Map Editor v$Version" --notes 'Standalone Windows x64 map editor. Original Spellcross data required. No Reloaded game files included.'
    if ($LASTEXITCODE -ne 0) { throw 'GitHub release creation failed; local ZIP was preserved.' }
    Write-Host "Published: https://github.com/$repo/releases/tag/$tag" -ForegroundColor Green
}
