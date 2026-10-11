// UI-side portable startup, kept separate from Mašláň's map editor engine.
// The imported data is owned by the user; none of it is committed to Git.
#pragma once
#include "reloaded_sources.h"
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>

namespace reloaded_bootstrap {
namespace fs = std::filesystem;
inline fs::path ConfigFile(const fs::path& executable_dir) {
    return executable_dir / "config.ini";
}
inline bool Prepare(CSimpleIniA& ini, const fs::path& executable_dir,
                    fs::path& data_dir, fs::path& cd_dir,
                    fs::path& assets_dir, fs::path& mod_dir) {
    using namespace reloaded_sources;
    assets_dir = FindEditorAssets(executable_dir);
    if (assets_dir.empty()) {
        wxMessageBox("Editor support data are missing. Expected data/COMMON.fs, "
                     "data/units.fsa and editor fonts near the executable or "
                     "inside the checked-out editor repository.",
                     "Spellcross Reloaded Editor", wxOK | wxICON_ERROR);
        return false;
    }

    // First respect what the user configured for the editor itself.
    const auto config = ConfigFile(executable_dir);
    const fs::path configured = ResolveValue(config, ini.GetValue("SPELCROS", "spell_path", ""));
    const fs::path configured_cd = ResolveValue(config, ini.GetValue("SPELCROS", "spellcd_path", ""));
    const fs::path configured_mod = ResolveValue(config, ini.GetValue("SPELCROS", "mod_path", ""));
    if (HasGameCommon(configured)) data_dir = configured;
    if (IsDir(configured_cd)) cd_dir = configured_cd;
    if (IsDir(configured_mod)) mod_dir = configured_mod;

    // Fall back to Reloaded's existing sources and its individual archive paths.
    if (data_dir.empty()) {
        const auto root = FindWorkspaceRoot(executable_dir);
        for (const auto& old_ini : {
                 root / "spellcross-map-edit-main" / "source" / "config.ini",
                 root / "spellcross-map-edit-main" / "config.ini",
                 root / "config.ini" }) {
            const auto found = FromReloadedIni(old_ini);
            if (!found.data_dir.empty()) { data_dir = found.data_dir; cd_dir = found.cd_dir; break; }
            // Separate per-file roots can be supported only when the required
            // archives can actually be found in a single compatible root.
            for (const auto& candidate : found.extra_dirs)
                if (HasGameCommon(candidate)) {
                    data_dir = candidate;
                    if (cd_dir.empty()) cd_dir = found.cd_dir;
                    break;
                }
            if (!data_dir.empty()) break;
        }
    }
    if (data_dir.empty()) data_dir = FindGameData(executable_dir);

    if (data_dir.empty()) {
        const int choice = wxMessageBox(
            "No Spellcross installation data were found.\n\n"
            "YES: Import from original game CD / mounted ISO "
            "(choose its root folder or DATA folder).\n"
            "NO: Select an existing folder containing COMMON.FS.\n"
            "CANCEL: Exit without changing game data.",
            "Spellcross game data", wxYES_NO | wxCANCEL | wxICON_QUESTION);
        if (choice == wxCANCEL) return false;
        wxDirDialog dialog(nullptr, choice == wxYES ?
            "Select original Spellcross CD root or DATA folder" :
            "Select the folder containing COMMON.FS", "",
            wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
        if (dialog.ShowModal() != wxID_OK) return false;
        const fs::path selected(dialog.GetPath().ToStdWstring());
        if (choice == wxYES) {
            auto install = FindInstallDta(selected);
            if (install.empty()) {
                wxMessageBox("INSTALL.DTA was not found in the selected folder or its DATA subfolder.",
                             "Spellcross import", wxOK | wxICON_ERROR);
                return false;
            }
            // User data are installed alongside the Reloaded workspace, never
            // into source control or into the Mašláň editor's auxiliary data.
            const fs::path destination = FindWorkspaceRoot(executable_dir) / "game_data" / "DATA";
            std::vector<fs::path> imported;
            std::string error;
            if (!ExtractInstallDtaDataFiles(install, destination, imported, error)) {
                wxMessageBox(wxString::FromUTF8(error), "Spellcross import", wxOK | wxICON_ERROR);
                return false;
            }
            CopyCdSidecar(install.parent_path(), destination, "INFO.FS");
            CopyCdSidecar(install.parent_path(), destination, "MOVIE.FS");
            CopyCdSidecar(install.parent_path(), destination, "MOVIES.FS", "MOVIE.FS");
            CopyCdSidecar(install.parent_path(), destination, "SPEAKER.FS");
            data_dir = destination;
            cd_dir = destination;
        } else {
            data_dir = selected;
            if (!HasGameCommon(data_dir)) {
                wxMessageBox("Selected folder does not contain COMMON.FS.",
                             "Spellcross game data", wxOK | wxICON_ERROR);
                return false;
            }
        }
    }

    if (cd_dir.empty()) cd_dir = data_dir;
    if (mod_dir.empty()) mod_dir.clear();
    if (ini.GetValue("STATE", "last_map", nullptr) == nullptr)
        ini.SetValue("STATE", "last_map", "");
    ini.SetValue("SPELCROS", "spell_path", data_dir.string().c_str());
    ini.SetValue("SPELCROS", "spellcd_path", cd_dir.string().c_str());
    if (!mod_dir.empty())
        ini.SetValue("SPELCROS", "mod_path", mod_dir.string().c_str());
    // An absolute path is intentional: std::filesystem::path / absolute path
    // discards the left operand in the legacy FormLoader.
    ini.SetValue("DATA", "spec_data_path", assets_dir.string().c_str());
    ini.SetValue("DATA", "units_aux_data_path", (assets_dir / "units.fsa").string().c_str());
    for (const auto* terrain : {"T11", "PUST", "DEVAST"}) {
        const auto key = std::string("TERRAIN::") + terrain;
        const fs::path ctx = assets_dir / (std::string(terrain) + ".con");
        if (IsFile(ctx)) ini.SetValue(key.c_str(), "context_path", ctx.string().c_str());
    }
    if (ini.SaveFile(config.wstring().c_str()) != SI_OK) {
        wxMessageBox("Cannot save editor configuration alongside its executable.",
                     "Spellcross configuration", wxOK | wxICON_ERROR);
        return false;
    }
    return true;
}
} // namespace reloaded_bootstrap
