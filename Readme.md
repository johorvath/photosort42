Project for the lecture "Softwaretechnologie" at the University of Applied Science Ulm"

PhotoSort42

Sorting program for fotos via face detection and recognition

QT:
- add TODO flag:
    - Help->About Plugins-> "Utilities" -> "Todo"
    - config: Tools -> Options -> Todo
    - Todo entries in the bar below

TODO
- file handling (check, etc.)
- output path handling
- comment EVERYTHING
- add logger
- talk about detection method/strategy; how many cascades?
- remove Gtk-WARNING
- perhaps not only choose photos but also folder?
- add filedialog for output
- add explanation about installing needed libs
	- boost: http://particlephysicsandcode.com/2013/03/11/installing-boost-1-52-ubuntu-12-04-fedora/ (instead of the last 2 lines do: 
		-sudo gedit ~/.bashrc
		at the end: export BOOSTROOT=/usr/local/boost-1.56.0
		or
		export BOOSTROOT=/usr/local/boost_1_56_0
	- opencv: http://www.sysads.co.uk/2014/05/install-opencv-2-4-9-ubuntu-14-04-13-10/
	- qtcreator: http://qt-project.org/downloads

