# Spellcross Map Editor

This project is highly experimental tool for editing game map files of my favourite turn based strategy game [Spellcross](https://wikipedia.org/wiki/Spellcross).
The project is basically collection of my reverse engineering experimental [tools](http://spellcross.kvalitne.cz) made around 2007 - 2013.
Those tools were made in Borland Developer Studio BDS2006 Turbo C++ with VCL graphics interface and the code was very nasty mix of ANSI C and C++. 
Recently I decided to try to learn C++ a bit more so I decided to scoop the bits of my old code, the loaders, the decoders and renderers of the Spellcross data files, convert them to C++ classes using only standard C++ libraries and eventually merge them into one application. The result is this map editor project. I already had one simplistic prototype in past made in the Borland BDS2006, but the whole application was very messy, mixed GUI and code, so I rather started again. Note the code is still quite messy, because I had no time to refactor it yet, but already does something.

Borland C++ with its VCL GUI (later RAD studio) is dead end and VCL implementation was always quite a mess anyway, so I decided to use MSVC 2019 environment as I'm a bit familiar with it. But I'm not a masochist to use its Windows Forms GUI because it's using really awful C++ to .NET interface. Instead I decided to use multiplatform GUI [wxWidgets](https://wxwidgets.org) which seems reasonably simple to use. 

So far the tool can do following:
- Load terrain sprites (tiles, objects, tile animations, sprite animations).
- Load unit graphics (20000+ compressed sprites) and units definition file JEDNOTKY.DEF.
- Load and play sound resources.
- Load and play MIDI music resources. 
- Load and play video resources (DPK, DP2 fully implemented, CAN only audio because it is [Cauldron](https://sk.wikipedia.org/wiki/Cauldron) proprietary codec).
- Load and save map DTA and DEF map script files.
- Runtime render the map with all its layers.
- Very crude map terrain editing (mostly only copy/paste). Autotile mapping not done yet. 
- Edit/add/remove units.
- Edit/add/remove mission events and objectives (buggy, incomplete). 

<img src="./doc/spell_map_edit_v1.png">    
<img src="./doc/spell_map_edit_v2.png">
<img src="./doc/spell_map_edit_v3.png">
                                 
## License
The tool is distributed under [MIT license](./LICENSE.txt). 
  
  
