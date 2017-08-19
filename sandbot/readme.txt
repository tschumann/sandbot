Sandbot

== About ==
Name: Sandbot
Description: A deathmatch bot for Half-Life, Half-Life Opposing Force (Deathmatch), Day of Defeat (Territorial Control), Gunman Chronicles, Natural Selection (Combat) and They Hunger (under the hood there is support for Half-Life Opposing Force (Capture the Flag), Team Fortress Classic, Natural Selection (Classic) and The Ship but they are still experimental).
Requirements: Steam versions of either Half-Life, Half-Life Opposing Force or Day of Defeat and the latest versions of Gunman Chronicles, Natural Selection or They Hunger.
Installation: Copy the files into your Steam/steamapps/common/Half-Life directory

Sandbot comes packaged with MetaMod-P for Windows and MetaMod for Linux.
MetaMod will work across the board except for with Gunman Chronicles due to a bug in MetaMod itself.
MetaMod-P should work across the board but has been found to freeze even without Sandbot on Ubuntu.

== Usage ==

addbot		Add a bot to the server
		In Day of Defeat you can addbot allies or addbot axis
		In Natural Selection you can addbot alien or addbot marine
botskill [1-5]	Set the bot skill level
waypoint add	Add a waypoint
waypoint delete	Delete a waypoint
waypoint save	Write waypoints to file
waypoint info	Print what type of waypoint a waypoint is

== Frequently Asked Questions ==

Q. Can you add support for mod x?
A. I'm not planning to at the moment - I'm planning support for Half-Life, Opposing Force, Day of Defeat, Team Fortress Classic, Gunman Chronicles, Natural Selection, The Ship and They Hunger which is likely going to take a while.

Q. Can it be used for dedicated servers?
A. Maybe - I haven't tested it, but it should.

Q. Can I make some waypoints?
A. Yes, please do!

Q. Does single player work?
A. No but it will be fixed in a future version.

== Changelog ==

v0.4.1 (??/??/??)
- [All] Replaced botdonshoot console command with bot_shoot cvar.
- [Windows] Updated Windows compiler to Visual Studio 2017 (from Visual Studio 2008).
- [Linux] Updated Linux compiler to gcc 6.3.0 (from I'm not sure what - gcc 4.8.2? gcc 4.9.0?).
- [Linux] Added a bunch of extra gcc flags to better optimise the code (thanks Robocop@Bots United).
- [Linux] Tidied up the Makefile.
- [Linux] Added debug symbol generation to Makefile.
- [Day of Defeat] Bots will now move at walk speed instead of sprinting everywhere.
- [Day of Defeat] Marine bots will now say when they're reloading.
- [Natural Selection] Marine bots will now say when they're reloading.
- [Natural Selection] Marine bots are now slightly less accurate to compensate for the fact their isn't reduced in dark areas.

v0.4.0.1 (18/06/17)
- [All] Fixed single player not loading (thanks shardthefox200@ModDB)
- [Linux] Replaced included MetaMod-P for Linux with MetaMod for Linux as MetaMod-P almost always freezes on Ubuntu.

v0.4 (05/03/17)
- [All] Increased number of allowed waypoints to 2048 (thanks 256 colors).
- [All] Added bot_count cvar to control how many bots spawn by default (mapname_bot.cfg has been removed).
- [All] Added MetaMod support.

v0.3.1 (26/01/17)
- [All] Bots no longer rejoin when the map changes.
- [Half-Life] Bots are now random colours.
- [Half-Life] Bots now automatically join a game.
- [Opposing Force] Bots are now random colours.
- [Opposing Force] Bots now automatically join a game.
- [Day of Defeat] Sandbot will now let you know if you're playing on a map with a flag model that is doesn't understand.
- [Day of Defeat] Bots now automatically join a game.
- [Gunman Chronicles] Bots now use new weapon selection again.
- [Gunman Chronicles] Bots now automatically join a game.
- [Natural Selection] Bots will now consider all enemy structures as enemies.
- [Natural Selection] Marine bots will now build structures in Classic.
- [They Hunger] Bots now automatically join a game.

v0.3.0.1 (10/01/17)
- [Natural Selection] Fixed liblist.gam not loading sandbot.dll

v0.3 (12/11/16)
- [All] Fixed kicking a bot causing the game to crash.
- [Opposing Force] Bots will look for random goals on maps with waypoints.
- [Day of Defeat] Bots will now capture points.
- [Gunman Chronicles] Bots will look for random goals on maps with waypoints.
- [Natural Selection] Bots will now reload after they haven't seen an enemy in a while.
- [Natural Selection] Bots will now attack the command chair and the built hive in Classic.
- [They Hunger] Bots will look for random goals on maps with waypoints.

v0.2.1 (10/08/16)
- [Day of Defeat] Bots now spawn as Allies and Axis (only the first four classes of each team) and attack each other.
- [Gunman Chronicles] Bots now use different gauss pistol modes.
- [Gunman Chronicles] Bots now know the gauss pistol is fully automatic.
- [Natural Selection] Alien bots no longer slow down when getting close to an enemy to attack.
- [Natural Selection] Bots will now rejoin after a round has ended.
- [Natural Selection] Alien bots will now become all different lifeforms.
- [Natural Selection] Stopped bots randomly pausing from time to time.
- [Natural Selection] Combat bots now upgrade more abilities.
- [Natural Selection] Marine bots will no longer upgrade when they have an enemy in combat mode.

v0.2 (18/06/16)
- [Natural Selection] Added better combat mode play.
- [Gunman Chronicles] Gauss pistol rate of fire slightly.
- Started using new bot framework (which brings Linux support with it).
- Added some documenation.

v0.1 (02/05/16)
- Initial release (after more than 5 years of being untouched and unreleased).

== End User License Agreement ==

The developer of this software is in no way responsible for anything that happens as a result of using this software.
This software is provided as-is and with no warranty of any kind, and no guarentees are made or implied.
Also you cannot now or ever take any legal action against the developer of this software for anything.
You do not have the right to re-use or modify any part of this modification.
You may not use the software if you do not agree to these terms.

== Thanks ==

Thanks to:
Valve, for the Half-Life SDK and the Valve ERC.
Todd Aspeotis, for teaching me C++.
Jeffrey 'botman' Broome for his bot code and other cool stuff.
Sam 'solokiller' VanHeer for his knowledge of the GoldSource engine.
Maleficus for setting a high standard of how good bots can be.
ModDB for being a good host.
Bots United for their support.

== Credits ==

Framework: botman
MetaMod support: The Storm
Programming: tschumann
