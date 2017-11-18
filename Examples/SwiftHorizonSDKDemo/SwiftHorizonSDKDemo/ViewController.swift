//
//  ViewController.swift
//  HorizonSDKSwiftSample
//
//  Created by Stelios Petrakis on 2/25/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

import UIKit
import Photos
import HorizonSDK

class ViewController : UIViewController, HVTCameraDelegate {
    
    var camera : HVTCamera!
    
    @IBOutlet weak var preview: HVTView!
    
    @IBOutlet weak var recordButton: UIButton!
    
    // MARK: View lifecycle
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        recordButton.layer.cornerRadius = 30.0
        
        preview.fillMode = .aspectFill
        
        camera = HVTCamera()
        camera.add(preview)
        camera.delegate = self
        
        AVCaptureDevice.requestAccess(for: AVMediaType.video, completionHandler: { (Bool) -> Void in
            
            AVCaptureDevice.requestAccess(for: AVMediaType.audio, completionHandler: { (Bool) -> Void in
                
                self.camera.startRunning();
            });
        });
    }
    
    override var prefersStatusBarHidden: Bool {
        get {
            return true
        }  
    }
    
    // MARK: IB methods
    
    @IBAction func recordTapped(_ sender: AnyObject) {
        
        if(camera.isRecording()) {
            
            camera.stopRecording()
        }
        else {
            
            let filePath        = NSString.path(withComponents: [NSTemporaryDirectory(), "Movie.mov"])
            let recordingURL    = NSURL(fileURLWithPath: filePath)
            
            camera.startRecording(withMovieURL: recordingURL as URL!)
        }
    }
    
    // MARK: Private methods
    
    func removeTempMovie(movieURL: URL!) {
        do {
            try FileManager.default.removeItem(at: movieURL)
        } catch _ {
        }
    }
    
    // MARK: HVTCameraDelegate methods
    
    func hvtCameraRecordingDidStart(_ hvtCamera: HVTCamera!) {
        
        recordButton.setTitle("Stop", for: .normal)
    }
    
    func hvtCamera(_ hvtCamera: HVTCamera!, didUpdateRecordingDuration duration: TimeInterval) {
        
        let seconds = Int(duration)
        var newDuration = "00:00"
        
        if(seconds > 3600) {
            
            newDuration = NSString(format: "%02lu:%02lu:%02lu", seconds/3600, ((seconds / 60) % 60), seconds % 60) as String
        }
        else {
            
            newDuration = NSString(format: "%02lu:%02lu", seconds / 60, seconds % 60) as String
        }
        
        recordButton.setTitle(newDuration, for: .normal)
    }
    
    func hvtCamera(_ hvtCamera: HVTCamera!, recordingDidFailWithError error: Error!) {
        
        recordButton.setTitle("Record", for: .normal)
    }
    
    func hvtCamera(_ hvtCamera: HVTCamera!, didStopRecordingWithMetadata metadata: [AnyHashable : Any]!) {
        
        recordButton.setTitle("Record", for: .normal)
        
        let metadataDict = metadata as NSDictionary
        let movieURL = metadataDict.object(forKey: HVTMetadataMovieURLKey) as! URL

        PHPhotoLibrary.requestAuthorization({ (status:PHAuthorizationStatus) in
            
            if(status != .authorized) {
                return
            }
          
            PHPhotoLibrary.shared().performChanges({
                PHAssetChangeRequest.creationRequestForAssetFromVideo(atFileURL: movieURL)
            }, completionHandler: { (success, error) in
                
                self.removeTempMovie(movieURL: movieURL)
            })
        })
    }
    
    func hvtCamera(_ hvtCamera: HVTCamera!, didUpdate params: HVTParams) {
        recordButton.transform = CGAffineTransform(rotationAngle: CGFloat(params.angle))
    }
}

