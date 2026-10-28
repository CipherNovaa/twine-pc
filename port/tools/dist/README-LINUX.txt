TWINE - 007: The World Is Not Enough, native PC port (Linux)
============================================================

You need your own copy of the game: the American PlayStation disc (SLUS-01272), as a
disc image (.cue and .bin, as DuckStation or a disc ripper makes). Nothing else needs
installing.

1. Unpack this folder anywhere.
2. Run "twine" (double-click it, or ./twine in a terminal; on a Steam Deck, add it to
   Steam as a non-Steam game).
3. The first time, choose your disc image. It is checked and remembered.

A PS1 BIOS is optional: it only plays the console's start-up animation.

Settings:  ~/.config/twine/twine.ini   (./twine --setup shows the disc screen again)
Saves:     ~/.local/share/twine/
Log:       ~/.local/state/twine/twine.log

The game runs on the system's own 32-bit libraries when it has them (Steam Deck, or a PC
with Steam), so the graphics card draws; otherwise on the ones in runtime/, drawing on the
CPU. TWINE_RUNTIME=bundled ./twine or TWINE_RUNTIME=system ./twine picks one.

Third-party software and licences: licenses/THIRD-PARTY.txt
