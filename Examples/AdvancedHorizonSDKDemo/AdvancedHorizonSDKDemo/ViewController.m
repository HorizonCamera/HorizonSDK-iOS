//
//  ViewController.m
//  AdvancedHorizonSDKDemo
//
//  Created by Stelios Petrakis on 2/10/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import "ViewController.h"

@import HorizonSDK;
@import Photos;

@interface ViewController () <HVTCameraDelegate>
{
    BOOL _filterActive;
}

@property (nonatomic) HVTCamera *camera;
@property (weak, nonatomic) IBOutlet HVTView *topView;
@property (weak, nonatomic) IBOutlet HVTView *bottomView;
@property (weak, nonatomic) IBOutlet UIButton *recordButton;
@property (weak, nonatomic) IBOutlet UIButton *positionButton;
@property (weak, nonatomic) IBOutlet UIButton *cropButton;
@property (weak, nonatomic) IBOutlet UIButton *filterButton;

@end

@implementation ViewController

#pragma mark - View lifecycle

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    _filterActive = NO;
    
    [self.recordButton.layer setCornerRadius:30.];
    [self.positionButton.layer setCornerRadius:30.];
    [self.cropButton.layer setCornerRadius:30.];
    [self.filterButton.layer setCornerRadius:30.];
    
    self.camera = [HVTCamera new];
    [self.camera setDelegate:self];
    [self.camera setInterfaceOrientation:UIInterfaceOrientationPortrait];
    
    [self.topView setViewType:HVTViewTypeLeveled];
    
    [self.camera addView:self.topView];
    [self.camera addView:self.bottomView];
    
    UITapGestureRecognizer *doubleTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTap)];
    [doubleTapRecognizer setNumberOfTapsRequired:2];
    [self.bottomView addGestureRecognizer:doubleTapRecognizer];
    
    [self authorizeCamera];
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return UIInterfaceOrientationPortrait;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait;
}

- (BOOL)prefersStatusBarHidden {
    
    return YES;
}

#pragma mark - IB methods

- (IBAction)positionButtonTapped:(id)sender {
    
    AVCaptureDevicePosition position = ([_camera cameraPosition] == AVCaptureDevicePositionBack ? AVCaptureDevicePositionFront : AVCaptureDevicePositionBack);

    // The wide-angle camera on the other side: every iPhone and iPad has one, front and back
    NSUInteger index = [[HVTCamera availableDevices] indexOfObjectPassingTest:^BOOL(AVCaptureDevice *device, NSUInteger idx, BOOL *stop) {
        return device.position == position && [device.deviceType isEqualToString:AVCaptureDeviceTypeBuiltInWideAngleCamera];
    }];

    if (index == NSNotFound) {
        return;
    }

    NSError *error = nil;

    if (![_camera setCaptureDevice:[HVTCamera availableDevices][index] error:&error]) {
        NSLog(@"Cannot switch camera: %@", error);
    }
}

- (IBAction)cropButtonTapped:(id)sender {

    if(self.camera.levelerCropMode == HVTLevelerCropModeFlex)
        [self.camera setLevelerCropMode:HVTLevelerCropModeRotate];
    else if(self.camera.levelerCropMode == HVTLevelerCropModeRotate)
        [self.camera setLevelerCropMode:HVTLevelerCropModeLocked];
    else if(self.camera.levelerCropMode == HVTLevelerCropModeLocked)
        [self.camera setLevelerCropMode:HVTLevelerCropModeFlex];
}

- (IBAction)filterButtonTapped:(id)sender {
    
    if(_filterActive)
    {
        [self.topView setFilter:nil];
        [self.bottomView setFilter:nil];
        [self.camera removeActiveFilter];
        _filterActive = NO;
    }
    else
    {
        CIFilter *sepiaFilter = [CIFilter filterWithName:@"CISepiaTone"];
        [sepiaFilter setValue:@.8f forKey:kCIInputIntensityKey];
        [self.topView setFilter:sepiaFilter];
        [self.bottomView setFilter:sepiaFilter];
        [self.camera setActiveFilter:sepiaFilter];
        
        _filterActive = YES;
    }
}

- (IBAction)toggleRecording:(id)sender {
    
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

- (void)doubleTap {
    
    if(self.bottomView.fillMode == HVTViewFillModeAspectFill)
        [self.bottomView setFillMode:HVTViewFillModeAspectFit];
    else
        [self.bottomView setFillMode:HVTViewFillModeAspectFill];
}

- (void)authorizeCamera {
    
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo
                             completionHandler:^(BOOL granted) {
                                 
                                 dispatch_async(dispatch_get_main_queue(), ^{
                                     [self authorizeMicrophone];
                                 });
                             }];
}

- (void)authorizeMicrophone {
    
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio
                             completionHandler:^(BOOL granted) {
                                 
                                 dispatch_async(dispatch_get_main_queue(), ^{
                                     [self startCamera];
                                 });
                             }];
}

#pragma mark - HVTCameraDelegate

- (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateParams:(HVTParams)params {
    
    double angle = params.angle;
    
    CGAffineTransform buttonTransform = CGAffineTransformMakeRotation(angle);
    
    [self.recordButton setTransform:buttonTransform];
    [self.cropButton setTransform:buttonTransform];
    [self.positionButton setTransform:buttonTransform];
    [self.filterButton setTransform:buttonTransform];
}

- (void)hvtCameraRecordingDidStart:(HVTCamera *)hvtCamera {
    
    [self.positionButton setHidden:YES];
    [self.filterButton setHidden:YES];
    [self.recordButton setTitle:@"Stop" forState:UIControlStateNormal];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera didUpdateRecordingDuration:(NSTimeInterval)duration {

    NSUInteger seconds = (NSUInteger)duration;
    
    NSString *newDuration = @"00:00";
    
    if(seconds > 3600.0)
        newDuration = [NSString stringWithFormat:@"%02lu:%02lu:%02lu", seconds/3600UL, ((seconds / 60UL) % 60UL), seconds % 60UL];
    else
        newDuration = [NSString stringWithFormat:@"%02lu:%02lu", seconds / 60UL, seconds % 60UL];
    
    [self.recordButton setTitle:newDuration forState:UIControlStateNormal];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera recordingDidFailWithError:(NSError *)error {
    
    [self.positionButton setHidden:NO];
    [self.filterButton setHidden:NO];
    [self.recordButton setTitle:@"Record" forState:UIControlStateNormal];
}

- (void)hvtCamera:(HVTCamera *)hvtCamera didStopRecordingWithMetadata:(NSDictionary*)metadata {
    
    [self.positionButton setHidden:NO];
    [self.filterButton setHidden:NO];
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

@end
