#include "../source/editor_edge_pan.h"
#include <cassert>
#include <cstdlib>

int main()
{
    using namespace spellcross_tactical;

    // Hot zones now use the complete native frame, including menu and HUD.
    assert(EdgePanAt(400, 200, 800, 480, 28).x == 0.0);
    assert(EdgePanAt(0, 200, 800, 480, 28).x == -1.0);
    assert(EdgePanAt(799, 200, 800, 480, 28).x == 1.0);
    assert(EdgePanAt(400, 0, 800, 480, 28).y == -1.0);
    assert(EdgePanAt(400, 479, 800, 480, 28).y == 1.0);
    assert(EdgePanAt(0, 480, 800, 480, 28).x == 0.0);
    assert(EdgePanAt(0, 550, 800, 480, 28).y == 0.0); // beyond frame
    // Window includes top menu (0..56), battlefield (56..500) and HUD (500..600).
    assert(EdgePanAt(400, 0, 800, 600, 40).y == -1.0);
    assert(EdgePanAt(400, 20, 800, 600, 40).y < 0.0); // over menu
    assert(EdgePanAt(400, 56, 800, 600, 40).y == 0.0); // inside field
    assert(EdgePanAt(400, 510, 800, 600, 40).y == 0.0); // early HUD
    assert(EdgePanAt(400, 599, 800, 600, 40).y == 1.0); // under HUD
    assert(EdgePanAt(0, 580, 800, 600, 40).x == -1.0); // side through HUD
    assert(EdgePanAt(0, 580, 800, 600, 40).y > 0.0); // diagonal
    assert(EdgePanAt(400, 600, 800, 600, 40).y == 0.0); // outside
    assert(EdgePanAtScreen(1400, 225, 1000, 200, 800, 600, 40).y < 0.0);
    assert(EdgePanAtScreen(-1600, 1100, -2000, 600, 800, 600, 40).y == 0.0);
    assert(EdgePanAtScreen(-1600, 1199, -2000, 600, 800, 600, 40).y == 1.0);
    // Near-edge acceleration remains continuous but much less sluggish.
    assert(EdgePanAxis(20, 0, 800, 40) < -0.7);
    assert(EdgePanAt(-1, 100, 800, 480, 28).x == 0.0);
    assert(EdgePanAt(800, 100, 800, 480, 28).x == 0.0);
    assert(EdgePanAt(27, 100, 800, 480, 28).x < 0.0);
    assert(EdgePanAt(28, 100, 800, 480, 28).x == 0.0);
    assert(EdgePanAt(0, 0, 800, 480, 28).x == -1.0);
    assert(EdgePanAt(0, 0, 800, 480, 28).y == -1.0);
    assert(EdgePanAt(0, 0, 20, 20, 28).x == -1.0);
    assert(EdgePanAt(10, 10, 20, 20, 28).x == 0.0);
    assert(EdgePanAxis(0, 0, 800, 0) == 0.0);

    // Fractional logical pixels accumulate over timer ticks: no speed
    // dependency on how many wx mouse-move events the OS delivered.
    EdgePanAccumulator pan;
    int total = 0;
    for (int i = 0; i < 100; ++i)
        total += pan.Advance({1.0, 0.0}, 0.001, 1.0).first;
    assert(total == 160);
    pan.Reset();
    total = 0;
    for (int i = 0; i < 100; ++i)
        total += pan.Advance({1.0, 0.0}, 0.001, 0.5).first;
    assert(total == 80);

    pan.Reset();
    // One delayed event cannot teleport the camera across the map.
    assert(pan.Advance({1.0, 0.0}, 10.0, 1.0).first == 80);
    pan.Reset();
    pan.Advance({0.01, 0.0}, 0.01, 1.0);
    assert(pan.Advance({0.0, 0.0}, 0.01, 1.0).first == 0);
    assert(pan.Advance({-1.0, 0.0}, 0.01, 1.0).first < 0);
    // Keyboard directions use the same frame-rate-independent accumulator.
    assert(KeyboardPanDirection(true, false, false, false).x == -1.0);
    assert(KeyboardPanDirection(false, true, false, false).x == 1.0);
    assert(KeyboardPanDirection(false, false, true, false).y == -1.0);
    assert(KeyboardPanDirection(false, false, false, true).y == 1.0);
    assert(KeyboardPanDirection(true, true, false, false).x == 0.0);
    assert(KeyboardPanDirection(false, false, true, true).y == 0.0);
    const auto diagonal = KeyboardPanDirection(true, false, true, false);
    assert(diagonal.x == -1.0 && diagonal.y == -1.0);
    pan.Reset();
    const auto keyboardStep = pan.Advance(diagonal, 0.01, 1.0);
    assert(keyboardStep.first == -16 && keyboardStep.second == -16);
    pan.Reset();
    const auto releasedStep = pan.Advance({}, 0.01, 1.0);
    assert(releasedStep.first == 0 && releasedStep.second == 0);
    // Reversing direction must not carry stale fractional movement.
    pan.Reset();
    pan.Advance({0.015, 0.0}, 0.01, 1.0);
    assert(pan.Advance({-1.0, 0.0}, 0.01, 1.0).first < 0);

    // The far edge must never become active outside a window's rectangle.
    assert(EdgePanAtScreen(1100, 300, 300, 200, 800, 600, 40).x == 0.0);
    assert(EdgePanAtScreen(1099, 300, 300, 200, 800, 600, 40).x == 1.0);
    return 0;
}
