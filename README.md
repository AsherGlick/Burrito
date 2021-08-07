Burri Tactical Overlay
======================
BurriTO is an early alpha overlay tool for Guildwars 2 that runs on linux without any display hacks.

The primary goal is to render and edit POIs and routes based off of sharable data files while being able to run on linux with no workarounds.

It functions natively linux via running a pre-compiled binary inside of wine that streams the memory mapped data back to the host.


Getting Started
===============
Lutris
======
Get the lutris id for guildwars 2. It is usually `guild-wars-2` but could be different.
```
lutris --list-games
```

Now we need to extract the environment variables and wine path lutris uses for GW2. To do this we will have lutris create a standalone launcher script for GW2 then we will modify the script to run our linker binary instead of Guildwars.
```
lutris guild-wars-2 --output-script `pwd`/run_burrito_link.sh
```

Now we need to change the last line of the scirpt to point to our binary. It probably looks something like this
```
gamemoderun /home/myuser/.local/share/lutris/runners/wine/lutris-5.7-11-x86_64/bin/wine '/home/myuser/Games/guild-wars-2/drive_c/Program Files/Guild Wars 2/GW2-64.exe' -autologin
```

We want to change it to
```
/home/myuser/.local/share/lutris/runners/wine/lutris-5.7-11-x86_64/bin/wine '<path to this repo>/burrito_link/burrito_link.exe'
```

Now the script can be executed to start running the burrito link.
```
./run_burrito_link.sh
```

Wine
====
If you are using your own wine prefix then all you need to do is run `burrito_link.exe` file within the same envrionment as GW2.


Short Term Roadmap
==================
* Automatically run the burrito_link binary when burrito starts.
* Launch the burrito_link binary via an internal config instead of via an external script.
  * A seperate script to generate the config from Lutris settings automatically.
* Automatically close burrito when GW2 closes.
* Automatically parse/convert TacO marker files instead of using a converter script.

Known Bug Workarounds
=====================
* Burrito launches off-center or on the wrong monitor
  * This seems to be WM dependant. on Gnome it can be solved by holding down the super key and dragging burrito to the right position.
* Burrito does not allocate space for the mumble mmap
  * This can be fixed by running some other program that takes advantage of the link such as arcdps.
