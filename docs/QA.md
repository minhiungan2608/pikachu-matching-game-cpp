# QA coverage

## Local verification

**Result:** C++17 build passed; CTest passed with 2,119 checks.

- GNU C++ 13.3, CMake 3.28; SDL2 2.30, SDL2_image 2.8, SDL2_mixer 2.8.
- Complete C++17 game and test executable build with compiler warnings enabled.
- AddressSanitizer and UndefinedBehaviorSanitizer enabled for the test executable.
- 1,000 fixed-seed board comparisons against an independent graph search allowing at most two turns.
- 150 generated coordinate pairs per level: all seven deletion/shift rules compared against an independent sequence-removal model (1,050 cases).
- Runtime checks use the actual SDL event loop and renderer with dummy video/audio drivers: menu, start, two-click matching, off-board input, pause/resume, level completion/progression, expired timer, exhausted shuffle allowance, restart, return to menu, and quit.
- All seven levels load their backgrounds, buttons, tile textures, audio effects, and pause/win/loss screens. Tile-count and shuffle-multiset invariants are checked.
- Timer checks cover start, paused stability, resume, and stop/reset.
- Demo media are reproducible from a Python standard-library generator. The gameplay screenshot is captured from SDL rendering during the runtime test.

The local host runs processes under tracing, so LeakSanitizer cannot run there. Local QA disables leak detection only; address/undefined-behavior checks remain enabled. The GitHub Actions workflow runs the sanitizer configuration on a standard runner without that local override.

## Packaging and source checks

Tracked deliverables are source/header files, CMake configuration, tests, documentation, the workflow, the asset generator, and its demo media. Executables, DLLs, object files, dependency caches, local build folders, IDE state, and media source archives are excluded.

Source review checks for hardcoded credentials, private configuration, personal contact data, local absolute paths, unexpected network/process calls, and missing include/asset references. The game does not require credentials or network access.

## Limits

Automated headless checks do not verify physical audio output, platform-specific display behavior, or a complete interactive seven-level playthrough. macOS/Windows builds have not been executed. Generated cases are regression coverage, not an exhaustive proof of all board states or full-board solvability. The game keeps its fixed-size board, numeric states, and raw-pointer design.

To capture a new frame while running the tests:

```bash
PIKACHU_CAPTURE_SCREENSHOT=docs/gameplay.png ctest --test-dir build-qa --output-on-failure
```
