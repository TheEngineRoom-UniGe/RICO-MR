# Robot Intent Communication through Mixed Reality (RICO-MR)
Open-source Unreal Engine 4.27 based project implementing a Mixed Reality layer for enhancing communication in human-robot teams, applicable in a wide variety of interactive tasks.
The project is built using the following UE plugins:
1. [Microsoft OpenXR](https://github.com/microsoft/Microsoft-OpenXR-Unreal)
2. [Mixed Reality UX Tools](https://www.unrealengine.com/marketplace/en-US/product/mixed-reality-ux-tools)

and targets the **Microsoft HoloLens 2** Mixed Reality headsets.

We provide both a binary version of the application, which you can immediately deploy on your devices to start experimenting in different scenarios, or a complete walkthrough on how to import the project in the UE editor if you wish to customize and expand certain functionalities.

## Installation

### Binary Version

Visit the [release page](https://github.com/TheEngineRoom-UniGe/RICO-MR/releases) and download the most recent release of RICO-MR. 

In order to deploy the application onto your HoloLens 2 device, simply follow the procedure described in the following [guide](https://learn.microsoft.com/en-us/windows/mixed-reality/develop/unreal/tutorials/unreal-uxt-ch6#packaging-and-deploying-the-app-via-device-portal). Specifically, refer to the paragraph *Packaging and deploying the app via device portal*, and follow the instructions mentioned at steps 6, 7 and 8.

### Import Project in UE Editor

Before importing the project in UE, make sure to follow the [Introductory Guide](https://learn.microsoft.com/en-us/windows/mixed-reality/develop/unreal/tutorials/unreal-uxt-ch1), which provides step by step instructions on how to setup UE for Mixed Reality development.

Additionally, in order to make sure that packaging the application for HoloLens 2 works successfully, please locate and delete the `libWebSockets` and `OpenSSL` folders, which are located inside the Engine installation path, under `Engine/Source/ThirdParty`. Replace the deleted folders with the ones you find at the following [repository](https://github.com/TheEngineRoom-UniGe/RICO-MR-libWebSockets-OpenSSL).

Finally, clone the repository (make sure to clone recursively to download dependant plugins) in your local machine and compile the Visual Studio solution. 

## Usage

Upon launching the application, a brief *update* screen is displayed. During this time, the application scans remote repositories and downloads any new resources (i.e., robots files), making them subsequently available to the user. Upon completing this process, the user is presented with an appearantly blank screen.

### Interacting with he Menus

Inside the seemingly blank screen, the user is presented with two options:
 1. Rotate and gaze at the palm of the *right* hand to pop up the **Model Selection Menu**. This makes it possible to select robot models from a list of avaiable ones. Simply click on the corresponding icon to *select* a new model to spawn (the spawining process is detailed below). The list is initially pre-loaded with four robot models (i.e., *Baxter*, *Tiago++*, *UR10* and *Panda*), but can be extended by simply uploading all the necessary files (see below) in the custom repository.
 2. Rotate and gaze at the palm of the *left* hand to pop up the **Setting Menu**. Here, users can:
    1) Customize the links pointing at the remote repository where they upload their own robot files to use inside the application. Upon setting the new URL, on each subsequent startup the application will proceed to scan the repository for updates and will download new resources to be selected via **Model Selection Menu**.
    2) Set the *publication rate* for the tracking of the HMD's pose, once initial *handshake* is established as discussed in the following paragraph.

### Spawining Robot Models

Spawining robot models works through the **QR Code Tracking API** offered by Microsoft's OpenXR plugin. Simply generate a QR marker (you can use this [website](https://www.the-qrcode-generator.com/) to generate free textual codes), print it and place it in the environment. Then, once the user has properly selected the robot model via **Model Selection Menu**, simply look at the QR with the HoloLens's camera and wait until the robot hologram is properly spawned at the marker's estimated location. 

To guarantee support for multi-robot scenarios and avoid ambiguities, make sure to employ a different QR code for any robot hologram you wish to spawn in the Mixed Reality scene. To this extent, make sure to generate as many QR codes with different textual contents (e.g., you can simply fill the first marker's content with *robot1*, the second with *robot2* and so on).

Additionally, the QR Code parsing feature makes it possible to embed *fixed offsets* inside the marker's textual content. This makes it possible to place QR codes at *conformable* locations (thus with no need for the user to bend on the ground to *look* at the QR and spawn the robot accordingly). To exploit this functionality, simply embed the offset inside the marker's text, after the robot ID, as in the following example:

`robot1/x:100/y:100/z:100`

Here, the robot's hologram with ID *robot1* will be spawned with a fixed offset of 1 meter (i.e., 100cm) on all three axes with respect to the marker's estimated location. This is extrimely useful if you wish to spawn robot's holograms on the floor without having to physically place the QR on the ground. 

### Uploading New Robot Models on the Remote Repository

TODO


