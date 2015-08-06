//
//  HVTView.h
//  HorizonSDK
//
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreVideo/CoreVideo.h>
#import <AVFoundation/AVCaptureSession.h>
#import <HorizonSDK/HVTVars.h>

/** Constants indicating whether the HVTView will display a leveled on unleveled preview. */
typedef NS_ENUM(NSInteger, HVTViewType) {
/** The view will show the camera's video stream without applying the leveling algorithm. */
    HVTViewTypeNormal,
/** The view will show the leveled video stream, previewing what will be saved in a video file.*/
    HVTViewTypeLeveled
};

/** Constants indicating how the video preview is displayed within the HVTView's bounds rect. */
typedef NS_ENUM(NSInteger, HVTViewFillMode) {
    /** This preserves the aspect ratio, leaving black bars where the video does not fill the available screen area. */
    HVTViewFillModeAspectFit,
    /** This preserves the aspect ratio, but fills the available screen area, cropping the video when necessary. */
    HVTViewFillModeAspectFill
};

/** 
 HVTView is a subclass of UIView that you use to preview video as it is being captured by HVTCamera.
 
 Being a subclass of UIKit's UIView, it inherits all the properties of this class and
 extends its functionality. You can create an HVTView by using the Interface Builder (and setting the 
 class property to HVTView) or by instantiating it programmatically with initWithCoder: and 
 initWithFrame: methods.
 
 You use this view in conjunction with an HVTCamera instance, as illustrated in the following code fragment:
 
    HVTcamera *camera = [HVTcamera new];
    HVTView *view = // an instance of HVTView created programmatically or in the Interface Builder
    [camera addView:view];
 
 The view's orientation is derived by the HVTCamera it is attached to. So, make sure the [HVTCamera interfaceOrientation] property
 matches your app's interface orientation.

 You can use the fillMode property to influence how the video preview is displayed relative to the view's bounds.
 You can use the showHUD property if you want the crop region to be displayed or not.
 */
@interface HVTView : UIView

/** @name Configuration */

/** Specifies the preview type of the view.
 
 When set to `HVTViewTypeNormal`, HUD elements can be displayed depending on the showHUD property.
 
 The default value, `HVTViewTypeNormal` is best suited for most applications.

 @see HVTViewType*/
@property (nonatomic) HVTViewType viewType;

/** Specifies whether the view is enabled or not.
 
 Enabled means that the rendering takes place and disabled is when rendering is not active for this view.
 
 Use this property to manually stop the renderning to a specific view for certain cases (e.g. use in a UICollectionViewCell)
 */
@property (nonatomic) BOOL enabled;

/** Specifies the active filter for this view.
 
 You can use the predefined filters provided by Core Image library or add your own custom CIKernels.
 */
@property (nonatomic) CIFilter *filter;

/** Specifies whether HUD elements, such as the crop region and corners, are shown.
 
 This property applies only when viewType is set to `HVTViewTypeNormal`.
 
 The default value is `YES`. */
@property (nonatomic) BOOL showHUD;

/** Specifies how the video preview is displayed within the view's bounds rect.
 
 A common usage scenario is to toggle the fill mode when the user double taps the view.
 
 The property in animated. 
 
 The default value is `HVTViewFillModeAspectFit`.
 @see HVTViewFillMode*/
@property (nonatomic) HVTViewFillMode fillMode;

/** Controls if the view will include a double tap gesture to cycle through the different fill modes available.
 
 The default value is `NO`.
 @see HVTViewFillMode*/
@property (nonatomic, getter=isDoubleTapToChangeFillModeEnabled) BOOL enablesDoubleTapToChangeFillMode;

/** Controls if the view will include a tap to focus gesture.
 
 The default value is `NO`.*/
@property (nonatomic, getter=isTapToFocusEnabled) BOOL enablesTapToFocus;

/** Controls if the view will include a long press to lock focus and exposure gesture.
 
 The default value is `NO`.*/
@property (nonatomic, getter=isLongPressToLockFocusExposureEnabled) BOOL enablesLongPressToLockFocusExposure;

@end