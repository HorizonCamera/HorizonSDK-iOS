Horizon SDK for iOS
=======================

![Horizon logo](https://horizoncamera.github.io/HorizonSDK-iOS/docs/documentation-static/images/horizonsdk-logo.png)

Horizon SDK is a state of the art real-time video recording / photo shooting iOS library.

Some of the features of Horizon SDK include:

* Horizon real-time leveling algorithm (you can find more at [https://horizon.camera](https://horizon.camera))
* Support for multiple resolutions (up to 2K recording) and frame rates.
* Custom filter support using the Core Image framework.
* Multiple previews.
* A simple view controller that makes integrating the SDK to your app really easy.
* Access to the video buffer so that it can be further processed or live streamed using other libraries.


In order to use Horizon SDK on your app, you have to receive an API key by [contacting us](https://horizon.camera/sdk/). The provided sample apps contain their own API keys.


The library can be used on devices running iOS version 8 or higher.  You can build your application using the latest Xcode.

For more information take a look at the [Installation](https://horizoncamera.github.io/HorizonSDK-iOS/docs/documentation-static/Installation%20Guide.html) and [Quick Start](https://horizoncamera.github.io/HorizonSDK-iOS/docs/documentation-static/Quick%20Start.html) guides of the [documentation](https://horizoncamera.github.io/HorizonSDK-iOS/).

Examples
--------

There are three Xcode projects in the Examples directory with examples of how to create a simple video recording application, a more advanced use of the library and a Swift project. 

* The **HorizonSDKDemo** example app demonstrates the same basic functionality of the HorizonSDK: Creating a preview, instanciating the camera and having a record button. HorizonSDKDemo requires iOS 8.0 or later.

* The **AdvancedHorizonSDKDemo** example app demonstrates a more advanced functionality of the HorizonSDK: There are two previews, a simple and a leveled one. The app responds to device orientation events and demonstrates some of the features available on HorizonSDK. AdvancedHorizonSDKDemo requires iOS 8.0 or later.

* The **SwiftHorizonSDKDemo** example app provides a simple app written in Swift that shows how to integrate HorizonSDK library with a Swift project. SwiftHorizonSDKDemo requires iOS 8.4 or later.
