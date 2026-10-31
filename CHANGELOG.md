# Horizon SDK 3.1

Stereo sound, 16:9 photos, recordings that survive a crash, and fixes to photo mode.

## New

- **Stereo sound.** `HVTCamera.audioChannels` records left and right from the built-in microphones (`HVTAudioChannelsStereo`). It needs iOS 18 or later, a device whose microphones support it, and microphone access; check `supportsStereoAudio`. Elsewhere the sound stays mono. The default is still mono, and a change applies from the next `startRunning` or capture mode change.
- **16:9 photos.** `HVTCamera.photoAspectRatio` set to `HVTPhotoAspectRatio16x9` (Swift: `.ratio16x9`) takes photos in 16:9, cropped from the full width of the camera's 4:3, and the preview shows the same shape. The default, `HVTPhotoAspectRatioSensor`, keeps the camera's shape. Video mode is unchanged: photos there follow `outputMovieSize`.

## Fixed

- **A recording cut short is no longer lost.** QuickTime recordings had no index until the recording finished, so a take ended by a crash, the app being killed or the battery running out left a file that would not open. They are now written in 10 s fragments: a take cut short plays, losing at most its last few seconds. Files are 2-4% larger (measured on a 60 s 4K take). `.mp4` recordings are unchanged.
- **Photo dates follow the Gregorian calendar.** EXIF `DateTimeOriginal` and `DateTimeDigitized` were formatted with the user's calendar and clock settings: a phone on the Japanese calendar wrote the year as `0008`, a Buddhist one as `2569`. They are now always written in the EXIF format.
- **Photo mode crops from the camera's real shape.** In photo mode the leveler had no camera shape to work from, so 4:3 photo sizes varied between runs on the same device.
- **No white flash when the phone turns in photo mode.** With `HVTStillImageOrientationAuto`, the frame where the photo switched between landscape and portrait was drawn with the zoom of the old shape. Turning to landscape, the HUD showed white past the camera image for that frame, and on the first frame after start.
- **Stereo input is recorded in stereo.** QuickTime recordings without app audio settings used the audio settings from when the session was set up, which could record a stereo input as one channel.
- **`cameraResolution` in photo mode.** It reported 0x0 when the camera started in photo mode, and the last video format after a switch from video mode. It now reports the format the camera runs.

## Installation

- **Swift Package Manager**: `https://github.com/HorizonCamera/HorizonSDK-iOS`, from 3.1.
- **CocoaPods**: `pod 'HorizonSDK', '~> 3.1'`.

# Horizon SDK 3.0.2

A packaging fix, with no API or behavior changes.

## Fixed

- **Debug symbols ship with the framework.** Each XCFramework slice now carries its dSYM. Archiving an app that uses the SDK no longer reports "Upload Symbols Failed" for `HorizonSDK.framework` on App Store Connect upload, and SDK frames in crash reports show function names and line numbers instead of addresses.

## Installation

- **Swift Package Manager**: `https://github.com/HorizonCamera/HorizonSDK-iOS`, from 3.0.2.
- **CocoaPods**: `pod 'HorizonSDK', '~> 3.0'`.

# Horizon SDK 3.0.1

A fix release, with no API changes.

## Fixed

- **Crop mode changes are quicker.** Switching between Flex, Rotate and Locked eased the zoom and angle over 0.7-0.8 s, and Locked to Flex took over 2 s while Flex's zoom smoothing caught up. Every change now settles in about a third of a second.
- **Crop mode changes animate smoothly.** `HVTView` drew its crop rectangle only when a camera frame arrived, so a mode change stepped at the camera's frame rate (under 20 fps in dim light). During a change the view now redraws at the screen's rate. Leveled views still update once per camera frame.

## Installation

- **Swift Package Manager**: `https://github.com/HorizonCamera/HorizonSDK-iOS`, from 3.0.1.
- **CocoaPods**: `pod 'HorizonSDK', '~> 3.0'`.

# Horizon SDK 3.0

A modernization release: iOS 15+, Swift Package Manager, a Metal preview, new capture APIs, and more than 50 fixes for crashes, hangs and wrong output.

## Requirements

- iOS 15 or later. Older devices and iOS versions stay on 2.8.
- arm64 devices; the simulator slice is arm64 and x86_64. armv7 and i386 are gone.
- The SDK ships as a signed XCFramework (Apple Distribution: PZ MOBILE LIMITED).

## Installation

- **Swift Package Manager** (new): add `https://github.com/HorizonCamera/HorizonSDK-iOS`.
- **CocoaPods**: `pod 'HorizonSDK', '~> 3.0'`. CocoaPods trunk stops accepting new versions on December 2, 2026, so later releases ship through Swift Package Manager only.
- **Carthage** is no longer supported.
- The framework now carries its own resources and privacy manifest; nothing else needs copying.

## Upgrading from 2.x

Objective-C code written for 2.x keeps building: replaced APIs are deprecated, not removed.

**Swift: update these delegate signatures, or they are never called.** Their dictionaries are now typed `[String : Any]`. With the 2.x signature, Swift only warns that the method "nearly matches" the protocol:

- `hvtCamera(_:didStopRecordingWithMetadata:)`
- `hvtCamera(_:didCapturePhotoData:metadata:)`
- `hvtCamera(_:didCapturePhotoImageRef:metadata:)`
- `cameraController(_:didFinishCapturingMediaWithInfo:)`

**Swift: activation throws.** Use `try HVTSDK.sharedInstance().activate(withAPIKey:)`; failures are now Swift errors instead of Objective-C exceptions.

See the [README](README.md#upgrading-from-2x) for the full table of replacements.

## New

- **Non-blocking start and stop**: `startRunningWithCompletion:` and `stopRunningWithCompletion:` configure the camera off the main thread and report why it could not start, e.g. camera access denied.
- **Recording configuration**: `HVTRecordingConfiguration` with `startRecordingWithConfiguration:error:` validates the URL and settings up front and supports HEVC and MPEG-4.
- **Photo settings**: `capturePhotoWithSettings:` with `HVTPhotoSettings` (per-shot flash, quality prioritization, HDR bracket, HEIF or JPEG). Photos arrive as `HVTPhoto` in `hvtCamera:didCapturePhoto:`, including snapshots.
- **Every built-in camera**: `availableDevices` lists the wide, ultra wide, telephoto, multi-lens and TrueDepth cameras.
- **Exact format and frame rate**: `setCaptureDevice:format:frameRate:error:` takes a format and frame rate, fractional rates such as 29.97 included. `formatForDevice:resolution:frameRate:` finds a format for a resolution.
- **Interruption reporting**: `hvtCamera:wasInterrupted:` gives the reason (background, iPad multitasking, another app, system pressure), and `hvtCameraInterruptionEnded:` reports the end.
- **iPad multitasking**: the camera keeps running in Split View and Stage Manager where the system allows it.
- **4K** in `HVTCameraController`, which now takes `UTType` content types (`allowedContentTypes`).
- **Errors instead of exceptions**: `activateWithAPIKey:error:` and the other new calls return errors; nothing new throws.
- **Swift-friendly headers**: nullability, generics and typed errors (`HVTErrorDomain` as a Swift error type).
- **Documentation**: new DocC documentation with Installation and Quick Start guides.

## Improved

- **Metal preview** replaces OpenGL ES. It holds the frame rate (60 fps tested) with less main-thread work, and one shared Core Image context replaces a context per renderer.
- **Recording**: starts with lower latency, records audio at every resolution (4K included) without the old separate audio recorder, and keeps audio and video in sync.
- **Recording settings**: color is tagged correctly (BT.709), and keyframe intervals follow the actual frame rate.
- **Leveling** is smoother at the edges of the motion data.
- **First photo**: the first photo after the camera starts waits for exposure as well as focus, so it's no longer dark or tinted.
- **Brightness**: `brightnessValue` change notifications arrive on the main thread.
- **Audio session**: the app's audio session category is restored after recording.
- **Temporary files** no longer go to the user-visible, iCloud-backed Documents folder.
- **Size**: the boost dependency is gone, so the SDK is much smaller.

## Fixed

- **Crashes**:
  - HDR photos with fewer than three brackets.
  - Frames without attachments.
  - Missing pixel buffer pools.
  - Recording started before the camera's first frame.
  - Recording to a nil URL.
  - Every camera start on iOS 15.
- **Hangs**:
  - A deadlock when switching cameras while a paused recording races a session interruption.
  - The main thread blocked while the capture session started or stopped.
- **Background tasks**: background tasks always end, the camera no longer holds one for as long as it runs in the foreground, and `stopRunning` works when no background task exists.
- **Orientation**:
  - The preview is upright when an iPad app launches in landscape.
  - The level angle is correct upside down.
- **Camera switching**: switching cameras while a recording is paused now applies the new camera's format and frame rate.
- **Errors**: delegates receive the actual error, and functions that report failure return `NO` when no error pointer is passed.
- **Free-license**: the resolution cap and preview count are enforced, and the watermark loads from the framework.
- **Photo settings**: per-shot settings no longer leak into later captures.
- **Focus**: a stuck focus wait no longer blocks every later capture.
- **Exports**: exports succeed when the target file already exists, and use the video's own duration.
- **Filters**: filters render correctly on the preview, recording and photos at the same time.
- **Memory**: a retain cycle that kept the camera alive is gone.

## Examples

The three example apps now target iOS 15, use the 3.0 APIs, and rotate correctly on iPad.
