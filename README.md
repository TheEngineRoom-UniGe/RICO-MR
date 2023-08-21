# Robot Intent Communication through Mixed Reality (RICO-MR)
Open-source Unreal Engine 4.27 based project implementing a Mixed Reality layer for enhancing communication in human-robot teams, applicable in a wide variety of interactive tasks.
The project is built using the following UE plugins:
    1. [Microsoft OpenXR](https://github.com/microsoft/Microsoft-OpenXR-Unreal)
    2. [Mixed Reality UX Tools](https://www.unrealengine.com/marketplace/en-US/product/mixed-reality-ux-tools)
and targets the **Microsoft HoloLens 2** Mixed Reality headsets.

We provide both a binary version of the application, which you can immediately deploy on your devices to start experimenting in different scenarios, or a complete walkthrough on how to import the project in the UE editor if you wish to customize and expand certain functionalities.

## Installation

### Binary Version

Visit the [release page](https://github.com/TheEngineRoom-UniGe/RICO-MR/releases) and download the most recent release of RICO-MR. In order to deploy the application onto your HoloLens 2 device, simply follow the procedure described in the following [guide](https://learn.microsoft.com/en-us/windows/mixed-reality/develop/unreal/tutorials/unreal-uxt-ch6#packaging-and-deploying-the-app-via-device-portal). Specifically, refer to the paragraph *Packaging and deploying the app via device portal*, and follow the instructions mentioned at steps 6, 7 and 8.

### Import Project in UE Editor

Before importing the project in UE, make sure to follow the [Introductory Guide](https://learn.microsoft.com/en-us/windows/mixed-reality/develop/unreal/tutorials/unreal-uxt-ch1), which provides step by step instructions on how to setup UE for Mixed Reality development.

Additionally, in order to make sure that packaging the application for HoloLens 2 works successfully, please locate and delete the `libWebSockets` and `OpenSSL` folders, which are located inside the Engine installation path, under `Engine/Source/ThirdParty`. Replace the deleted folders with the ones you find at the following [repository](https://github.com/TheEngineRoom-UniGe/RICO-MR-libWebSockets-OpenSSL).

Finally, clone the repository (make sure to clone recursively to download dependant plugins) in your local machine and compile the Visual Studio solution. 

## Usage


