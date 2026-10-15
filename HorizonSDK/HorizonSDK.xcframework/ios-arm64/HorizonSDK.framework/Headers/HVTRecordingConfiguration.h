//
//  HVTRecordingConfiguration.h
//  HorizonSDK
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 What and where to record with -[HVTCamera startRecordingWithConfiguration:error:].
 */
@interface HVTRecordingConfiguration : NSObject <NSCopying>

/** A configuration recording H.264 into a QuickTime movie at movieURL. An existing file is replaced. */
+ (instancetype)configurationWithMovieURL:(NSURL *)movieURL;

- (instancetype)init NS_UNAVAILABLE;

/** Output file. An existing file at this URL is replaced. */
@property (nonatomic, copy) NSURL *movieURL;

/** `AVVideoCodecTypeH264` (default) or `AVVideoCodecTypeHEVC`. Ignored when videoSettings is set. */
@property (nonatomic, copy) AVVideoCodecType codec;

/** `AVFileTypeQuickTimeMovie` (default) or `AVFileTypeMPEG4`. */
@property (nonatomic, copy) AVFileType fileType;

/** Full video encoder settings (see AVVideoSettings.h); nil derives them from codec and the output movie size. */
@property (nonatomic, copy, nullable) NSDictionary<NSString *, id> *videoSettings;

/** Full audio encoder settings (see AVAudioSettings.h); nil uses the recommended settings for fileType. */
@property (nonatomic, copy, nullable) NSDictionary<NSString *, id> *audioSettings;

/** Metadata items written to the movie. */
@property (nonatomic, copy, nullable) NSArray<AVMetadataItem *> *metadata;

@end

NS_ASSUME_NONNULL_END
