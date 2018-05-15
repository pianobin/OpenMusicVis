# CS 126 Final Project: Music Visualizer

This is a customizable music visualizer built with [openFrameworks](http://openframeworks.cc/). This visualizer takes any mp3/wav file as input and displays a real-time audio spectrum and particle system. It also takes any png/jpg file as image input for the background display.

## Getting Started

### Prerequisites

You will need to download the latest [openFrameworks](http://openframeworks.cc/download/) library. This Music Visualizer uses the [ofxHistoryPlot](https://github.com/armadillu/ofxHistoryPlot) addon to display past data on audio frequencies as well as the [ofxPostProcessing](https://github.com/neilmendoza/ofxPostProcessing) addon for particle effects, so you will need to clone both repositories and add them to the `addons/` folder within your openFrameworks directory.

### Installing

Import the project using the Project Generator located within the `projectGenerator-osx` folder. Ensure the **ofxGui**, **ofxHistoryPlot**, and **ofxPostProcessing** addons are selected under the addons dropdown. Follow the setup guides within [here](http://openframeworks.cc/download/) to build and run with your favorite IDE.

### Usage

The program will prompt you with the system file directory. You can load a mp3/wav file from anywhere on your computer. The program comes with two presets, which can be toggled with the **p** key. The first preset is intended for solo instrument performances, such as piano and guitar songs. The second preset is for all other song types. Both presets are customizable with the **s** key, which contains the settings panel.

Press the spacebar to stop and start the current song. Use the right and left arrow keys to fast forward and rewind the audio file by 10 seconds.

To select a new song, press the **n** key. To change the background image, press the **b** key.

Many of the elements in the display can be toggled to fit personal preferences. Press the **h** key to toggle the history plot display. Press the **g** key to toggle the guide messages at the top of the screen. Press the **l** key to toggle the image display at the center.

The tint of the bars can be modified in the settings panel, but can also be changed randomly using the "Light Show" tickbox and the keyboard hotkeys 1-4.
