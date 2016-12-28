//
//  ViewController.m
//  AdvancedHorizonSDKDemo
//
//  Created by Stelios Petrakis on 2/10/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import "ViewController.h"

@import HorizonSDK;
@import AssetsLibrary;

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
    
    NSError *error = nil;
    
    if([_camera cameraPosition] == AVCaptureDevicePositionBack)
        [_camera setCameraPosition:AVCaptureDevicePositionFront withResolution:CGSizeMake(1280.0, 720.0) error:&error];
    else
        [_camera setCameraPosition:AVCaptureDevicePositionBack withResolution:CGSizeMake(1920.0, 1080.0) error:&error];
    
    if(error)
        NSLog(@"%@", error);
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
        
        [self.camera startRecordingWithMovieURL:recordingURL];
    }
}

#pragma mark - Private methods

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
                                     [self.camera startRunning];
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
    
    ALAssetsLibrary *library = [ALAssetsLibrary new];
    
    [library writeVideoAtPathToSavedPhotosAlbum:movieURL
                                completionBlock:^(NSURL *assetURL, NSError *error) {
                                    
                                    [[NSFileManager defaultManager] removeItemAtURL:movieURL
                                                                              error:NULL];
                                }];
}

@end
