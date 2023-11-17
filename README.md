Burrito Overlay
===============
Burrito is an early alpha overlay tool for Guildwars 2 that runs on linux without any display hacks.

The primary goal is to render and edit POIs and routes based off of sharable data files while being able to run on linux with no workarounds.

It functions natively linux via running a pre-compiled binary inside of wine that streams the memory mapped data back to the host.


Getting Started
===============
Go to [burrito.orthogonalprojects.com](https://burrito.orthogonalprojects.com) for instructions


Roadmap
=======
* Automatically run the burrito_link binary when burrito starts.
* Launch the burrito_link binary via an internal config instead of via an external script.
  * A seperate script to generate the config from Lutris settings automatically.
* Automatically close burrito when GW2 closes.
* Automatically parse/convert TacO marker files instead of using a converter script.


Known Bug Workarounds
=====================
* Burrito launches off-center or on the wrong monitor (This seems to be WM dependant.)
  * On Gnome and KDE it can be solved by holding down the super key and dragging burrito to the right position.
  * On KDE it is also possible to use window-rules to set the position or a target monitor automatically. \
    If you are not yet familiar with window-rules, you may find the examples in the official documentation useful. [Link](https://docs.kde.org/stable5/en/kwin/kcontrol/windowspecific/examples.html) \
    To put it simply, you can add a new rule if you go to `System Settings → Window Behavior → Window Rules`. \
    Remember to set `window class (application)` to `burrito`.
