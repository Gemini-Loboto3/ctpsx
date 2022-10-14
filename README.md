# Clock Tower: The First Fear reversed engineered
A decompilation project of the PC version of the game, but using PlayStation assets for better visual fidelity and extras that came with that port.

## Working
* Main virtual machine commands (can start the intro and fully play it, quick start, continue, or show the ending list);
* Most general routines for maps, entities, etc;
* General API via DirectX (should make porting rather trivial);
* OGG playback for music.

## Not working
* Controls (only left-right clicks are recognized via DInput8 and some base cursor movement);
* Sprite code not fully working;
* TMC support is just a stub;
* Missing PSX specific code (i.e. fully controllable title screen and context menu);
* Missing STR (compressed videos) support.
