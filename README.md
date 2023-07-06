### About this repository
This repository serves as an example for the issue opened [here](https://github.com/DolbyIO/comms-sdk-unreal/issues/266);

### The issue
Despite the fact that the `Enable Video` method is providing a parameter for the video device to use, the Dolby IO Virtual Worlds plugin always uses the system default webcam.

### Steps to reproduce:
- ensure your system has at least 2 video devices/webcams connected. Most laptops have a built-in camera, and to get a second one you can download [OBS Studio](https://obsproject.com/download) and use its Virtual Webcam feature;
- clone this repository;
- download the [1.1.0](https://github.com/DolbyIO/comms-sdk-unreal/releases/tag/1.1.0) release of the DolbyIO Virtual Worlds UE Plugin and add it to the project;
- open up the `BP_Dolby_Debug_Actor` file and enter a valid DolbyIO token in the `SetToken` node;
- start PIE;
- the logic will attempt to get the video devices connected to the system and start previewing each one of them, switching at a 5 second interval. However, it seems that the underlying CPP SDK ignores the video device provided through the `Enable Video` parameter and uses the system default device;