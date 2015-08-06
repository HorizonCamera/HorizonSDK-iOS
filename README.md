Horizon SDK for iOS
=======================

This package contains the Horizon iOS SDK. Here's how you will get up and running in no time!

This framework can be used on devices running iOS version 7.1 or higher.  You can build your application using Xcode 6 or higher.

For more information take a look at the Installation and Quick Start guides of the documentation.

Quick Install Guide
-------------------

* Drag and drop the HorizonSDK/ folder inside your project
* Add **CoreMotion.framework** in your project (**Build Phases > Link Binary With Libraries**)
* Leave **Link Frameworks Automatically** to YES (**Build Settings > Apple LLVM 6.0 - Language - Modules**)
* Add the following in **Other Linker Flags** (**Build Settings > Linking**):
	- `-ObjC` 
	- `-lstdc++` 
* Look at the Quick Start guide to setup your first project.



Examples
--------

There are two Xcode projects in the Examples directory with examples of how to create a simple video recording application as well as a more advanced one. 

The HorizonSDKDemo example app demonstrates the same basic functionality of the HorizonSDK: Creating a preview, instanciating the camera and having a record button. HorizonSDKDemo requires iOS 7.1 or later.

The AdvancedHorizonSDKDemo example app demonstrates a more advanced functionality of the HorizonSDK: There are two previews, a simple and a leveled one. The app responds to device orientation events and demonstrates some of the features available on HorizonSDK. AdvancedHorizonSDKDemo requires iOS 7.1 or later.



Documentation
-------------

You can install the Xcode docset by running the following command from
this directory:

    cp -R com.hvt.Horizon-SDK.docset ~/Library/Developer/Shared/Documentation/DocSets

Then restart Xcode to complete the installation.
