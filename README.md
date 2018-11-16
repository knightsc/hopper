# Hopper

The following is a collection of plugins and scripts for use with the [Hopper Disassembler](https://www.hopperapp.com/)

## include

Hopper SDK header files

## cpus

### GBCPU
Implements a CPU plugin for the Game Boy z80 CPU

## loaders

### GBLoader
Implements a Game Boy ROM loader. Hopper doesn't currently fully support banked memory but the full ROM is loaded with additional banks at higher memory addresses

### GBALoader
Implements a Game Boy Advance ROM loader. Makes use of the built in Hopper ARM CPU support. It can optionally load the GBA Bios for disassembly along with the ROM. Hopper currently incorrectly disassembles the first instruction in Thumb mode when it should be ARM mode.

## tools

## scripts

Hopper python scripts that need to be installed in `~/Library/Application Support/Hopper/Scripts/`

## Related Links

[https://github.com/phracker/HopperScripts](https://github.com/phracker/HopperScripts)  
[https://github.com/agatti/hopper-plugins](https://github.com/agatti/hopper-plugins)
