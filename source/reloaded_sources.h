// Spellcross Reloaded: portable data-source bootstrap for the upstream editor.
//
// Keeps resource discovery and original-CD installation separate from the
// upstream archive decoders. The INSTALL.DTA implementation is adapted from
// spellcross-reloaded/spellcross-map-edit-main/source/main.cpp (MIT).
#pragma once

#include "SimpleIni.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <system_error>
#include <vector>

namespace reloaded_sources {
namespace fs = std::filesystem;

inline bool IsFile(const fs::path& p) {
    std::error_code ec;
    return !p.empty() && fs::is_regular_file(p, ec) && !ec;
}

inline bool IsDir(const fs::path& p) {
    std::error_code ec;
    return !p.empty() && fs::is_directory(p, ec) && !ec;
}

inline std::string Lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

inline fs::path FileIn(const fs::path& dir, const std::string& wanted) {
    if (!IsDir(dir)) return {};
    // Fast case on the usual Windows filesystem.
    auto normal = dir / wanted;
    if (IsFile(normal)) return normal;
    std::error_code ec;
    for (const auto& item : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) break;
        if (item.is_regular_file(ec) && !ec &&
            Lower(item.path().filename().string()) == Lower(wanted)) return item.path();
        ec.clear();
    }
    return {};
}

inline std::vector<fs::path> Ancestors(fs::path path) {
    std::vector<fs::path> out;
    if (!IsDir(path)) path = path.parent_path();
    while (!path.empty()) {
        out.push_back(path);
        auto parent = path.parent_path();
        if (parent.empty() || parent == path) break;
        path = parent;
    }
    return out;
}

inline fs::path FindEditorAssets(const fs::path& exe_dir) {
    for (const auto& dir : Ancestors(exe_dir)) {
        for (const auto& c : {dir / "data", dir / "external" / "map-editor" / "data"}) {
            if (IsFile(c / "COMMON.fs") && IsFile(c / "units.fsa") &&
                IsFile(c / "font_spellcross_7pix.fnt")) return c;
        }
    }
    return {};
}

inline fs::path FindWorkspaceRoot(const fs::path& exe_dir) {
    for (const auto& dir : Ancestors(exe_dir))
        if (IsDir(dir / "spellcross-map-edit-main")) return dir;
    return exe_dir;
}

inline bool HasGameCommon(const fs::path& dir) {
    return !FileIn(dir, "COMMON.FS").empty() &&
           !FileIn(dir, "UNITS.FSU").empty() &&
           !FileIn(dir, "TEXTS.FS").empty();
}

inline fs::path FindGameData(const fs::path& exe_dir) {
    for (const auto& dir : Ancestors(exe_dir)) {
        const std::array<fs::path, 5> candidates = {
            dir / "game_data" / "DATA",
            dir / "spellcross-map-edit-main" / "game_data" / "DATA",
            dir / "DATA",
            dir / "data" / "DATA",
            dir / "spellcross-map-edit-main" / "DATA"
        };
        for (const auto& c : candidates) if (HasGameCommon(c)) return c;
    }
    return {};
}

inline fs::path FindOriginalIni(const fs::path& exe_dir) {
    for (const auto& dir : Ancestors(exe_dir)) {
        for (const auto& c : {
            dir / "spellcross-map-edit-main" / "source" / "config.ini",
            dir / "spellcross-map-edit-main" / "config.ini",
            dir / "config.ini"
        }) {
            if (IsFile(c) && c != (exe_dir / "config.ini")) return c;
        }
    }
    return {};
}

inline fs::path ResolveValue(const fs::path& ini_file, const char* value) {
    if (!value || !*value) return {};
    fs::path rel = fs::u8path(value);
    if (rel.is_absolute()) return rel.lexically_normal();
    auto a = ini_file.parent_path() / rel;
    if (IsFile(a) || IsDir(a)) return a.lexically_normal();
    // Reloaded configurations can store paths relative to the game runtime
    // root even when the INI was found inside its "source" subdirectory.
    auto b = ini_file.parent_path().parent_path() / rel;
    if (IsFile(b) || IsDir(b)) return b.lexically_normal();
    return a.lexically_normal();
}

struct Discovered {
    fs::path data_dir;
    fs::path cd_dir;
    std::vector<fs::path> extra_dirs;
};

inline void AddDir(std::vector<fs::path>& dirs, const fs::path& dir) {
    if (!IsDir(dir)) return;
    auto normalized = dir.lexically_normal();
    if (std::find(dirs.begin(), dirs.end(), normalized) == dirs.end()) dirs.push_back(normalized);
}

inline Discovered FromReloadedIni(const fs::path& ini_file) {
    Discovered result;
    if (!IsFile(ini_file)) return result;
    CSimpleIniA ini;
    ini.SetUnicode();
    if (ini.LoadFile(ini_file.wstring().c_str()) != SI_OK) return result;
    const auto base = ResolveValue(ini_file, ini.GetValue("SPELCROS", "spell_path", ""));
    const auto cd = ResolveValue(ini_file, ini.GetValue("SPELCROS", "spellcd_path", ""));
    if (HasGameCommon(base)) result.data_dir = base;
    if (IsDir(cd)) result.cd_dir = cd;

    const std::array<const char*, 13> keys = {
        "common_fs", "info_fs", "t11_fs", "pust_fs", "devast_fs",
        "units_fsu", "texts_fs", "samples_fs", "music_fs",
        "research_fs", "movie_fs", "speaker_fs", "movies_fs"
    };
    for (const auto* key : keys) {
        auto archive = ResolveValue(ini_file, ini.GetValue("FILES", key, ""));
        if (!IsFile(archive)) continue;
        AddDir(result.extra_dirs, archive.parent_path());
        if (std::string(key) == "common_fs" && result.data_dir.empty())
            result.data_dir = archive.parent_path();
        if (std::string(key) == "info_fs" && result.cd_dir.empty())
            result.cd_dir = archive.parent_path();
    }
    return result;
}

// Reloaded stores individual absolute/relative archives in [FILES].
// Keep them as direct references; a single "game folder" can no longer
// represent every installation layout.
inline std::map<std::string, fs::path>& ArchiveOverrides() {
    static std::map<std::string, fs::path> paths;
    return paths;
}

inline void ReadArchiveOverrides(const fs::path& ini_path) {
    if (!IsFile(ini_path)) return;
    CSimpleIniA ini;
    ini.SetUnicode();
    if (ini.LoadFile(ini_path.wstring().c_str()) != SI_OK) return;
    const std::array<std::pair<const char*, const char*>, 13> keys = {{
        {"common_fs", "COMMON.FS"},
        {"info_fs", "INFO.FS"},
        {"t11_fs", "T11.FS"},
        {"pust_fs", "PUST.FS"},
        {"devast_fs", "DEVAST.FS"},
        {"units_fsu", "UNITS.FSU"},
        {"texts_fs", "TEXTS.FS"},
        {"samples_fs", "SAMPLES.FS"},
        {"music_fs", "MUSIC.FS"},
        {"research_fs", "RESEARCH.FS"},
        {"movie_fs", "MOVIE.FS"},
        {"speaker_fs", "SPEAKER.FS"},
        {"movies_fs", "MOVIES.FS"}
    }};
    for (const auto& [key, name] : keys) {
        auto value = ResolveValue(ini_path, ini.GetValue("FILES", key, ""));
        if (IsFile(value) && Lower(value.filename().string()) == Lower(name))
            ArchiveOverrides()[Lower(name)] = value;
    }
}

inline fs::path FindOverriddenArchive(const std::string& filename) {
    auto it = ArchiveOverrides().find(Lower(filename));
    if (it == ArchiveOverrides().end() || !IsFile(it->second)) return {};
    return it->second;
}

inline fs::path FindInstallDta(const fs::path& selection) {
    auto p = FileIn(selection, "INSTALL.DTA");
    if (!p.empty()) return p;
    return FileIn(selection / "DATA", "INSTALL.DTA");
}

struct InstallEntry {
    std::string name;
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
    std::uint32_t dir_index = 0;
};

inline bool ReadU32(std::ifstream& in, std::uint32_t& v) {
    unsigned char b[4] = {};
    in.read(reinterpret_cast<char*>(b), 4);
    if (!in) return false;
    v = static_cast<std::uint32_t>(b[0]) |
        (static_cast<std::uint32_t>(b[1]) << 8) |
        (static_cast<std::uint32_t>(b[2]) << 16) |
        (static_cast<std::uint32_t>(b[3]) << 24);
    return true;
}

inline bool ReadInstallIndex(const fs::path& src, std::vector<std::string>& dirs,
                            std::vector<InstallEntry>& entries, std::string& error) {
    dirs.clear();
    entries.clear();
    std::ifstream in(src, std::ios::binary);
    if (!in) { error = "Cannot open INSTALL.DTA"; return false; }
    std::error_code ec;
    const std::uintmax_t total = fs::file_size(src, ec);
    if (ec || total < 16) { error = "Invalid INSTALL.DTA size"; return false; }

    std::uint32_t block = 0, n_dirs = 0;
    if (!ReadU32(in, block) || !ReadU32(in, n_dirs) ||
        block < 4 || block > 1024 * 1024 || n_dirs == 0 || n_dirs > 1024) {
        error = "Invalid INSTALL.DTA directory table";
        return false;
    }
    for (std::uint32_t i = 0; i < n_dirs; ++i) {
        std::string name;
        bool terminated = false;
        for (int j = 0; j < 4096; ++j) {
            char ch = 0;
            in.get(ch);
            if (!in) { error = "Truncated INSTALL.DTA directory table"; return false; }
            if (ch == '\0') { terminated = true; break; }
            name.push_back(ch);
        }
        if (!terminated) { error = "Invalid directory name"; return false; }
        dirs.push_back(name);
    }
    const std::uint64_t count_pos = 4ull + block;
    if (count_pos + 4 > total) { error = "Directory table outside file"; return false; }
    in.seekg(static_cast<std::streamoff>(count_pos), std::ios::beg);
    std::uint32_t n_files = 0;
    if (!ReadU32(in, n_files) || n_files == 0 || n_files > 100000 ||
        count_pos + 4ull + 25ull * n_files > total) {
        error = "Invalid INSTALL.DTA file table";
        return false;
    }
    entries.reserve(n_files);
    for (std::uint32_t i = 0; i < n_files; ++i) {
        char raw[13] = {};
        in.read(raw, 13);
        if (!in) { error = "Truncated file record"; return false; }
        InstallEntry e;
        auto end = std::find(raw, raw + 13, '\0');
        e.name.assign(raw, end);
        if (!ReadU32(in, e.offset) || !ReadU32(in, e.size) || !ReadU32(in, e.dir_index) ||
            e.name.empty() || e.dir_index >= dirs.size() ||
            static_cast<std::uint64_t>(e.offset) + e.size > total) {
            error = "Invalid INSTALL.DTA file record";
            return false;
        }
        entries.push_back(e);
    }
    return true;
}

inline bool SegmentMatches(std::ifstream& src, const InstallEntry& e, const fs::path& dst) {
    std::error_code ec;
    if (!IsFile(dst) || fs::file_size(dst, ec) != e.size || ec) return false;
    std::ifstream old(dst, std::ios::binary);
    if (!old) return false;
    src.clear();
    src.seekg(e.offset, std::ios::beg);
    std::vector<char> left(1024 * 1024), right(left.size());
    std::uint64_t remain = e.size;
    while (remain > 0) {
        size_t n = static_cast<size_t>(std::min<std::uint64_t>(remain, left.size()));
        src.read(left.data(), static_cast<std::streamsize>(n));
        old.read(right.data(), static_cast<std::streamsize>(n));
        if (!src || !old || !std::equal(left.begin(), left.begin() + n, right.begin()))
            return false;
        remain -= n;
    }
    return true;
}

// Import only the DOS installer's DATA directory. Do not overwrite a matching
// archive; use a temporary .partial file before replacing a changed copy.
inline bool ExtractInstallDtaDataFiles(const fs::path& src, const fs::path& target,
                                       std::vector<fs::path>& imported, std::string& error) {
    imported.clear();
    std::vector<std::string> dirs;
    std::vector<InstallEntry> files;
    if (!ReadInstallIndex(src, dirs, files, error)) return false;

    std::error_code ec;
    fs::create_directories(target, ec);
    if (ec) { error = "Cannot create destination: " + ec.message(); return false; }
    std::ifstream in(src, std::ios::binary);
    if (!in) { error = "Cannot reopen INSTALL.DTA"; return false; }

    std::vector<char> buf(1024 * 1024);
    for (const auto& e : files) {
        if (Lower(dirs[e.dir_index]) != "data") continue;
        if (e.name == "." || e.name == ".." ||
            e.name.find('/') != std::string::npos ||
            e.name.find('\\') != std::string::npos ||
            e.name.find(':') != std::string::npos) continue;
        auto dst = target / e.name;
        if (SegmentMatches(in, e, dst)) { imported.push_back(dst); continue; }
        in.clear();
        in.seekg(e.offset, std::ios::beg);
        if (!in) { error = "Cannot seek to " + e.name; return false; }
        auto tmp = dst;
        tmp += ".partial";
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
        if (!out) { error = "Cannot create " + tmp.string(); return false; }
        std::uint64_t remain = e.size;
        while (remain) {
            size_t n = static_cast<size_t>(std::min<std::uint64_t>(remain, buf.size()));
            in.read(buf.data(), static_cast<std::streamsize>(n));
            out.write(buf.data(), static_cast<std::streamsize>(n));
            if (!in || !out) {
                out.close();
                fs::remove(tmp, ec);
                error = "Failed extracting " + e.name;
                return false;
            }
            remain -= n;
        }
        out.close();
        fs::remove(dst, ec);
        ec.clear();
        fs::rename(tmp, dst, ec);
        if (ec) {
            fs::remove(tmp, ec);
            error = "Cannot finalize " + dst.string();
            return false;
        }
        imported.push_back(dst);
    }
    if (imported.empty()) { error = "No DATA payload in INSTALL.DTA"; return false; }
    return true;
}

inline void CopyCdSidecar(const fs::path& cd_data_dir, const fs::path& target,
                          const std::string& name, const std::string& alias = "") {
    const auto src = FileIn(cd_data_dir, name);
    if (src.empty()) return;
    std::error_code ec;
    fs::copy_file(src, target / (alias.empty() ? name : alias),
                  fs::copy_options::overwrite_existing, ec);
}

} // namespace reloaded_sources
