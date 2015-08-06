Horizon SDK for iOS
=======================

<img src="https://horizon.camera/static/sdk/docs/docs/documentation-static/images/horizonsdk-logo.png" alt="Horizon SDK logo" style="width:112px;height:112px;">

Horizon SDK is a state of the art real-time video recording / photo shooting iOS framework.

Some of the features of Horizon SDK include:

* Horizon real-time leveling algorithm (you can find more at [https://horizon.camera](https://horizon.camera))
* Multiple resolutions (up to 2K recording) and frame rate support.
* Custom filter support using the Core Image framework.
* Multiple previews.
* A simple view controller that makes integrating the SDK to your app really easy.

You can register for an account at [https://horizon.camera/sdk](https://horizon.camera/sdk), create an app and receive your API key in order to start developing with Horizon SDK.

This framework can be used on devices running iOS version 7.1 or higher.  You can build your application using Xcode 6 or higher.

For more information take a look at the [Installation](https://horizon.camera/static/sdk/docs/docs/documentation-static/Installation%20Guide.html) and [Quick Start](https://horizon.camera/static/sdk/docs/docs/documentation-static/Quick%20Start.html) guides of the [documentation](https://horizon.camera).

Quick Install Guide
-------------------

* Drag and drop the HorizonSDK/ folder inside your project
* Add **CoreMotion.framework** in your project (**Build Phases > Link Binary With Libraries**)
* Leave **Link Frameworks Automatically** to YES (**Build Settings > Apple LLVM 6.0 - Language - Modules**)
* Add the following in **Other Linker Flags** (**Build Settings > Linking**):
	- `-ObjC` 
	- `-lstdc++` 
* Look at the [Quick Start](https://horizon.camera/static/sdk/docs/docs/documentation-static/Quick%20Start.html) guide to setup your first project.



Examples
--------

There are two Xcode projects in the Examples directory with examples of how to create a simple video recording application as well as a more advanced one. 

* The **HorizonSDKDemo** example app demonstrates the same basic functionality of the HorizonSDK: Creating a preview, instanciating the camera and having a record button. HorizonSDKDemo requires iOS 7.1 or later.

* The **AdvancedHorizonSDKDemo** example app demonstrates a more advanced functionality of the HorizonSDK: There are two previews, a simple and a leveled one. The app responds to device orientation events and demonstrates some of the features available on HorizonSDK. AdvancedHorizonSDKDemo requires iOS 7.1 or later.



Documentation
-------------

You can install the Xcode docset by running the following command from
this directory:

    cp -R com.hvt.Horizon-SDK.docset ~/Library/Developer/Shared/Documentation/DocSets

Then restart Xcode to complete the installation.
