# Standalone Spellcross Map Editor releases

These instructions apply **only** to
[luboshorak/spellcross-reloaded-editor](https://github.com/luboshorak/spellcross-reloaded-editor).
Do not use the game repository's combined build or release scripts.

## Prerequisites

- Windows x64 with Visual Studio C++ build tools, MSBuild, the **v145** platform toolset and Windows SDK.
- Compatible wxWidgets installed through vcpkg (or a layout with the same x64 headers, libraries and runtime DLLs). Provide the installation root using -WxRoot or the wxwin environment variable.
- PowerShell 5.1+ and enough disk space for an x64 Release rebuild.
- To publish to GitHub: Git, the GitHub CLI (gh), and an authenticated account with write permissions on the **editor** repository.

For example, if your wxWidgets/vcpkg root is C:\vcpkg\installed\x64-windows,
its runtime DLLs should be in C:\vcpkg\installed\x64-windows\bin.

## Build a local editor-only package

From the editor repository root:

```powershell
.\packaging\Build-EditorRelease.ps1 -Version 0.1.0 -WxRoot 'C:\vcpkg\installed\x64-windows'
```

This builds **only** source/spellcross_map_editor.sln using
Release|x64. It does not invoke Spellcross Reloaded's build scripts,
change the game's source, update the game's submodule or create a game release.

To repackage an already built editor without rebuilding:

```powershell
.\packaging\Build-EditorRelease.ps1 -Version 0.1.0 -WxRoot 'C:\vcpkg\installed\x64-windows' -SkipBuild
```

-SkipBuild is intended for local testing; it does **not** verify that the
previously built EXE matches the version label. It cannot be used with -Publish.

The generated assets are:

```text
dist/
  Spellcross_Map_Editor_v0.1.0_Windows_x64.zip
  Spellcross_Map_Editor_v0.1.0_Windows_x64.zip.sha256
```

The ZIP contains a self-contained editor folder:

```text
spellcross_map_editor.exe
render_unit_model.py
data/
  COMMON.fs
  units.fsa
  DEVAST.con, DEVAST.fs
  PUST.con, PUST.fs
  T11.con, T11.FS
  font_spellcross_7pix.fnt
  font_spellcross_14pix.fnt
  tree_randomizer/
required wxWidgets/vcpkg DLLs
third-party-licenses/
LICENSE.txt
BUILD-INFO.txt
README-FIRST.txt
```

No Spellcross.exe, user config.ini, user mods or savegames, downloaded
commercial game assets, or original retail archives are included. The file
data/COMMON.fs **belongs to the editor support data** and is not the retail
game's COMMON.FS archive.

Extract to a writable directory and run spellcross_map_editor.exe. The
editor's startup bootstrap looks for its own data/ files, writes a fresh
config.ini and asks for original game data if it cannot find a valid path.
A mounted original Spellcross CD/ISO can be selected for the standalone
INSTALL.DTA import. Back up config.ini before replacing an existing editor.

The ZIP includes optional render_unit_model.py, but converting new 3D unit
models additionally requires locally installed Blender.

## Publish a GitHub Release of the editor only

After testing the extracted ZIP on Windows, switch to the editor's **main**
branch, commit/push all editor changes, and ensure your worktree is clean.
Then run:

```powershell
git switch main
git pull --ff-only
.\packaging\Build-EditorRelease.ps1 -Version 0.1.0 -WxRoot 'C:\vcpkg\installed\x64-windows' -Publish
```

-Publish verifies that local main equals origin/main and rejects an existing
editor release tag. It creates the tag **editor-v0.1.0** and publishes the
ZIP and SHA256 file under:

https://github.com/luboshorak/spellcross-reloaded-editor/releases

It does **not** commit, push, merge or update anything in
luboshorak/spellcross-reloaded. Tags are intentionally prefixed editor-v
so they cannot be mistaken for game release tags.

**Important:** GitHub publication is explicit. Without -Publish, the script
only creates local files. There is no automatic push or release.

## Release validation

Before publishing, unpack the archive **outside the repository** and verify
that the editor opens without a local game checkout, prompts for original
game data, can open a .DEF/.DTA mission, save to a new file and reopen it.
Verify any optional Blender model conversion you intend to advertise.

If the build fails, check that the v145 toolset and matching wxWidgets x64
libraries are installed. If a wxWidgets DLL is missing, pass the correct
-WxRoot containing the matching bin/ folder. On a clean Windows machine,
the Visual C++ Redistributable x64 may still be needed.

The game repository has a separate automated submodule synchronization for
**editor source/data changes**. This editor packaging workflow deliberately
does not trigger that synchronization. Existing game release mechanics and
version numbers are unchanged.
