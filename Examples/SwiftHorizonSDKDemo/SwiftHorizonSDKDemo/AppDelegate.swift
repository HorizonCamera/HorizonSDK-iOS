//
//  AppDelegate.swift
//  SwiftHorizonSDKDemo
//
//  Created by Stelios Petrakis on 8/15/15.
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey : Any]? = nil) -> Bool {

        HVTSDK.sharedInstance().activate(withAPIKey: "QMCA6kLDQGpK4c92SfmBK7Kr9dNxwekmLCZKYKb4x3Q+OcFQ/iZS8MH2V5xW/0lLkY9JTY3c8UtvV+PDLvRkBWNvbS5odnQuSG9yaXpvblNESy1Td2lmdHxDVQ==")
        
        return true
    }

}

