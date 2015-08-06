//
//  HorizonSDK.h
//  HorizonSDK
//
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import <HorizonSDK/HVTCamera.h>

/**
 The base class of Horizon SDK
 
 Use this class to initiate the SDK using the activateWithAPIKey: method by accessing the singleton instance sharedInstance
 */
@interface HVTSDK : NSObject

/**
 *  Returns the singleton HVTSDK reference to use in your application.
 *
 *  Do not initialize the HVTSDK class yourself.
 *
 *  @return The HVTSDK singleton instance reference.
 */
+ (HVTSDK*) sharedInstance;

/**
 *  @brief Start the shared HorizonSDK session
 *
 *  This method serves as the entry point to Horizon SDK.  It must be
 *  called before instantiating a new HVTCamera object, preferably in the scope
 *  of application:didFinishLaunchingWithOptions: mthod of your UIApplicationDelegate class.
 *
 *  @param apiKey The API key for this app.
 *  @return YES if the HVTSDK was successfully activated, NO otherwise (with NSInternalInconsistencyException for exception handling)
 */
- (BOOL)activateWithAPIKey:(NSString*)apiKey;

@end