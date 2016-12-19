Sandbot

== About ==
Name: Sandbot
Description: A deathmatch bot for Half-Life, Half-Life Opposing Force (Deathmatch), Day of Defeat, Gunman Chronicles, Natural Selection (Combat) and They Hunger (under the hood there is support for Half-Life Opposing Force (Capture the Flag), Team Fortress Classic, Natural Selection (Classic) and The Ship but they are still experimental).
Requirements: Steam versions of either Half-Life, Half-Life Opposing Force and the latest versions of Gunman Chronicles or They Hunger.
Installation: Copy the files into your Steam/steamapps/common/Half-Life directory

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

Q. Can you add MetaMod support?
A. I've tried and things just broke unexpectedly, so not right now.

Q. Can it be used for dedicated servers?
A. Maybe - I haven't tested it, but it should.

Q. Can I make some waypoints?
A. Yes, but hold off for now - things are probably going to change in that space.

== Changelog ==

v0.3.x (??/??/??)
- [Half-Life] Bots are now random colours.
- [Opposing Force] Bots are now random colours.
- [Natural Selection] Bots will now consider all enemy structures as enemies.
- [Natural Selection] Marine bots will now build structures in Classic.

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
Maleficus for setting a high standard of how good bots can be.
ModDB for being a good host.
Bots United for their support.

== Credits ==

Framework: botman
Programming: tschumann