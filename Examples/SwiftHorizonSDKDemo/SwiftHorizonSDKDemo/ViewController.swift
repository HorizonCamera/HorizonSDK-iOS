//
//  ViewController.swift
//  HorizonSDKSwiftSample
//
//  Created by Stelios Petrakis on 2/25/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

import UIKit
import AssetsLibrary

class ViewController : UIViewController, HVTCameraDelegate {
    
    var camera : HVTCamera!
    
    @IBOutlet weak var preview: HVTView!
    
    @IBOutlet weak var recordButton: UIButton!
    
    // MARK: View lifecycle
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
                
        recordButton.layer.cornerRadius = 30.0
        
        preview.fillMode = .AspectFill
        
        camera = HVTCamera()
        camera.addView(preview)
        camera.delegate = self
        
        AVCaptureDevice.requestAccessForMediaType(AVMediaTypeVideo, completionHandler: { (Bool) -> Void in
            
            AVCaptureDevice.requestAccessForMediaType(AVMediaTypeAudio, completionHandler: { (Bool) -> Void in
                
                self.camera.startRunning();
            });
        });
    }
    
    override func prefersStatusBarHidden() -> Bool {
        return true
    }
    
    // MARK: IB methods
    
    @IBAction func recordTapped(sender: UIButton) {
        
        if(camera.isRecording()) {
            
            camera.stopRecording()
        }
        else {
            
            let filePath        = NSString.pathWithComponents([NSTemporaryDirectory(), "Movie.mov"])
            let recordingURL    = NSURL(fileURLWithPath: filePath)
            
            camera.startRecordingWithMovieURL(recordingURL)
        }
    }
    
    // MARK: Private methods
    
    func removeTempMovie(movieURL: NSURL!) {
        do {
            try NSFileManager.defaultManager().removeItemAtURL(movieURL)
        } catch _ {
        }
    }
    
    // MARK: HVTCameraDelegate methods
    
    func hvtCameraRecordingDidStart(hvtCamera: HVTCamera!) {
        
        recordButton.setTitle("Stop", forState: .Normal)
    }
    
    func hvtCamera(hvtCamera: HVTCamera!, didUpdateRecordingDuration duration: NSTimeInterval) {
        
        let seconds = Int(duration)
        var newDuration = "00:00"
        
        if(seconds > 3600) {
            
            newDuration = NSString(format: "%02lu:%02lu:%02lu", seconds/3600, ((seconds / 60) % 60), seconds % 60) as String
        }
        else {
            
            newDuration = NSString(format: "%02lu:%02lu", seconds / 60, seconds % 60) as String
        }
        
        recordButton.setTitle(newDuration, forState: .Normal)
    }
    
    func hvtCamera(hvtCamera: HVTCamera!, recordingDidFailWithError error: NSError!) {
        
        recordButton.setTitle("Record", forState: .Normal)
    }
    
    func hvtCamera(hvtCamera: HVTCamera!, didStopRecordingWithMetadata metadata: [NSObject : AnyObject]!) {
        
        recordButton.setTitle("Record", forState: .Normal)
        
        let metadataDict = metadata as NSDictionary
        let movieURL = metadataDict.objectForKey(HVTMetadataMovieURLKey) as! NSURL
        
        ALAssetsLibrary().writeVideoAtPathToSavedPhotosAlbum(movieURL, completionBlock: { (path: NSURL!,error: NSError!) -> Void in
            self.removeTempMovie(movieURL)
        })
    }
    
    func hvtCamera(hvtCamera: HVTCamera!, didUpdateParams params: HVTParams) {
        recordButton.transform = CGAffineTransformMakeRotation(CGFloat(params.angle))
    }
}

