# Necro-Multi-Patch
Repository for __Necro Multi-Patch__ addon, a [SourceMod](https://github.com/alliedmodders/sourcemod) plugin specially created for __[tick 100]NecroHELL!__ and 

__[tick 100]!!crossfire 4Ever!!__, both are public Black Mesa servers created by __[Dr.Necro](https://steamcommunity.com/profiles/76561198071553465/)__.

## Console Variables
This plugin adds following ConVars:


* **`necro_allowfastrespawn`**: `1` - Allow player respawn by pressing the buttons before `spec_freeze_time` and `spec_freeze_traveltime` is finished.
* **`necro_bolthitscandamage`**: `65.0` - Sets specified amount of damage for the crossbow bolt hitscan.
* **`necro_boltparticles`**: `1` - Enables trail for explosive crossbow bolts, the trail makes it easier to determine where the bolt was fired from.
* **`necro_classicfrags`**: `0` - By default, this plugin implements frag grenade physics from the singleplayer campaign, allowing players to pick up it. If this console variable is enabled, the game will use the legacy physics.
* **`necro_classiclaserdot`**: `0` - By default, this plugin implements new rendering for laser dot to prevent glowing through walls. If this console variable is enabled, the new rendering will be disabled.
* **`necro_explodingbolt`**: `1` - Enables exploding crossbow bolts.
* **`necro_fastrespawndelay`**: `0.5` - Amount of time in seconds before player can respawn by pressing the buttons with enabled fast respawn.
* **`necro_givedefaultitems`**: `1` - If enabled, gives default weapon pack for the player when spawned.
* **`necro_mp5contactparticles`**: `1` - Enables smoke for MP5 barrel grenade.
* **`necro_overridedefaultweaponparams`**: `1` - Enables parameters override for all weapons by loading custom weapon script with __necro_ postfix.

## Contribution

- Report issues if you find.
- Create feedback about our servers and suggest improvements.
- Users are able to add their own changes, using `Pull requests` system.

## Credits

- Used a part of code from [SourceCoop](https://github.com/ampreeT/SourceCoop).
- Used and reworked code from [fast_spawn](https://forums.alliedmods.net/showthread.php?p=2362850) plugin by __Alienmario__ ([Steam](https://steamcommunity.com/id/4oM0/), [GitHub](https://github.com/Alienmario)).
- Used and reworked code from [L4D2 Lag Compensation Null CUserCmd fix](https://hlmod.net/threads/krash-servera-ne-ochen-ponjatny-prichiny.35472/post-600631) plugin by __fdxx__.
  
TODO: Add more in README.md.
