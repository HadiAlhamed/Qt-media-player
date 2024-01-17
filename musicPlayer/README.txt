# Media Player

This is a simple media player project created with Qt Framework and C++. It can play audio and video files using various codecs.

## Features

The media player has the following features:

- Play, pause, stop, and seek media files
- Open files or folders from the menu or by drag and drop
- Adjust volume, mute, and toggle full screen mode
- Create and manage playlists, shuffle and loop modes
- Skip forward and backward by a fixed interval
- Clear playlist and remove selected items
- Handle open with from other applications

## Dependencies

The media player depends on the following Qt modules:

- Qt Multimedia
- Qt Widgets
- Qt Gui
- Qt Core

## Installation

To build and run the media player, you need to have Qt 6.6 or higher installed on your system. You can use Qt Creator to open the project file `media-player.pro` and build the project. Alternatively, you can use the command line tools `qmake` and `make` to build the project.

Note that you may need to install K-Lite Codec Pack if the the program could not play your media file
## Usage

To run the media player, you can either execute the binary file `media-player` from the build directory, or install the application to your system and run it from the menu or the command line. You can use the menu or the toolbar buttons to control the playback and the playlist. You can also use the following keyboard shortcuts:

- Space: play/pause
- 4: skip backward
- 3: skip forward
- 2: volume up
- 1: volume down


## License

This project is licensed under the GNU General Public License v3.0. See the `LICENSE` file for more details.
