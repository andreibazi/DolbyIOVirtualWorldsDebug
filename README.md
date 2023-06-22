### About this repository
This repository serves as an example for the issue opened [here](https://github.com/DolbyIO/comms-sdk-unreal/issues/266);

### Steps to reproduce:
- clone this repository;
- download the [1.1.0-beta.9](https://github.com/DolbyIO/comms-sdk-unreal/releases/tag/1.1.0-beta.9) release of the DolbyIO Virtual Worlds UE Plugin and add it to the project;
- open up the `BP_Dolby_Debug_Actor` file and enter a valid DolbyIO token in the `SetToken` node;
- start PIE;
- check the `Output Log` for the error mentioned in the issue;
