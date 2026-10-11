// Spellcross Reloaded: deterministic quarter-turns of flat isometric L1 tiles.
//
// A native Spellcross map stores an eight-character sprite name, not bitmap
// pixels. Virtual names R0Axxxxx encode (original terrain sprite index * 4 +
// orientation). The same algorithm MUST ship in both the standalone editor and
// the playable game; neither changes the user's original *.FS archives.
//
// Only flat terrain sprites with y offset 0 and slope 'A' are eligible.
// This deliberately excludes houses, animated objects and sloped ground.
#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace spellcross_flat_rotation
{
    constexpr int kWidth = 80;
    constexpr int kHeight = 48;
    constexpr unsigned kMaximumSourceIndex = 0x3FFFFu;

    inline bool DecodeName(const std::string& name, unsigned& originalIndex, int& rotation)
    {
        if (name.size() != 8 || name.compare(0, 3, "R0A") != 0)
            return false;
        unsigned code = 0;
        for (size_t i = 3; i < 8; ++i)
        {
            const char ch = name[i];
            const int hex = (ch >= '0' && ch <= '9') ? ch - '0'
                : ((ch >= 'A' && ch <= 'F') ? ch - 'A' + 10 : -1);
            if (hex < 0)
                return false;
            code = (code << 4) | static_cast<unsigned>(hex);
        }
        originalIndex = code >> 2;
        rotation = static_cast<int>(code & 3);
        return rotation != 0;
    }

    inline bool IsVirtualName(const std::string& name)
    {
        unsigned originalIndex = 0;
        int rotation = 0;
        return DecodeName(name, originalIndex, rotation);
    }

    inline std::string EncodeName(unsigned originalIndex, int rotation)
    {
        if (originalIndex > kMaximumSourceIndex || rotation < 1 || rotation > 3)
            return {};
        char name[9] = {};
        std::snprintf(name, sizeof(name), "R0A%05X",
                      static_cast<unsigned>((originalIndex << 2) | rotation));
        return name;
    }

    inline bool IsFlatSprite(Sprite* sprite)
    {
        return sprite && sprite->land_type != 0 &&
            sprite->name.size() >= 3 && sprite->name[2] == 'A' &&
            sprite->x_size == kWidth &&
            sprite->x_ofs == 0 && sprite->y_ofs == 0 &&
            sprite->y_size >= 1 && sprite->y_size <= kHeight &&
            !sprite->is_dummy;
    }

    // Create the actual palette-indexed 80x48 diamond. Rotation is performed
    // in *isometric world space*, rather than a naïve on-screen 90-degree
    // bitmap rotation (which would turn the diamond sideways).
    //
    // Horizontal world axis: 40 image pixels. Vertical world axis: 24.
    // Source coordinates are sampled inversely using nearest neighbour.
    inline Sprite* Generate(Sprite* source, unsigned baseIndex, int rotation)
    {
        if (!IsFlatSprite(source) || baseIndex > kMaximumSourceIndex ||
            rotation < 1 || rotation > 3)
            return nullptr;

        const std::string name = EncodeName(baseIndex, rotation);
        if (name.empty())
            return nullptr;

        std::array<uint8_t, kWidth * kHeight> sourcePixels{};
        for (int y = 0; y < kHeight; ++y)
            for (int x = 0; x < kWidth; ++x)
                if (auto* pixel = source->GetPixel(x, y))
                    sourcePixels[y * kWidth + x] = *pixel;

        auto* rotated = new Sprite(*source);
        rotated->name = name;
        rotated->x_ofs = 0;
        rotated->y_ofs = 0;
        rotated->y_size = kHeight;
        rotated->x_size = kWidth;
        rotated->is_dummy = false;
        rotated->is_removed = false;
        rotated->data.clear();
        rotated->data.reserve(kHeight * (8 + kWidth));

        auto append32 = [&](std::int32_t value)
        {
            const auto* bytes = reinterpret_cast<const uint8_t*>(&value);
            rotated->data.insert(rotated->data.end(), bytes, bytes + sizeof(value));
        };

        for (int y = 0; y < kHeight; ++y)
        {
            append32(0);       // row left offset
            append32(kWidth);  // row pixel count
            for (int x = 0; x < kWidth; ++x)
            {
                const double nx = (static_cast<double>(x) + 0.5 - 40.0) / 40.0;
                const double ny = (static_cast<double>(y) + 0.5 - 24.0) / 24.0;
                double sx = nx, sy = ny;
                if (rotation == 1) { sx = ny;  sy = -nx; }
                if (rotation == 2) { sx = -nx; sy = -ny; }
                if (rotation == 3) { sx = -ny; sy = nx; }
                const int ix = static_cast<int>(std::floor(40.0 + sx * 40.0));
                const int iy = static_cast<int>(std::floor(24.0 + sy * 24.0));
                uint8_t color = 0;
                if (ix >= 0 && ix < kWidth && iy >= 0 && iy < kHeight)
                    color = sourcePixels[iy * kWidth + ix];
                rotated->data.push_back(color);
            }
        }
        return rotated;
    }
}
