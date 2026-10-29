# Changelog

All notable changes to the PC port are listed here. The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and versions follow [Semantic Versioning](https://semver.org/).

## [0.1.1] - 2026-10-29

### Changed
- Weapon combos: a number key now takes the weapon of its group you held last, and skips weapons unchecked in the inventory (when any of the group is checked). Q (and L1) no longer counts a weapon you only passed through on the way, so pressing 1 twice to reach the IAC Defender and then Q returns to the weapon you had before.

### Fixed
- Windows: the game now starts after choosing a disc on the setup screen. It stopped with "SLUS_012.72 is not on the disc", because the `.cue` was read in place of the disc image.
- PORT SETTINGS: HD MOVIES shows NONE, and can't be turned on, until there are HD movie clips to play.
- Endless Mode: big frame-rate drops with some weapons lent from other missions, such as the rocket launcher in Courier's rooms, at high refresh rates with smooth motion.

## [0.1.0] - 2026-10-28

### Added
- First public release of the native PC port for Windows and Linux (32-bit x86).
- Smooth motion: frames drawn between the game's 20 Hz ticks at the display's refresh rate.
- Hardware renderer (Vulkan) at up to 8x the PlayStation's resolution, with true colour; the software renderer remains available.
- Mouse and keyboard controls (keys by position, any layout) and a modern gamepad layout, with rumble and sprint.
- Every action rebindable on keyboard, mouse and gamepad: PORT SETTINGS → CONTROLS, or the `[controls]` block in `twine.ini`.
- Quick punch and quick grenade on their own buttons, with an optional infinite grenades setting.
- Endless Mode: waves of guards through 61 rooms from nine missions, weapons and health carried from room to room, weapons lent from other missions, scores and medals on the memory card.
- Blackjack straight from the main menu.
- The developers' debug menu (F9), with three pages restored from a 2000 beta.
- HD movies: the mission films played from your own copy of the film (`port/tools/hdmovies.py`).
- PORT SETTINGS inside the game's Options menu, including the message box at the top or bottom.
- Save states (F5 / F6).
- First-run setup screen that finds and checks the disc image, optional BIOS, and a crash report box.
- Portable mode: settings and saves next to the game.
