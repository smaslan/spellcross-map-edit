// Reused from Spellcross Reloaded's tactical camera implementation (MIT).
// Keep camera physics consistent with the main game.
#pragma once

// Standalone tactical edge-pan math. No wxWidgets dependency: camera motion
// must depend on elapsed time, never on the number of mouse-move messages.
#include <algorithm>
#include <cmath>
#include <utility>

namespace spellcross_tactical
{
    struct EdgePanDirection
    {
        double x = 0.0;
        double y = 0.0;
    };

    // Query physical arrow-key state once per timer tick. This is independent
    // of OS key-repeat speed and works even after a HUD button takes focus.
    // Opposing arrows cancel on their own axis.
    inline EdgePanDirection KeyboardPanDirection(bool left, bool right, bool up, bool down)
    {
        return { static_cast<double>(static_cast<int>(right) - static_cast<int>(left)),
            static_cast<double>(static_cast<int>(down) - static_cast<int>(up)) };
    }

    // The far edge is exclusive. Constrain the hot zone on small viewports
    // so opposing edges never overlap and the centre always stays neutral.
    inline double EdgePanAxis(int position, int begin, int end, int zone)
    {
        if (position < begin || position >= end || zone <= 0 || end <= begin + 2)
            return 0.0;
        const int active = (std::min)(zone, (end - begin) / 3);
        if (active <= 0)
            return 0.0;

        const int fromNear = position - begin;
        if (fromNear < active)
        {
            const double t = static_cast<double>(active - fromNear) / active;
            return -t * (2.0 - t);
        }
        const int fromFar = end - position - 1;
        if (fromFar < active)
        {
            const double t = static_cast<double>(active - fromFar) / active;
            return t * (2.0 - t);
        }
        return 0.0;
    }

    // Edge detection uses the *whole native window*, not the map canvas.
    // This deliberately includes the top menu/title area and the HUD below
    // the battlefield. Actual menu interactions are gated by the caller.
    inline EdgePanDirection EdgePanAt(int x, int y, int width, int height, int zone)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return {};
        return { EdgePanAxis(x, 0, width, zone),
            EdgePanAxis(y, 0, height, zone) };
    }

    // Accept global screen coordinates, including monitors left/up from (0,0).
    // The cursor and frame bounds must use the same coordinate system.
    inline EdgePanDirection EdgePanAtScreen(int mouseX, int mouseY,
        int windowLeft, int windowTop, int windowWidth, int windowHeight, int zone)
    {
        return EdgePanAt(mouseX - windowLeft, mouseY - windowTop,
            windowWidth, windowHeight, zone);
    }

    class EdgePanAccumulator
    {
        double m_xFraction = 0.0;
        double m_yFraction = 0.0;

        static int Integrate(double direction, double& fraction,
            double seconds, double logicalPixelsPerScreenPixel, double screenPixelsPerSecond)
        {
            if (direction == 0.0)
            {
                fraction = 0.0;
                return 0;
            }
            // A direction reversal must not inherit the opposite direction's
            // fractional remainder, which otherwise feels like sticky edges.
            if (fraction * direction < 0.0)
                fraction = 0.0;
            const double distance = fraction + direction * seconds *
                logicalPixelsPerScreenPixel * screenPixelsPerSecond;
            // Avoid a lost pixel at exact integer boundaries caused by
            // binary floating-point rounding (e.g. 0.48 * 100).
            const double rounding = distance >= 0.0 ? 1e-9 : -1e-9;
            const int whole = static_cast<int>(std::trunc(distance + rounding));
            fraction = distance - whole;
            return whole;
        }

    public:
        void Reset() { m_xFraction = m_yFraction = 0.0; }

        std::pair<int, int> Advance(EdgePanDirection direction, double elapsedSeconds,
            double logicalPixelsPerScreenPixel, double maxScreenPixelsPerSecond = 1600.0)
        {
            // Do not catch up several seconds after a breakpoint, modal dialog,
            // window drag, or unexpectedly slow paint.
            const double seconds = (std::clamp)(elapsedSeconds, 0.0, 0.05);
            return {
                Integrate(direction.x, m_xFraction, seconds,
                    logicalPixelsPerScreenPixel, maxScreenPixelsPerSecond),
                Integrate(direction.y, m_yFraction, seconds,
                    logicalPixelsPerScreenPixel, maxScreenPixelsPerSecond)
            };
        }
    };
}
