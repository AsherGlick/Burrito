Burrito
================================================================================
Burrito is an overlay tool for Guild Wars 2 that runs on Linux.

The primary goal is to render and edit real-time 3D UI elements on Linux.

The Burrito UI functions natively on Linux, but requires running a small
windows binary inside of Wine that reads the memory mapped real-time
position data.


Getting Started
================================================================================
Go to [burrito.orthogonalprojects.com](https://burrito.orthogonalprojects.com) for instructions.


Roadmap
================================================================================
* Support for all of the XML node attributes that are supported by TacO and BlishHUD
* Native-looking user interface, with more-seamless mouse pass-through
* Support for preventing icons and trails from rendering over UI elements
* Automated testing for different desktop environments
* Automated releases for the `burrito` and `burrito-next` channels
* Marker pack documentation
* Better Wayland support


Known Bug Workarounds
================================================================================
* Burrito launches off-center or on the wrong monitor (This seems to be WM dependent.)
  * On Gnome and KDE it can be solved by holding down the super key and dragging burrito to the right position.
  * On KDE it is also possible to use [window-rules](https://docs.kde.org/stable5/en/kwin/kcontrol/windowspecific/examples.html) to set the position or a target monitor automatically by adding a new rule to `System Settings` → `Window Behavior` → `Window Rules` and setting `window class (application)` to `burrito`.
* Wayland / XWayland does not play nicely with transparency / mouse pass-through / window focus
  * Many wayland implementations are not as mature as X11. It is recommended at this time to use X11.
