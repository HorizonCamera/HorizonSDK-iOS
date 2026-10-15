//
//  HVTPhoto.h
//  HorizonSDK
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@class HVTPhotoSettings;

NS_ASSUME_NONNULL_BEGIN

/**
 A leveled photo delivered by -[HVTCameraDelegate hvtCamera:didCapturePhoto:].
 */
@interface HVTPhoto : NSObject

- (instancetype)init NS_UNAVAILABLE;

/** The leveled, cropped image. */
@property (nonatomic, readonly) CGImageRef CGImage;

/** Image metadata (EXIF, TIFF, GPS when set via setGPSMetadata:), keyed as in ImageIO. */
@property (nonatomic, readonly, copy) NSDictionary<NSString *, id> *metadata;

/** Settings the photo was taken with. */
@property (nonatomic, readonly, copy) HVTPhotoSettings *settings;

/**
 The image encoded with settings.codec (JPEG or HEIF), metadata included.

 @return The file data, or nil if the codec cannot be encoded on this device.
 */
- (nullable NSData *)fileDataRepresentation;

@end

NS_ASSUME_NONNULL_END
