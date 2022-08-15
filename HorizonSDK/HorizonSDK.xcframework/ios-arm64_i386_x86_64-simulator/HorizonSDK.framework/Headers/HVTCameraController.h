//
//  HVTCameraController.h
//  HorizonSDK
//
//  Created by Stelios Petrakis on 7/24/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol HVTCameraControllerDelegate;

/**
 Video quality settings for movies recorded with the built-in camera.
 
 Essentially those are different resolutions that you can select from before presenting the Camera Controller.
 
 The constants in this enumeration are for use as values of the videoQuality property.
 
 */
typedef NS_ENUM(NSInteger, HVTCameraControllerQualityType) {
/** 640x480 resolution.*/
    HVTCameraControllerQualityType640x480   = 0,
/** 1280x720 resolution.*/
    HVTCameraControllerQualityType1280x720  = 1,
/** 1920x1080 resolution.*/
    HVTCameraControllerQualityType1920x1080 = 2,
/** 720x1280 (vertical) resolution.*/
    HVTCameraControllerQualityType720X1280  = 3,
/** 1080x1920 (vertical) resolution.*/
    HVTCameraControllerQualityType1080x1920 = 4
};

/**
 The category of media for the camera to capture.
 
 The constants in this enumeration are for use as values of the cameraCaptureMode property.
 */
typedef NS_ENUM(NSInteger, HVTCameraControllerCameraCaptureMode) {
/** Specifies that the camera captures still images.*/
    HVTCameraControllerCameraCaptureModePhoto,
/** Specifies that the camera captures movies.*/
    HVTCameraControllerCameraCaptureModeVideo
};

/**
 The camera to use for image or movie capture.
 
 The constants in this enumeration are for use as values of the cameraDevice property.
 */
typedef NS_ENUM(NSInteger, HVTCameraControllerCameraDevice) {
/** Specifies the camera on the rear of the device.*/
    HVTCameraControllerCameraDeviceRear,
/** Specifies the camera on the front of the device.*/
    HVTCameraControllerCameraDeviceFront
};

/**
 The flash mode to use with the active camera.
 
 The constants in this enumeration are for use as values of the cameraFlashMode property.
 
 The behavior of the flash depends on the camera capture mode.
 
 For a cameraCaptureMode value of HVTCameraControllerCameraCaptureModePhoto, flash is used to transiently illuminate the subject during still image capture.
 
 For a cameraCaptureMode value of HVTCameraControllerCameraCaptureModeVideo, flash is used to continuously illuminate the subject during movie capture.
 
 For a given camera on a device, flash may or may not be available. You specify the active camera by way of the cameraDevice property. You can determine if the active camera has flash available by calling the isFlashAvailableForCameraDevice: class method.
 
 You can manipulate the flash directly to provide effects such as a strobe light. Present the interface set to use video capture mode. Then, turn the flash LED on or off by setting the cameraFlashMode property to HVTCameraControllerCameraFlashModeOn or HVTCameraControllerCameraFlashModeOff.
 */
typedef NS_ENUM(NSInteger, HVTCameraControllerCameraFlashMode) {
/** Specifies that flash illumination is always off, no matter what the ambient light conditions are.*/
    HVTCameraControllerCameraFlashModeOff  = -1,
/** Specifies that the device should consider ambient light conditions to automatically determine whether or not to use flash illumination.*/
    HVTCameraControllerCameraFlashModeAuto = 0,
/** Specifies that flash illumination is always on, no matter what the ambient light conditions are.*/
    HVTCameraControllerCameraFlashModeOn   = 1
};

/**
 Specifies the media type selected by the user.
 
 The value for this key is an NSString object containing a type code such as kUTTypeImage or kUTTypeMovie.
 */
extern NSString *const HVTCameraControllerMediaType;
/**
 Specifies the original image captured by the camera controller.
 
 The value for this key is a UIImage object.
 */
extern NSString *const HVTCameraControllerOriginalImage;
/**
 Specifies the filesystem URL for the movie.
 
 The value for this key is an NSURL object.
 */
extern NSString *const HVTCameraControllerMediaURL;
/**
 Metadata for a newly-captured photograph.
 
 This key applies only to still images.
 
 The value for this key is an NSDictionary object that contains the metadata of the photo that was just captured. To store the metadata along with the image in the Camera Roll, use the PHAssetChangeRequest class from the Photos framework.
 */
extern NSString *const HVTCameraControllerMediaMetadata;

/**
 The HVTCameraController operates exactly as the UIImagePickerController class of UIKit.
 It manages customizable, system-supplied user interfaces for taking pictures and movies on supported devices for use in your app. A camera controller manages user interactions and delivers the results of those interactions to a delegate object.
 
 The role and appearance of the camera controller depend on the source type you assign to it before you present it.
 
 To use the camera controller containing its default controls, perform these steps:
 
 Check which media types are available for the source type you’re using, by calling the availableMediaTypesForSourceType: class method. This lets you distinguish between a camera that can be used for video recording and one that can be used only for still images.
 
 Tell the image picker controller to adjust the UI according to the media types you want to make available—still images, movies, or both—by setting the mediaTypes property.
 
 Present the user interface by calling the presentViewController:animated:completion: method of the currently active view controller, passing your configured camera controller as the new view controller.
 
 When the user taps a button to pick a newly-captured or saved image or movie, or cancels the operation, dismiss the image picker using your delegate object. Your delegate can then save it to the Camera Roll on the device. 

 You can customize an image picker controller to manage user interactions yourself. To do this, provide an overlay view containing the controls you want to display. You can display your custom overlay view in addition to, or instead of, the default controls. 
 */
@interface HVTCameraController : UIViewController

/**----------------------------------------------------------------------
 * @name Configuring the Picker
 * ----------------------------------------------------------------------
 */

/**
The image picker’s delegate object.

The delegate receives notifications when the user finishes capturing an image or movie, or exits the camera interface. The delegate also decides when to dismiss the picker interface, so you must provide a delegate to use a picker.

For information about the methods you can implement for your delegate object, see HVTCameraControllerDelegate Protocol Reference.
*/
@property(nonatomic,weak) id <HVTCameraControllerDelegate> cameraDelegate;

/**
 An array indicating the media types to be accessed by the media picker controller.
 
 Depending on the media types you assign to this property, the camera controller displays a dedicated interface for still images or movies, or a selection control that lets the user choose the camera interface. Before setting this property, check which media types are available by calling the availableMediaTypesForSourceType: class method.
 
 If you set this property to an empty array, or to an array in which none of the media types is available for the current source, the system throws an exception.
 
 By default, this property is set to the single value kUTTypeImage, which designates the still camera interface. To designate the movie capture interface, use the kUTTypeMovie identifier in a statement like this:
 
 myCameraController.mediaTypes = @[(NSString *) kUTTypeMovie];
 */
@property(nonatomic,copy) NSArray *mediaTypes;

/**----------------------------------------------------------------------
 * @name Configuring the Video Capture Options
 * ----------------------------------------------------------------------
 */

/**
 The video recording quality expressed in resolution types.
 
 The video quality setting specified by this property is used during video recording.
 
 The various video qualities are listed in the HVTCameraControllerQualityType enumeration. The default value is HVTCameraControllerQualityType1920x1080. To capture a movie using a video quality other than the default value, you must set the quality explicitly.
 
 This property is available only if the mediaTypes property’s value array includes the kUTTypeMovie media type.
 
 If the cameraDevice does not support the videoQuality, it will use the default value.
 */
@property(nonatomic) HVTCameraControllerQualityType videoQuality;

/**
 The maximum duration, in seconds, for a video recording.
 
 The default value for this property is 10 minutes (600 seconds).
 
 This property is available only if the mediaTypes property’s value array includes the kUTTypeMovie media type.
 */
@property(nonatomic) NSTimeInterval videoMaximumDuration;

/**----------------------------------------------------------------------
 * @name Customizing the Camera Controls
 * ----------------------------------------------------------------------
 */

/**
 Indicates whether the camera controller displays the default camera controls.
 
 The default value of this property is YES, which specifies that the default camera controls are visible in the camera controller. Set it to NO to hide the default controls if you want to instead provide a custom overlay view using the cameraOverlayView property.
 */
@property(nonatomic) BOOL showsCameraControls;

/**
 The view to display on top of the default image picker interface.
 
 You can use an overlay view to present a custom view hierarchy on top of the default camera interface. The camera controller layers your custom overlay view on top of the other views and positions it relative to the screen coordinates. If you have the default camera controls set to be visible, incorporate transparency into your view, or position it to avoid obscuring the underlying content.
 */
@property(nonatomic,strong) UIView *cameraOverlayView;

/**
 The transform to apply to the camera’s preview image.
 
 This transform affects the live preview image only and does not affect your custom overlay view or the default camera controls. You can use this property in conjunction with custom controls to implement your own electronic zoom behaviors.
 */
@property(nonatomic) CGAffineTransform cameraViewTransform;

/**----------------------------------------------------------------------
 * @name Capturing Still Images or Movies
 * ----------------------------------------------------------------------
 */

/**
 Captures a still image using the camera.
 
 Calling this method while an image is being captured has no effect. You must wait until the associated delegate object receives an cameraController:didFinishCapturingMediaWithInfo: message before you can capture another picture.
 */
- (void)takePicture;

/**
 Starts video capture using the camera specified by the UIImagePickerControllerCameraDevice property.
 
 @return YES on success or NO on failure. This method may return a value of NO for various reasons, among them the following:
 
 Movie capture is already in progress.
 
 The camera controller's cameraCaptureMode is set to HVTCameraControllerCameraCaptureModePhoto.
 */
- (BOOL)startVideoCapture;

/**
 Stops video capture.
 
 After you call this method to stop video capture, the system calls the image picker delegate’s cameraController:didFinishCapturingMediaWithInfo: method.
 */
- (void)stopVideoCapture;

/**----------------------------------------------------------------------
 * @name Configuring the Camera
 * ----------------------------------------------------------------------
 */

/**
 The camera used by the image picker controller.
 
 The default is HVTCameraControllerCameraDeviceRear.
 */
@property(nonatomic) HVTCameraControllerCameraDevice cameraDevice;

/**
 Returns a Boolean value that indicates whether a given camera is available.
 
 @param cameraDevice A HVTCameraControllerCameraDevice constant indicating the camera whose availability you want to check.
 
 @return `YES` if the camera indicated by cameraDevice is available, or `NO` if it is not available.
 */
+ (BOOL)isCameraDeviceAvailable:(HVTCameraControllerCameraDevice)cameraDevice;

/**
 The capture mode used by the camera.
 
 The various capture modes are listed in the HVTCameraControllerCameraCaptureMode enumeration. The default value is HVTCameraControllerCameraCaptureModePhoto.
 */
@property(nonatomic) HVTCameraControllerCameraCaptureMode cameraCaptureMode;

/**
 The flash mode used by the active camera.
 
 The various flash modes are listed in the HVTCameraControllerCameraFlashMode enumeration. The default value is HVTCameraControllerCameraFlashModeAuto.
 
 The value of this property specifies the behavior of the still-image flash when the value of the cameraCaptureMode property is HVTCameraControllerCameraCaptureModePhoto, and specifies the behavior of the video torch when cameraCaptureMode is HVTCameraControllerCameraCaptureModeVideo.
 */
@property(nonatomic) HVTCameraControllerCameraFlashMode   cameraFlashMode;

/**
 Indicates whether a given camera has flash illumination capability.
 
 @param cameraDevice A HVTCameraControllerCameraDevice constant indicating the camera whose flash capability you want to know.
 
 @return `YES` if cameraDevice can use flash illumination, or `NO` if it cannot.
 */
+ (BOOL)isFlashAvailableForCameraDevice:(HVTCameraControllerCameraDevice)cameraDevice;

@end

/**
 The HVTCameraControllerDelegate protocol defines methods that your delegate object must implement to interact with the camera controller interface. The methods of this protocol notify your delegate when the user either finishes capturing an image or movie, or cancels the camera operation.
 
 The delegate methods are responsible for dismissing the picker when the operation completes. To dismiss the picker, call the dismissModalViewControllerAnimated:completion: method of the parent controller responsible for displaying the HVTCameraController object.
 
 To save a still image to the user’s Camera Roll album, call the UIImageWriteToSavedPhotosAlbum function from within the body of the cameraController:didFinishCapturingMediaWithInfo: method. To save a movie to the user’s Camera Roll album, instead call the UISaveVideoAtPathToSavedPhotosAlbum function. These functions, described in UIKit Function Reference, save the image or movie only; they do not save metadata.
 
 To write additional metadata when saving an image to the Camera Roll, use the PHAssetChangeRequest class from the Photos framework. See the description for the HVTCameraControllerMediaMetadata key.
 */
@protocol HVTCameraControllerDelegate <NSObject>

/**----------------------------------------------------------------------
 * @name Closing the Camera
 * ----------------------------------------------------------------------
 */

@optional
/**
 Tells the delegate that the user picked a still image or movie.
 
 Your delegate object’s implementation of this method should pass the specified media on to any custom code that needs it, and should then dismiss the camera controller view.
 
 Implementation of this method is optional, but expected.
 
 @param cameraController The controller object managing the camera interface.
 @param info A dictionary containing the original image and the edited image, if an image was picked; or a filesystem URL for the movie, if a movie was picked. The dictionary also contains any relevant editing information. The keys for this dictionary are listed in Editing Information Keys.
 */
- (void)cameraController:(HVTCameraController *)cameraController didFinishCapturingMediaWithInfo:(NSDictionary *)info;
/**
 Tells the delegate that the user cancelled the pick operation.
 
 Your delegate’s implementation of this method should dismiss the camera controller view by calling the dismissModalViewControllerAnimated:completion: method of the parent view controller.
 
 Implementation of this method is optional, but expected.

 @param cameraController The controller object managing the camera interface.
 */
- (void)cameraControllerDidCancel:(HVTCameraController *)cameraController;

@end