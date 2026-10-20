# Quest 64: Recompiled mod template

A starting point for mods for [Quest 64: Recompiled](https://github.com/zmarotrix/Quest64-Recomp). The example mod,
**Blue Fire**, turns Fire Ball Lv1 into a recolored triple shot with more damage, with a mod-menu option for each change.

## What's here

| Path | What it is |
| --- | --- |
| `src/blue_fire.c` | The example mod: hooks `boot_load_static_data` and edits Fire Ball's data |
| `include/q64_spells.h` | Quest 64 spell data structures (`SpellDefinition`, `SpellAnimDefinition`) and table accessors |
| `include/modding.h`, `recomputils.h`, `recompconfig.h` | Mod API headers from the N64Recomp mod template |
| `mod.toml` | Mod manifest and mod-menu options |
| `Quest64Syms/` | Function and data names for the game (submodule) |

## Building

You need the same tools as Quest 64: Recompiled (see its BUILDING.md): MSYS2's clang (for MIPS), LLVM's `ld.lld`,
`make`, and `RecompModTool.exe` from the recomp build.

```bash
git clone --recurse-submodules https://github.com/zmarotrix/Quest64-ModTemplate.git
cd Quest64-ModTemplate
make
path/to/RecompModTool.exe mod.toml build
```

This produces `build/q64_blue_fire_example.nrm`. Copy it to `%LOCALAPPDATA%\Quest64Recompiled\mods` and enable it
in the game's mod menu.

## Notes for modders

- **Enemies share player spells.** Enemy AI casts spells by ID from the same tables Brian uses, so changing a
  player spell also changes it for enemies that cast it (Blue Fire turns enemy fireballs blue too).
- Spell colors: the low byte of a visual stage's `color` picks the color.
- Function names come from `Quest64Syms/names.toml`. Packaged mods refer to functions by address, so renames
  don't break built mods; only rebuilding from source needs the current names.

## Credits

Research behind `q64_spells.h`: the Eltale Monsters Discord community (modding spreadsheet), the Quest 64 decomp
contributors on decomp.me, and the [Merrow randomizer](https://github.com/hangedmandesign/merrow). Build setup
adapted from the [MMRecompModTemplate](https://github.com/Zelda64Recomp/MMRecompModTemplate) (CC0).

## License

[CC0 1.0](LICENSE), like the template it's based on: use it for your own mods without restriction.
