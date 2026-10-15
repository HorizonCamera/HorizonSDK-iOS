Horizon SDK for iOS
=======================

![Horizon logo](https://horizoncamera.github.io/HorizonSDK-iOS/docs/documentation-static/images/horizonsdk-logo.png)

[![CocoaPods compatible](https://img.shields.io/cocoapods/v/HorizonSDK.svg)](https://cocoapods.org/pods/HorizonSDK)

Horizon SDK is a real-time video recording and photo capture library for iOS.

Features:

* Horizon real-time leveling algorithm (more at [https://horizon.camera](https://horizon.camera))
* Recording up to 4K, at the frame rates each camera supports, in HEVC or H.264
* Every built-in camera: wide, ultra wide, telephoto and front
* Leveled photos, in HEIF or JPEG
* Custom filters using Core Image
* Multiple previews
* A ready-made view controller for quick integration
* Access to the video buffer for further processing or live streaming

To use Horizon SDK in your app you need an API key: [contact us](https://horizon.camera/sdk/). The example apps contain their own API keys.

Horizon SDK supports iOS 15 and later.

Installation
------------

### Swift Package Manager

In Xcode, choose File › Add Package Dependencies, enter `https://github.com/HorizonCamera/HorizonSDK-iOS` and add the `HorizonSDK` library to your app target.

### CocoaPods

```ruby
pod 'HorizonSDK', '~> 3.0'
```

CocoaPods trunk stops accepting new versions on December 2, 2026. Releases after that date ship through Swift Package Manager only.

### Manually

Drag `HorizonSDK/HorizonSDK.xcframework` into your project and set it to **Embed & Sign**. The framework carries its own resources.

Then add `NSCameraUsageDescription`, `NSMicrophoneUsageDescription` (to record sound) and, if you save to Photos, `NSPhotoLibraryAddUsageDescription` to your Info.plist.

Documentation
-------------

The [documentation](https://horizoncamera.github.io/HorizonSDK-iOS/) covers [Installation](https://horizoncamera.github.io/HorizonSDK-iOS/documentation/horizonsdk/installation/), a [Quick Start](https://horizoncamera.github.io/HorizonSDK-iOS/documentation/horizonsdk/quickstart/) and the full API reference.

Upgrading from 2.x
------------------

3.0 needs iOS 15. Objective-C code written for 2.x keeps building: the replaced APIs are deprecated, not removed, and still work in 3.x. Swift code needs the changes below.

**Swift: update these delegate signatures, or they are never called.** Their dictionaries are now typed. With the 2.x signature Swift only warns that the method "nearly matches" the protocol, and the SDK never calls it:

```swift
// HVTCameraDelegate
func hvtCamera(_ hvtCamera: HVTCamera, didStopRecordingWithMetadata metadata: [String : Any])
func hvtCamera(_ hvtCamera: HVTCamera, didCapturePhotoData photo: Data, metadata: [String : Any])
func hvtCamera(_ hvtCamera: HVTCamera, didCapturePhotoImageRef photo: CGImage, metadata: [String : Any])

// HVTCameraControllerDelegate
func cameraController(_ cameraController: HVTCameraController, didFinishCapturingMediaWithInfo info: [String : Any])
```

The other delegate methods now take non-optional parameters (`HVTCamera` instead of `HVTCamera!`). Their 2.x signatures still work, with a warning about optionality.

**Swift: activation throws.** `activate(withAPIKey:)` reports failures as a Swift error instead of an Objective-C exception:

```swift
do {
    try HVTSDK.sharedInstance().activate(withAPIKey: "YOUR_API_KEY")
} catch {
    print("Horizon SDK activation failed: \(error)")
}
```

**Replacements for deprecated APIs:**

| 2.x | 3.0 |
|---|---|
| `activateWithAPIKey:`, `activateWithAPIKey:settings:` | `activateWithAPIKey:error:`, `activateWithAPIKey:settings:error:` |
| `startRunning` | `startRunningWithCompletion:` (does not block; reports why it could not start) |
| `stopRunning` | `stopRunningWithCompletion:` |
| `startRecordingWithMovieURL:` and its variants | `startRecordingWithConfiguration:error:` with `HVTRecordingConfiguration` |
| `captureFullResolutionPhoto`, `captureFullResolutionPhotoUsingHDR:` | `capturePhotoWithSettings:` with `HVTPhotoSettings` |
| `hvtCamera:didCapturePhotoData:metadata:`, `hvtCamera:didCapturePhotoImageRef:metadata:` | `hvtCamera:didCapturePhoto:` with `HVTPhoto` |
| `setCameraPosition:…`, `setCameraResolution:…`, `setCaptureDevice:resolution:…` | `setCaptureDevice:format:frameRate:error:`, with the format from `formatForDevice:resolution:frameRate:` |
| `isCameraPositionSupported:` | `availableDevices` |
| `HVTCameraController.mediaTypes` | `allowedContentTypes` (`UTType`) |

Also new in 3.0: camera interruption reporting (`hvtCamera:wasInterrupted:`, `hvtCameraInterruptionEnded:`), ultra wide and telephoto cameras, HEVC and HEIF, 4K in `HVTCameraController`, and a Metal preview.

Examples
--------

The Examples directory has three Xcode projects. All need iOS 15 or later; to run one on a device, choose your team under Signing & Capabilities.

* **HorizonSDKDemo**: the basics. A preview, a camera and a record button.
* **AdvancedHorizonSDKDemo**: two previews, one plain and one leveled, with camera switching, crop modes, a Core Image filter and aspect fill or fit on double tap.
* **SwiftHorizonSDKDemo**: the basics in Swift.
