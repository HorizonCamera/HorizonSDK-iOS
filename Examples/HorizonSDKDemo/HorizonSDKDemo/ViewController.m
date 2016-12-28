//
//  ViewController.m
//  HorizonSDKDemo
//
//  Created by Stelios Petrakis on 2/5/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import "ViewController.h"

@import HorizonSDK;
@import AssetsLibrary;

@interface ViewController () <HVTCameraDelegate>

@property (nonatomic) HVTCamera *camera;

@property (weak, nonatomic) IBOutlet HVTView *preview;
@property (weak, nonatomic) IBOutlet UIButton *recordButton;

@end

@implementation ViewController

#pragma mark - View lifecycle 

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.recordButton.layer setCornerRadius:33.];
    
    self.camera = [HVTCamera new];
    [self.camera setDelegate:self];
    [self.camera addView:self.preview];
    
    [self authorizeCamera];
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

#pragma mark - IB methods

- (IBAction)toggleRecording:(id)sender {
    
    if([self.camera isRecording])
        [self.camera stopRecording];
    else
    {
        NSURL *recordingURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), @"Movie.MOV"]]];

        [self.camera startRecordingWithMovieURL:recordingURL];
    }
}

#pragma mark - Private methods

- (void)authorizeCamera
{
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo
                             completionHandler:^(BOOL granted) {
                                 
                                 dispatch_async(dispatch_get_main_queue(), ^{
                                     [self authorizeMicrophone];
                                 });
                             }];
}

- (void)authorizeMicrophone
{
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio
                             completionHandler:^(BOOL granted) {
                                 
                                 dispatch_async(dispatch_get_main_queue(), ^{
                                     [self.camera startRunning];
                                 });
                             }];
}

#pragma mark - HVTCameraDelegate

- (void)hvtCameraRecordingDidStart:(HVTCamera *)hvtCamera {
    
    [self.recordButton setTitle:@"Stop" forState:UIControlStateNormal];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateRecordingDuration:(NSTimeInterval)duration{
    
    NSUInteger seconds = (NSUInteger)duration;
    
    NSString *newDuration = @"00:00";
    
    if(seconds > 3600.0)
        newDuration = [NSString stringWithFormat:@"%02lu:%02lu:%02lu", seconds/3600UL, ((seconds / 60UL) % 60UL), seconds % 60UL];
    else
        newDuration = [NSString stringWithFormat:@"%02lu:%02lu", seconds / 60UL, seconds % 60UL];
    
    [self.recordButton setTitle:newDuration forState:UIControlStateNormal];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera recordingDidFailWithError:(NSError *)error {
    
    [self.recordButton setTitle:@"Record" forState:UIControlStateNormal];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera didStopRecordingWithMetadata:(NSDictionary*)metadata {
    
    [self.recordButton setTitle:@"Record" forState:UIControlStateNormal];
    
    NSURL *movieURL = [metadata objectForKey:HVTMetadataMovieURLKey];
    
    ALAssetsLibrary *library = [ALAssetsLibrary new];
    
    [library writeVideoAtPathToSavedPhotosAlbum:movieURL
                                completionBlock:^(NSURL *assetURL, NSError *error) {
        
        [[NSFileManager defaultManager] removeItemAtURL:movieURL
                                                  error:NULL];
    }];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateParams:(HVTParams)params { }

@end
