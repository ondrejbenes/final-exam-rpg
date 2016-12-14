Requirements

* Visual Studio 2015 (with Visual C++)

* SFML 2.4.0 (download - http://www.sfml-dev.org/download/sfml/2.4.0/ (download C++14 version), install guide - http://www.sfml-dev.org/tutorials/2.4/start-vc.php)

* Bitbucket account (http://bitbucket.org)



Installation

1.Install VS 2015

2.Install SFML (rec. folder - C:\Program Files\SFML)

3.Clone project from Bitbucket (https://bitbucket.org/Ondrejb/imt3601_shineydonkeys) 
 
4. Configure SFML in project settings (follow instructions from SFML page) - steps a - d should now be a part of the repo, but check anyway


5. Switch to All Configurations

6. Add additional include directories (C:\Program Files\SFML\include)
 
7. Add additional library directories (C:\Program Files\SFML\lib)

8. Add dependencies for both debug and release (switch configurations)

* "sfml-graphics-d.lib"; "sfml-window-d.lib"; "sfml-system-d.lib"; "sfml-audio-d.lib"; "sfml-network-d.lib"; "sfml-main-d.lib"
* "sfml-graphics.lib"; "sfml-window.lib"; "sfml-system.lib"; "sfml-audio.lib"; "sfml-network.lib"; "sfml-main.lib"

9. Build solution for both Debug and Release

10. Copy dlls from SFML/bin to .../Debug and .../Release*