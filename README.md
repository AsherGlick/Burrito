Burri Tactical Overlay

BurriTO is a fully featured 3D engine (godot) sitting as an overlay for GW2.

It can function on linux via running a binary inside of wine that streams the memory mapped data back to godot which is running natively.

The goal is to allow it to render POIs and routes based off of sharable data files, and also be able to run on linux.
Windows compatability should be a non-issue as you just run the binary that normally is run in wine. Maybe there will
be some way to auto-run it, but that is a feature left for after it can do basic functionality.


Current Issues:
	Fullscreen passthrough is not possible
		Fullscreen causes background to render black instead of transparent
		Maximized prevents things from staying on top
		Clicks are absorbed by the godot window

	There is a program that seems to have overcome this, it is a pure python key sharing program that displays your keypresses visually.
	It might be possible to use the same tech they were able to use, on godot.
	This might involve patching godot but I am willing to do this. Hopefully it will not be more then compiling an export template.
	Also it will hopefully allow me to submit something to godot. Which would be cool.

	Minor issues:
		Cannot read Taco files that tekkit produces. These seems to be xml files but more research will need to be done to see how hard they are to parse.



FAQ
===
What is "Burri"?
A string of words that when "TO" is added to it spelled "Burrito". It has no meaning beyond that.
