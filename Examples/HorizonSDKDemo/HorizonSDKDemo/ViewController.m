//
//  ViewController.m
//  HorizonSDKDemo
//
//  Created by Stelios Petrakis on 2/5/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import "ViewController.h"

@import HorizonSDK;
@import Photos;

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

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];

    // The preview follows the interface: tell the camera whenever the window rotates (iPad)
    UIWindowScene *scene = self.view.window.windowScene;

    if (scene) {
        self.camera.interfaceOrientation = scene.interfaceOrientation;
    }
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

#pragma mark - IB methods

- (IBAction)toggleRecording:(id)sender {
    
    if(![self.camera isRunning]) {
        
        [self startCamera];
        return;
    }
    
    if([self.camera isRecording])
        [self.camera stopRecording];
    else
    {
        NSURL *recordingURL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), @"Movie.MOV"]]];
        HVTRecordingConfiguration *configuration = [HVTRecordingConfiguration configurationWithMovieURL:recordingURL];
        NSError *error = nil;

        if (![self.camera startRecordingWithConfiguration:configuration error:&error]) {
            NSLog(@"Cannot record: %@", error);
        }
    }
}

#pragma mark - Private methods

// Starts off the main thread; the completion reports why it could not start (e.g. camera access denied)
- (void)startCamera
{
    [self.camera startRunningWithCompletion:^(NSError *error) {
        if (error) {
            NSLog(@"Cannot start the camera: %@", error);
        }
    }];
}

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
                                     [self startCamera];
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
    
    [PHPhotoLibrary requestAuthorizationForAccessLevel:PHAccessLevelAddOnly handler:^(PHAuthorizationStatus status) {
        
        if(status == PHAuthorizationStatusAuthorized) {

            [[PHPhotoLibrary sharedPhotoLibrary] performChanges:^{ [PHAssetChangeRequest creationRequestForAssetFromVideoAtFileURL:movieURL]; }
                                              completionHandler:^(BOOL success, NSError *error) {
                                                 
                                                  [[NSFileManager defaultManager] removeItemAtURL:movieURL
                                                                                            error:NULL];
                                              }];
        }
    }];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateParams:(HVTParams)params { }

@end
