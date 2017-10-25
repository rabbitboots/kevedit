# KevEdit

KevEdit is a ZZT editor that was under development from 2000-2005 by Kevin
Vance and Ryan Phillips.  It runs under DOS, Windows, macOS, and Linux.

As of 2017 it is maintained by Kevin Vance on github.  Maintenance is focused
on improvements for running KevEdit on modern computers while still supporting
DOS.


## Download

Binary releases are available:

* [DOS](https://github.com/cknave/kevedit/releases/download/1.0.1/kevedit-1.0.1-dos.zip)
* [macOS 64-bit app](https://github.com/cknave/kevedit/releases/download/1.0.1/kevedit-1.0.1.dmg)
* [Linux x86_64 AppImage](https://github.com/cknave/kevedit/releases/download/1.0.1/kevedit-1.0.1-x86_64.AppImage)
* [Windows x64 installer](https://github.com/cknave/kevedit/releases/download/1.0.1/kevedit-1.0.1-setup.exe)


## Usage

    $ kevedit [file.zzt]

Press `H` in the editor to access the interactive help.


## Building from source

    $ ./bootstrap.sh
    $ ./configure
    $ make
    $ sudo make install


## Creating binary artifacts

Docker containers are provided for building Linux AppImage binaries, as well as
cross-compiling to macOS, Windows, and DOS.  This is automated by a python
build script:

    $ cd inst
    $ ./build.py [appimage] [dos] [macos] [windows]
    $ ls dist


## Building docker images

To build the docker images and tag them as 'latest' instead of pulling them
from Docker Hub:

    $ cd inst
    $ ./build.py -i build -t [appimage] [dos] [macos] [windows]


## Screenshots

![The main KevEdit interface](https://cloud.githubusercontent.com/assets/4196901/22183137/b51c68e6-e084-11e6-874d-3458041f4308.gif)
![KevEdit navigates a world with the Board Selector](https://cloud.githubusercontent.com/assets/4196901/22183135/b515754a-e084-11e6-9fe3-2483eb67ca79.gif)
![The character picker for objects](https://cloud.githubusercontent.com/assets/4196901/22183134/b514af02-e084-11e6-9ca7-7b21bedb479d.gif)
![Using the large backbuffer to copy a whole gradient](https://cloud.githubusercontent.com/assets/4196901/22183131/b5142230-e084-11e6-95c1-19133c677388.gif)
![Loading a world from any directory](https://cloud.githubusercontent.com/assets/4196901/22183132/b5142208-e084-11e6-8ab1-568d217391ec.gif)
![Sophisticated stats editing](https://cloud.githubusercontent.com/assets/4196901/22183133/b51426b8-e084-11e6-8ce7-e01b7d6a06ed.gif)
![The very colorful program editor](https://cloud.githubusercontent.com/assets/4196901/22183136/b516dd4a-e084-11e6-8e9b-30201734480a.gif)
