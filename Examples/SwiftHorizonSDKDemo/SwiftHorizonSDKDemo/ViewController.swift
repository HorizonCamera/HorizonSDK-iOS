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
                
                DispatchQueue.main.async {
                    self.startCamera()
                }
            });
        });
    }
    
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        
        // The preview follows the interface: tell the camera whenever the window rotates (iPad)
        if let scene = view.window?.windowScene {
            camera.interfaceOrientation = scene.interfaceOrientation
        }
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
            
            let recordingURL    = FileManager.default.temporaryDirectory.appendingPathComponent("Movie.mov")
            let configuration   = HVTRecordingConfiguration(movieURL: recordingURL)
            
            do {
                try camera.startRecording(with: configuration)
            } catch {
                print("Cannot record: \(error)")
            }
        }
    }
    
    // MARK: Private methods
    
    // Starts off the main thread; the completion reports why it could not start (e.g. camera access denied)
    func startCamera() {
        camera.startRunning { error in
            if let error = error {
                print("Cannot start the camera: \(error)")
            }
        }
    }
    
    func removeTempMovie(movieURL: URL) {
        do {
            try FileManager.default.removeItem(at: movieURL)
        } catch _ {
        }
    }
    
    // MARK: HVTCameraDelegate methods
    
    func hvtCameraRecordingDidStart(_ hvtCamera: HVTCamera) {
        
        recordButton.setTitle("Stop", for: .normal)
    }
    
    func hvtCamera(_ hvtCamera: HVTCamera, didUpdateRecordingDuration duration: TimeInterval) {
        
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
    
    func hvtCamera(_ hvtCamera: HVTCamera, recordingDidFailWithError error: Error) {
        
        recordButton.setTitle("Record", for: .normal)
    }
    
    func hvtCamera(_ hvtCamera: HVTCamera, didStopRecordingWithMetadata metadata: [String : Any]) {
        
        recordButton.setTitle("Record", for: .normal)
        
        guard let movieURL = metadata[HVTMetadataMovieURLKey] as? URL else {
            return
        }

        PHPhotoLibrary.requestAuthorization(for: .addOnly, handler: { (status:PHAuthorizationStatus) in
            
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
    
    func hvtCamera(_ hvtCamera: HVTCamera, didUpdate params: HVTParams) {
        recordButton.transform = CGAffineTransform(rotationAngle: CGFloat(params.angle))
    }
}

