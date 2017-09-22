RPG game developed by Shiney Donkeys studio as part of the NTNU's Game Programming course (imt3601).

![Image](https://i.imgur.com/4myaatB.jpg)

“Final Exam” is a survival RPG single player game. The player's goal is to kill the final boss. The game has a “spectator mode” multiplayer.

The player has no way of restoring any hitpoints he lost. Therefore, he needs to play around how much he stays in combat to have as much health as he can when he faces the final boss of the game. So in this game, players health is his surviving resource and can determine between winning and losing.

To kill the final boss you need to slay few other enemies to obtain keys leading to the final arena and weapons to improve your chances of killing them.

Features:

* Different kinds of hostile NPCs
* Enemies drop weapons and other loot
* Chat system (shared between NPCs, the player and spectators)
* Multiple character outfits to choose from

[Report](https://bitbucket.org/Ondrejb/final-exam-rpg-game/raw/de1ca35806b76893828d496cd7b664a92c957bb9/Report/FinalExamreport.pdf)

Requirements

* Visual Studio 2015 (with Visual C++)
* SFML 2.4.0 (download - http://www.sfml-dev.org/download/sfml/2.4.0/ (download C++14 version), install guide - http://www.sfml-dev.org/tutorials/2.4/start-vc.php)
* Bitbucket account (http://bitbucket.org)

Installation

1. Install VS 2015
2. Install SFML (rec. folder - C:\Program Files\SFML)
3. Clone project from Bitbucket (https://bitbucket.org/Ondrejb/final-exam-rpg-game)  
4. Configure SFML in project settings (follow instructions from SFML page) - steps a - d should now be a part of the repo, but check anyway
    1. Switch to All Configurations
    2. Add additional include directories (C:\Program Files\SFML\include)
    3. Add additional library directories (C:\Program Files\SFML\lib)
    4. Add dependencies for both debug and release (switch configurations)    
        * "sfml-graphics-d.lib"; "sfml-window-d.lib"; "sfml-system-d.lib"; "sfml-audio-d.lib"; "sfml-network-d.lib"; "sfml-main-d.lib"
        * "sfml-graphics.lib"; "sfml-window.lib"; "sfml-system.lib"; "sfml-audio.lib"; "sfml-network.lib"; "sfml-main.lib"
5. Build solution for both Debug and Release
6. Copy dlls from SFML/bin to .../Debug and .../Release*