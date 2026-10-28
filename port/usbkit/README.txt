TWINE test kit
==============

1. Copy your own disc image (the .bin and .cue) into the disc folder here. Optionally your BIOS file into bios
   and the film clips into hdmovies (as for the game's own folder). The kit never brings any of them.
2. Double-click TEST.bat. It writes a system report, runs a few short tests without a window (about two
   minutes), then starts the game: play as long as you like and quit it to finish.
3. Everything lands in results\<date-time>\ next to this file: summary.txt has a PASS or FAIL line for each
   step. Bring the stick back; on Linux, tools/usbkit_compare.py <results folder> compares it with the same runs
   there.

The file "portable" keeps the game's settings, saves and memory cards in the twine folder here, not in
%APPDATA%. Nothing is installed and nothing is written to the registry. Works from any drive letter and from a
FAT32 or exFAT stick.
