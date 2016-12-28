//
//  HVTCamera.h
//  HorizonSDK
//
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

#import "HVTView.h"
#import "HVTVars.h"
#import "HVTCameraController.h"

@protocol HVTCameraDelegate;

@protocol HVTBufferDelegate;

/**
 You can use a HVTCamera object to establish a connection with the device camera, update the attached preview(s)
 and record to an audio/video container file.
 
 When HVTcamera is running, it processes the flow of data from the camera device in real time, by utilizing the
 internal leveler and applying filters. You can alter the leveler's settings using the provided properties. Some
 properties are animated and can be changed even while recording without producing rough transitions in the 
 resulting video.
 
 ####Usage
 
 A simple usage scenario of the class requires the following steps in that particular order:
 
 - Instantiate a HVTCamera object.
 - Set the interfaceOrientation property.
 - (Optional) Implement and set a HVTCameraDelegate object.
 - If you want to display the video preview, you can attach one or more HVTView objects by calling addView:.
 - Invoke startRunning. Make sure you have handled all the camera & microphone permissions first.
 - Invoke startRecordingWithMovieURL: to start recording to a QuickTime movie file.
 - Invoke stopRecording.
 - Invoke stopRunning.
 
 ####Camera resolution
 
 HVTCamera receives video frames from an AVCaptureDevice instance, which can be accessed by the cameraDevice
 property. You can query the supported resolutions using `AVCaptureDevice`'s `formats` property.
 
 By default, the resolution used is the one set in the `AVCaptureDevice`'s `activeFormat` property. If you want to
 set a specific resolution, use one of the methods provided by HVTCamera and **not** the `AVFoundation`'s methods.
 
 If you set the resolution or/and frame rate while HVTCamera isRunning, the connection to the deviceCamera will be reset,
 which can take some time. The call will block until the connection has been established. So set the resolution before
 calling startRunning if possible.
 
 ####Performance Considerations
 
 For performance reasons, the maximum supported resolution and frame rate are 1920X1080 and 60 frames/sec respectively. 
 In some devices, it is recommended to use a lower resolution. For example, prefer 1280X720 in iPhone 4S.
 
 The use of video filters can significantly impact performance.
 
 @warning A single instance of HVTCamera must be running at any time.
 */
@interface HVTCamera : NSObject

#pragma mark Delegate
/**----------------------------------------------------------------------
 * @name Setting the Delegate
 * ----------------------------------------------------------------------
 */

/** The delegate object.
 
 Set a delegate object, in order to get updates on HVTCamera object's status. 
 
 The delegate is not retained.
 
 @see HVTCameraDelegate
 */
@property (nonatomic, weak) id<HVTCameraDelegate> delegate;

/**
 Sets the receiver's delegate that will accept audio & (leveled) video buffers and a dispatch queue on which the delegate will be
 called.
 
 @param sampleBufferDelegate An object conforming to the HVTBufferDelegate protocol that will receive sample buffers.
 @param sampleBufferCallbackQueue A dispatch queue on which all sample buffer delegate method will be called.
 */
- (void)setSampleBufferDelegate:(id<HVTBufferDelegate>)sampleBufferDelegate queue:(dispatch_queue_t)sampleBufferCallbackQueue;

#pragma mark Configuration
/**----------------------------------------------------------------------
 * @name Configuration
 * ----------------------------------------------------------------------
 */

/** 
 Set this property to the current orientation of the app's user interface.
 
 HVTCamera will adjust the orientation of the attached [HVTView(s)](HVTView) and the angle returned by
 [HVTCameraDelegate hvtCamera:didUpdateParams:] to match the supplied interface orientation.
 
 If your app supports multiple orientations, subscribe to the `UIDeviceOrientationDidChangeNotification`
 and set the property as shown in the following code fragment:

 For iOS 8 devices:
 
    // When you receive a UIDeviceOrientationDidChangeNotification:
    _camera.interfaceOrientation = [[UIApplication sharedApplication] statusBarOrientation];
 
 For iOS 7 devices:

    // When you receive a UIDeviceOrientationDidChangeNotification:
     UIDeviceOrientation deviceOrientation = [UIDevice currentDevice].orientation;
 
     if (deviceOrientation == UIDeviceOrientationPortrait
            || deviceOrientation == UIDeviceOrientationLandscapeLeft
            || deviceOrientation == UIDeviceOrientationLandscapeRight) {
        _camera._hvtCameraInstance.interfaceOrientation = (UIInterfaceOrientation)deviceOrientation;
     }
 
 The default value is `UIInterfaceOrientationPortrait`.
 */
@property (nonatomic) UIInterfaceOrientation interfaceOrientation;

#pragma mark Running State
/**----------------------------------------------------------------------
 * @name Managing Running State
 * ----------------------------------------------------------------------
 */

/**
 Indicates whether the receiver is running.
 
 @return `YES` if the receiver is running and `NO` if it is stopped.
 */
- (BOOL)isRunning;

/**
 Tells the receiver to start running.
 
 Starts the flow of data from the camera device. If [HVTView(s)](HVTView) have been added, they will receive 
 updates when new video frames arrive. You can also start video recording.
 
 If the receiver stops running due to an error, it will call the delegate's 
 [HVTCameraDelegate hvtCamera:didStopRunningWithError:] method.
 
 This call blocks until the receiver has completely started up.
 */
- (void)startRunning;

/**
 Tells the receiver to stop running.
 
 Call this method to conserve battery life if you don't need to update the attached [HVTView(s)](HVTView)
 or if you are not recording.
 
 You don't need to call this explicitly when the apps goes to background. The HVTCamera instance will stop running automatically
 and resume upon entering foreground.
 
 This call blocks until the receiver has completely stopped.
 */
- (void)stopRunning;

#pragma mark Video Preview
/**----------------------------------------------------------------------
 * @name Previewing the Video
 * ----------------------------------------------------------------------
 */

/**
 Attaches an HVTView in order to display a video preview.
 
 @param view The HVTView which will be added.
 
 @see HVTView
 */
- (void)addView:(HVTView*)view;

/**
 Removes a HVTView.
 
 @param view The HVTView which will be removed.
 
 @see HVTView
 */
- (void)removeView:(HVTView*)view;

#pragma mark Recording Video
/**----------------------------------------------------------------------
 * @name Recording Video
 * ----------------------------------------------------------------------
 */

/** 
 Specifies the output movie size of the final video file.
 
 The outputMovieSize can differ from the cameraResolution. For example, you can have a 1920x1080
 camera resolution and a 640x640 final video file.
 
 The default value matches the cameraResolution, until set explicetely via this property.
 
 outputMovieSize cannot be set if the captureMode is set to HVTCaptureModePhoto.
 */
@property (nonatomic) CGSize outputMovieSize;

/**
 Reports whether the instance is currently recording video or not.
 
 @return `YES` if video recording occures and `NO` if it isn't.
 */
- (BOOL)isRecording;

/**
 Starts the video recording.
 
 You must call this method only if the instance isRunning, or an exception is thrown.
 
 @param movieURL The URL of the output file in which the video will be saved.
 */
- (void)startRecordingWithMovieURL:(NSURL*)movieURL;

/**
 Starts the video recording.
 
 You must call this method only if the instance isRunning, or an exception is thrown.
 
 @param movieURL The URL of the output file in which the video will be saved.
 @param videoSettings The settings used for video encoding.
 
 
 See AVVideoSettings.h for more information on how to construct an output settings dictionary.
 If you only require simple preset-based output settings, see AVOutputSettingsAssistant.
 
 Can be `nil`.
 @param audioSettings The settings used for audio encoding.
 
 See AVAudioSettings.h for more information. If you only require simple preset-based output settings, 
 see AVOutputSettingsAssistant.
 
 Can be `nil`.
 
 @param metadata NSArray of AVMetadataItems that are to be written to the output file by the export session.
 
 If the value of this key is nil, any existing metadata in the exported asset will be translated as accurately as possible into
 the appropriate metadata keyspace for the output file and written to the output.
 
 Can be `nil`
 */
- (void)startRecordingWithMovieURL:(NSURL*)movieURL videoSettings:(NSDictionary*)videoSettings audioSettings:(NSDictionary*)audioSettings metadata:(NSArray*)metadata;

/**
 Stops the video recording.
 */
- (void)stopRecording;

/**
 Pauses the video recording.
 */
- (void)pauseRecording;

/**
 Resumes the video recording.
 */
- (void)resumeRecording;

/**
 Reports whether the recording is currently paused
 
 @return `YES` if video recording is paused and `NO` if it isn't.
 */
- (BOOL)isPaused;

/**
 Returns the video bitrate of the currently active recorder
 
 If the video recorder is not active, this method returns 0.
 
 @return The video bitrate.
 */
- (int)recorderVideoBitRate;

/**
 Returns the audio bitrate of the currently active recorder
 
 If the video recorder is not active, this method returns 0.
 
 @return The audio bitrate.
 */
- (int)recorderAudioBitRate;


#pragma mark Photo Capture
/**----------------------------------------------------------------------
 * @name Managing Photo Capture
 * ----------------------------------------------------------------------
 */

/**
 Handles the still image frame for photo mode
 
 @see HVTStillImageOrientation
 */
@property (nonatomic) HVTStillImageOrientation stillImageOrientation;

/**
 Sets the GPS metadata that can be used for photos
 
 @param gpsMetadata The dictionary containing GPS coordinates, conforming kCGImagePropertyGPSDictionary
 */
- (void)setGPSMetadata:(NSDictionary*)gpsMetadata;

/**
 Shoots a full resolution photo.
 
 If used while recording then you will receive a HVTErrorPhotoModeNotEnabled error by the
 hvtCamera:photoCapturingDidFailWithError: delegate method.
 
 While recording, you can use the captureSnapshot method to take photos.
 
 @return `NO` if another photo capture is in progress or if photo capturing is not supported and `YES` otherwise
 */
- (BOOL)captureFullResolutionPhoto;

/**
 Shoots a full resolution photo.
 
 If used while recording then you will hear the shutter sound. 
 
 If used while recording then you will receive a HVTErrorPhotoModeNotEnabled error by the
 hvtCamera:photoCapturingDidFailWithError: delegate method.
 
 While recording, you can use the captureSnapshot method to take photos.
 
 @param usingHDR Capture HDR photo
 
 @return `NO` if another photo capture is in progress and `YES` otherwise
 */
- (BOOL)captureFullResolutionPhotoUsingHDR:(BOOL)usingHDR;

/**
 Shoots a snapshot photo, right from the video stream. It can be used while recording
 
 @return `NO` if another photo capture is in progress and `YES` otherwise
 */
- (BOOL)captureSnapshot;

/**
 Returns if the device can capture full resoltion still image HDR photos
 
 @return `YES` if HDR photo capture is available, `NO` otherwise
 */
- (BOOL)isHDRPhotoCaptureAvailable;

#pragma mark Camera Settings
/**----------------------------------------------------------------------
 * @name Managing the Camera Settings
 * ----------------------------------------------------------------------
 */

/** The currently active AVFoundation device.
 
 You can change the camera device properties by first calling the AVCaptureDevice's lockForConfiguration: method.
 
 @warning If you want to alter the camera resolution or framerate, do not alter the activeFormat, activeVideoMinFrameDuration or activeVideoMaxFrameDuration properties of the `AVFoundation` device. Instead, use the methods supplied by HVTCamera. */
@property (nonatomic, readonly) AVCaptureDevice *cameraDevice;

/**
 The capture mode used by the camera.
 
 The two capture modes are listed in the HVTCaptureMode enumeration. The default value is HVTCaptureModeVideo.
 
 Capture mode changes automatically to HVTCaptureModeVideo value when setCameraResolution:error: or setCameraResolution:frameRate:error: methods are called
 
 @see HVTCaptureMode
 */
@property (nonatomic) HVTCaptureMode captureMode;

/**
 Sets a new capture mode with a specified camera position.
 
 @param captureMode The new capture mode.
 @param newPosition The new camera position.
 
 @see HVTCaptureMode
 */
- (void)setCaptureMode:(HVTCaptureMode)captureMode cameraPosition:(AVCaptureDevicePosition)newPosition;

/** The current flash mode
 
 You can set or query the current flash mode using this property. Horizon SDK takes care of the availability and the separation between torch/flash for you
 */
@property (nonatomic) AVCaptureFlashMode flashMode;

/** The pixel format used for the exported videos and frames by the hvtCamera:didOutputSampleBuffer: method.
 
 The default value is kCVPixelFormatType_32BGRA. 
 
 You can enter only the following two values for the outputPixelFormat:
 
 kCVPixelFormatType_32BGRA and kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange
 */
@property (nonatomic) FourCharCode outputPixelFormat;

/**
 The brightness value, indicating how dark or bright the scene currently is.
 
 The value can be negative (for dark scenes) or positive (for bright scenes) and
 is updated in real-time.
 
 You can observe the changes to this value using KVO.
 
 e.g.
 
    [self addObserver:self
            forKeyPath:@"_camera.brightnessValue"
                options:(NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew)
                context:(__bridge void *)(self)];
 */
@property (nonatomic, readonly) float brightnessValue;

/** 
 Clears any focus/exposure/white balance setting that may have been applied by the user, returning them to AVCaptureFocusModeContinuousAutoFocus, AVCaptureExposureModeContinuousAutoExposure and AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance values
 */
- (void)resetFocusExposureWhiteBalance;

/**
 Indicated whether the AVCaptureDevicePosition in question is supported by the current device.
 
 This is a class method so you can use it without first instanciating the HVTCamera object.
 
 @param cameraPosition The camera position in question.
 
 @return YES if device supports the said AVCaptureDevicePosition, NO otherwise
 */
+ (BOOL)isCameraPositionSupported:(AVCaptureDevicePosition)cameraPosition;

/**
 Indicates whether a given camera has flash illumination capability.
 
 To get notified on changes on the property, subscribe to receive NSNotifications with HVTFlashIsAvailableNotification name.
 
 @return YES if device has flash and it is available in HVTCaptureModePhoto or if it has torch and it is available in HVTCaptureModeVideo, NO otherwise
 */
- (BOOL)isFlashAvailable;

/**
 Reports the camera resolution for the current cameraDevice.
 
 @return The camera resolution as a CGSize struct.
 */
- (CGSize)cameraResolution;

/**
 Sets the camera resolution of the current cameraDevice. Frame rate is set to 30 frames/sec.
 
 @warning If you call the method while the receiver isRunning, the connection to the deviceCamera will be reset, which 
 can take some time. The call will block until the connection has been established.
 
 @param cameraResolution The new camera resolution.
 @param outError The output error.
 
 @return `YES` if the camera resolution has been changed and `NO` if there was an error while changing the camera resolution (check outError for more).
 */
- (BOOL)setCameraResolution:(CGSize)cameraResolution error:(NSError **)outError;

/**
 Sets the camera resolution and frame rate of the current cameraDevice.
 
 @warning If you call the method while the receiver isRunning, the connection to the deviceCamera will be reset, which
 can take some time. The call will block until the connection has been established.
 
 @param cameraResolution The new camera resolution.
 @param frameRate The new frame rate.
 @param outError The output error.
 
 @return `YES` if the camera resolution has been changed and `NO` if there was an error while changing the camera 
 resolution (check outError for more).
 */
- (BOOL)setCameraResolution:(CGSize)cameraResolution frameRate:(int)frameRate error:(NSError **)outError;

/**
 Reports the camera position of the current cameraDevice.
 
 @return The position of the camera as a value of AVCaptureDevicePosition.
 */
- (AVCaptureDevicePosition)cameraPosition;

/**
 Selects a cameraDevice having the supplied camera position.
 
 The camera resolution and frame rate of the cameraDevice is set to its default value or to its previous value if
 it was explicetely set before.
 
 @warning If you call the method while the receiver isRunning, a connection to the new cameraDevice will be established, which
 can take some time. The call will block until the connection has been established.
 
 @param newPosition The new camera position.
 @param outError The output error.
 
 @return `YES` if the camera position has been changed and `NO` if there was an error while changing the camera position 
 (check outError for more).
 */
- (BOOL)setCameraPosition:(AVCaptureDevicePosition)newPosition error:(NSError **)outError;

/**
 Selects a cameraDevice having the supplied camera position and sets its resolution. Frame rate is set to 30 frames/sec.
 
 @warning If you call the method while the receiver isRunning, a connection to the new cameraDevice will be established, which
 can take some time. The call will block until the connection has been established.
 
 @param newPosition The new camera position.
 @param cameraResolution The new camera resolution.
 @param outError The output error.
 
 @return `YES` if the camera resolution has been changed and `NO` if there was an error while changing the camera resolution 
 (check outError for more).
 */
- (BOOL)setCameraPosition:(AVCaptureDevicePosition)newPosition withResolution:(CGSize)cameraResolution error:(NSError **)outError;

/**
 Selects a cameraDevice having the supplied camera position and sets its resolution and frame rate.
 
 @warning If you call the method while the receiver isRunning, a connection to the new cameraDevice will be established, which
 can take some time. The call will block until the connection has been established.
 
 @param newPosition The new camera position.
 @param cameraResolution The new camera resolution.
 @param frameRate The new frame rate.
 @param outError The output error.
 
 @return `YES` if the camera resolution has been changed and `NO` if there was an error while changing the camera resolution 
 (check outError for more).
 */
- (BOOL)setCameraPosition:(AVCaptureDevicePosition)newPosition withResolution:(CGSize)cameraResolution frameRate:(int)frameRate error:(NSError **)outError;

#pragma mark Video Stabilization Settings
/**----------------------------------------------------------------------
 * @name Managing the Video Stabilization Settings
 * ----------------------------------------------------------------------
 */

/**
 Returns whether the current cameraDevice supports the given video stabilization mode.
 
 This methdod is a wrapper of `AVCaptureDeviceFormat`'s `isVideoStabilizationModeSupported:` method. It will not
 work on an iOS7 runtime.
 
 @param videoStabilizationMode An AVCaptureVideoStabilizationMode to be checked.
 @return A boolean value indicating whether the current  cameraDevice can be stabilized by setting
 preferredVideoStabilizationMode to the given mode.
 .
 */
- (BOOL)isVideoStabilizationModeSupported:(AVCaptureVideoStabilizationMode)videoStabilizationMode;

/**
 Indicates the prefered stabilization mode to apply when supported by the current cameraDevice.
 
 This property is a wrapper of `AVCaptureConnection`'s `preferredVideoStabilizationMode` property for iOS8 devices.
 In iOS7 devices, it sets `AVCaptureConnection`'s `setEnablesVideoStabilizationWhenAvailable` to `YES` when the
 preferredVideoStablizationMode is anything but `AVCaptureVideoStabilizationModeOff`.
 
 The default value is `AVCaptureVideoStabilizationModeOff`.
 
 @warning If you set the property while the receiver isRunning, the connection to the deviceCamera will be reset, which
 can take some time. The call will block until the connection has been established.
 */
@property (nonatomic) AVCaptureVideoStabilizationMode preferredVideoStabilizationMode;

/**
 Maximum recording duration set by the user. If set and the recording gets past this duration, then it is automatically stopped.
 
 Every recording made after setting this property to a valid CMTime will respect the maxRecordingDuration
 In order to return to unlimited recording, user can set this property value to kCMTimeInvalid
 */
@property (nonatomic) CMTime maxRecordingDuration;

#pragma mark Leveler Settings
/**----------------------------------------------------------------------
 * @name Managing the Leveler Settings
 * ----------------------------------------------------------------------
 */

/** Specifies which crop mode will be used by the leveler.
 
 The leveler crops each video frame so that the resulting frame is leveled.  This involves rotating 
 and scaling the crop region, prior to cropping the frame, according to the device's physical
 orientation at that time. The crop mode is the way the leveler adjusts the rotation and scale of
 the crop region.
 
 Note that scaling the crop region equals to zooming in the original video frame.
 
 The property is animated.
 
 The default value is `HVTLevelerCropModeFlex`.
 
 @see HVTLevelerCropMode
 */
@property (nonatomic) HVTLevelerCropMode levelerCropMode;

/** Specifies how fast the leveler zooms when HVTLevelerCropMode is set to `HVTLevelerCropModeFlex`.
 
 The default value is `HVTLevelerFlexSpeedSmooth`.
 
 @see HVTLevelerFlexSpeed
 */
@property (nonatomic) HVTLevelerFlexSpeed levelerFlexSpeed;

/** Informs the leveler of the expected device orientation when HVTLevelerCropMode is set 
 to `HVTLevelerCropModeLocked`.
 
 Since leveling is disabled when crop mode is `HVTLevelerCropModeLocked`, this property should match the device's physical 
 orientation. For example, if the user is intented to hold the device vertically, use `HVTLevelerLockedOrientationVertical`.
 However, if the user decides to turn the device horizontally, the recorded video's orientation will be wrong.
 
 The property is animated.
 
 The default value is `HVTLevelerLockedOrientationAuto`.
 
 @warning This does **not** control whether the recorded video will be landscape or portrait. This depends on the
 aspect ratio of outputMovieSize.
 
 @see HVTLevelerLockedOrientation
 */
@property (nonatomic) HVTLevelerLockedOrientation levelerLockedOrientation;

#pragma mark Video Filters
/**----------------------------------------------------------------------
 * @name Managing Video Filters
 * ----------------------------------------------------------------------
 */

/**
 Sets a custom CIFilter to be applied on the camera stream.
 
 The active filter is applied after the Leveler corrects the video orientation.
 
 @param filter The CIFilter to be applied
 */
- (void)setActiveFilter:(CIFilter*)filter;

/**
 Removes the active filter
 */
- (void)removeActiveFilter;

@end

/**
 The HVTBufferDelegate protocol defines the delegate method to be implemented in order
 to receive updates for the leveled sample buffers sent from the HVTCamera instance.
 
 The delegate method are invoked in the queue defined from setSampleBufferDelegate:queue: method.
 */
@protocol HVTBufferDelegate <NSObject>

@required

/**----------------------------------------------------------------------
 * @name Sample Buffer Delegate Method
 * ----------------------------------------------------------------------
 */

/**
 Implement this method to be notified when you receive a CMSampleBuffer from the delegate in order to
 process it further to your own pipeline.
 
 This method is not being dispatched to the main thread as opposed to the other delegate methods but to the dispatch queue
 set by the setSampleBufferDelegate:queue: method.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param sampleBuffer The CMSampleBuffer received.
 */
- (void)hvtCamera:(HVTCamera *)hvtCamera didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer;

@end

/**
 The HVTCameraDelegate protocol defines delegate methods you should implement in order
 to get updates from an HVTCamera instance about the running state, recording status, handle errors and update the UI.
 
 All the delegate methods are invoked in the main thread.
 */
@protocol HVTCameraDelegate <NSObject>

/** The NSURL path to the recorded video file.
 
 This key is used in the metadata NSDictionary returned by hvtCamera:didStopRecordingWithMetadata:
 */
extern NSString *const HVTMetadataMovieURLKey;
/** The NSTimeInterval duration of the recorded video file
 
 This key is used in the metadata NSDictionary returned by hvtCamera:didStopRecordingWithMetadata:
 */
extern NSString *const HVTMetadataMovieDurationKey;
/** The NSValue (CGSize) dimensions of the captured photo
 
 This key is used in the metadata NSDictionary returned by hvtCamera:didCapturePhotoData:metadata: hvtCamera:didCapturePhotoImageRef:metadata:
 */
extern NSString *const HVTMetadataPhotoDimensions;
/** The NSDictionary containing the original metadata of the captured image
 
 This key is used in the metadata NSDictionary returned by hvtCamera:didCapturePhotoData:metadata: hvtCamera:didCapturePhotoImageRef:metadata:
 */
extern NSString *const HVTMetadataOriginalPhotoMetadata;

@required

/**----------------------------------------------------------------------
 * @name Interface Delegate Method
 * ----------------------------------------------------------------------
 */

/**
 Implement this method to be notified of the updated device angle and computed scale factors in order to
 adjust your interface elements (i.e. rotate your UI elements).
 
 The angle represents the yaw of the device (in the real world) at the time the
 latest video frame was received by the device's camera.
 
 The scale represents the scale that has to be applied to the frame depending on the active levelerCropMode
 and the angle of the device.
 
 You may want to rotate your UI elements to match the attached HVTView(s) appearence 
 as illustrated in the following code fragment:
 
    - (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateParams:(HVTParams)params
    {
        [_uiElement setTransform:CGAffineTransformMakeRotation(params.angle)];
    }
 
 The calling frequency matches the frame rate of the HVTCamera instance.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param params The angle (physical yaw) of the device in rads and scale values.
 */
- (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateParams:(HVTParams)params;

@optional

/**----------------------------------------------------------------------
 * @name Running State Delegate Method
 * ----------------------------------------------------------------------
 */

/**
 Called when HVTcamera starts running.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 */
- (void)hvtCameraDidStartRunning:(HVTCamera *)hvtCamera;

/**
 Called when HVTcamera stops running due to an error.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param error The error occured.
 */
- (void)hvtCamera:(HVTCamera *)hvtCamera didStopRunningWithError:(NSError *)error;

/**----------------------------------------------------------------------
 * @name Recording Delegate Methods
 * ----------------------------------------------------------------------
 */

/**
 Called when video recording has begun.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 */
- (void)hvtCameraRecordingDidStart:(HVTCamera*)hvtCamera;

/**
 Implement this method to get the updated recording duration in order to update duration counters
 in your interface.
 
 This method is called for every recorded frame when recording a video.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param duration The video duration up until this point as a NSTimeInterval variable.
 */
- (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateRecordingDuration:(NSTimeInterval)duration;

/**
 Called when video recording is going to stop.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 */
- (void)hvtCameraRecordingWillStop:(HVTCamera*)hvtCamera;

/**
 Called when video recording has stopped successfully. You can access the recorded
 video's properties through the `metadata` NSDictionary.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param metadata The NSDictionary containing information of the recorded video.
 */
- (void)hvtCamera:(HVTCamera*)hvtCamera didStopRecordingWithMetadata:(NSDictionary*)metadata;

/**
 Called when video recording has stopped due to an error.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param error The error occured
 */
- (void)hvtCamera:(HVTCamera*)hvtCamera recordingDidFailWithError:(NSError *)error;

/**----------------------------------------------------------------------
 * @name Photo Capturing Delegate Methods
 * ----------------------------------------------------------------------
 */

/**
 Called when a photo has been captured
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param photo The photo captured as NSData (JPEG representation)
 @param metadata The photo metadata (already passed inside the NSData structure)
 */
- (void)hvtCamera:(HVTCamera*)hvtCamera didCapturePhotoData:(NSData *)photo metadata:(NSDictionary*)metadata;

/**
 Called when a photo has been captured
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param photo The photo captured as CGImageRef
 @param metadata The photo metadata
 */
- (void)hvtCamera:(HVTCamera *)hvtCamera didCapturePhotoImageRef:(CGImageRef)photo metadata:(NSDictionary *)metadata;

/**
 Called when photo capturing has not been completed successfully due to an error.
 
 @param hvtCamera The HVTCamera object calling the delegate method.
 @param error The error occured
 */
- (void)hvtCamera:(HVTCamera*)hvtCamera photoCapturingDidFailWithError:(NSError *)error;

@end
