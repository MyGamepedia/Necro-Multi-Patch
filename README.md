[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/MyGamepedia/Necro-Multi-Patch)
# Necro-Multi-Patch
Repository for __Necro Multi-Patch__ addon, a [SourceMod](https://github.com/alliedmodders/sourcemod) plugin specially created for __[tick 100]NecroHELL!__ and 

__[tick 100]!!crossfire 4Ever!!__, both are public Black Mesa servers created by __[Dr.Necro](https://steamcommunity.com/profiles/76561198071553465/)__.

## Contribution

- Report issues if you find.
- Create feedback about our servers and suggest improvements.
- - Make sure your issue does not repeat someone's. If you want to make a feedback on the same problem, please leave your comment under someone's issue or use emoji to support it.
- Users are able to add their own changes, using `Pull requests` system.

## New Systems
This plugin adds several systems that control the design of various aspects of the game, their behavior can be customized depending on the current needs of the server or game mode.

### Respawn
* There is optional "fast respawn" that allows a client to respawn before the auto respawn by clicking action buttons. The amount of time before the client can do "fast respawn" after death controlled by `necro_fastrespawndelay` convar, the entire ability of "fast respawn" can be disabled with `necro_allowfastrespawn` convar.
* The auto respawn can be disabled with `mp_forcerespawn` convar. Keep in mind that dead client will not be able to respawn after death, this is why it is recommended to use it with "fast respawn" enabled.
* The amount of time before respawn can be changed using `spec_freeze_traveltime` (the amount of time to zoom in to killer for the freeze camera (default is 0.4)) and `spec_freeze_time` (the amount of time before auto respawn after `spec_freeze_traveltime` (default is 4.0).
* There is optional delay before spectator can team a join or play deathmatch. Every time a player goes to spectator mode, the player will receive a time delay that doesn't allow the player to play in a team or deathmatch again, until the time runs out. This prevents some sort of griefering when a player constantly goes to spectators for advantage in a match, the system also completely prevents instant respawn when used "Auto Assign" button via the team menu. The delay can be changed using `necro_spectatorjointeamdelay` console variable.

### Crossbow bolts
* Restored functionality of `sk_crossbow_tracer_enabled`, which means you can use the `tracerbullet` entity for the bolts.
* The explosive bolts can be disabled, using `necro_explodingbolt` convar, this allows to use default bouncing bolts.

### Satchel delay
* Due to various bugs with the weapon models and gameplay issues caused by spamming this weapon, was added this system that allows to control satchel attack delay (both primary and secondary) in 3 different cases, primary attack, secondary attack and reload (when the owner take out a new satchel).
* For primary attack, it is controlled using `necro_satcheldelay_attack1_primary` and `necro_satcheldelay_attack1_secondary`.
* For secondary attack, it is controlled using `necro_satcheldelay_attack2_primary` and `necro_satcheldelay_attack2_secondary`.
* For reload, it is controlled using  `necro_satcheldelay_reload_primary` and `necro_satcheldelay_reload_secondary`.
* This system can be disabled using `necro_satcheldelayoverride` (not recommended).

### Projectile trails
* Added particle trails for some projectiles to make it a little easier to figure out their flight path, this helps to determine the location of the attacker.
* For MP5 contact grenade, it is controlled using `necro_mp5contactparticles`.
* For crossbow bolts, it is controlled using `necro_boltparticles`.

### Default items give
* The default spawn items list (crowbar, 3 frags, glock, full ammo for 9mm) can be disabled using `necro_givedefaultitems`.
 
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
* **`necro_otherplayersflashlight`**: `1` - Allow creation of flashlight effects, used as flashlight from other players perspective.
* **`necro_overridedefaultweaponparams`**: `1` - Enables parameters override for all weapons by loading custom weapon script with __necro_ postfix.
* **`necro_satcheldelay_attack1_primary`**: `1.0` - Sets delay for satchel weapon primary attack when the satchel thrown.  Recommended 0.84 at least to avoid bugs with the satchel rendering.
* **`necro_satcheldelay_attack1_secondary`**: `1.2` - Sets delay for satchel weapon primary secondary when the satchel thrown. Recommended 1.2 at least to avoid bugs with the radio rendering.
* **`necro_satcheldelay_attack2_primary`**: `0.3` - Sets delay for satchel weapon primary attack when the radio is used.
* **`necro_satcheldelay_attack2_secondary`**: `0.2` - Sets delay for satchel weapon secondary attack when the radio is used.
* **`necro_satcheldelay_reload_primary`**: `1.0` - Sets delay for satchel weapon primary attack when the owner take out a new satchel.
* **`necro_satcheldelay_reload_secondary`**: `1.0` - Sets delay for satchel weapon secondary attack when the owner take out a new satchel.
* **`necro_satcheldelayoverride`**: `1` - Enables primary and secondary attack delay override for satchel weapon.
* **`necro_spectatorjointeamdelay`**: `15.0` - Amount of time in seconds before spectator can join a team or play deathmatch again after the player joined spectators.


## Credits

- Used a part of code from [SourceCoop](https://github.com/ampreeT/SourceCoop).
- Used and reworked code from [fast_spawn](https://forums.alliedmods.net/showthread.php?p=2362850) plugin by __Alienmario__ ([Steam](https://steamcommunity.com/id/4oM0/), [GitHub](https://github.com/Alienmario)).
- Used and reworked code from [L4D2 Lag Compensation Null CUserCmd fix](https://hlmod.net/threads/krash-servera-ne-ochen-ponjatny-prichiny.35472/post-600631) plugin by __fdxx__.
  
TODO: Add more info in README.md generally.
