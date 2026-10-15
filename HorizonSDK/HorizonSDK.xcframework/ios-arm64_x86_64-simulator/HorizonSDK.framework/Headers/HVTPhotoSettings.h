//
//  HVTPhotoSettings.h
//  HorizonSDK
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Options for one photo taken with -[HVTCamera capturePhotoWithSettings:].

 Create a new instance per capture; the camera reports it back in the HVTPhoto it delivers.
 */
@interface HVTPhotoSettings : NSObject <NSCopying>

/** Default settings: flash off, no HDR bracket, balanced quality, JPEG. */
+ (instancetype)photoSettings;

/** Flash for this shot. Unsupported modes (e.g. on a camera without flash) fall back to off. Default `AVCaptureFlashModeOff`. */
@property (nonatomic) AVCaptureFlashMode flashMode;

/**
 Captures an under/normal/over exposure bracket and blends it into one leveled photo.

 Requires isHDRPhotoCaptureAvailable; otherwise the capture fails with HVTErrorPhotoHDRNotSupported.
 Flash is not used for bracketed shots. Default `NO`.
 */
@property (nonatomic) BOOL usesHDRBracket;

/**
 Speed versus quality trade-off of the system photo pipeline (Smart HDR, Deep Fusion and similar
 run at `.quality`). Ignored for HDR brackets. Default `AVCapturePhotoQualityPrioritizationBalanced`.
 */
@property (nonatomic) AVCapturePhotoQualityPrioritization qualityPrioritization;

/** Codec of HVTPhoto's fileDataRepresentation: `AVVideoCodecTypeJPEG` (default) or `AVVideoCodecTypeHEVC` (HEIF). */
@property (nonatomic, copy) AVVideoCodecType codec;

@end

NS_ASSUME_NONNULL_END
