//
//  HorizonSDK.h
//  HorizonSDK
//
//  Copyright (c) 2015 Horizon Video Technologies. All rights reserved.
//

#import <HorizonSDK/HVTCamera.h>

NS_ASSUME_NONNULL_BEGIN

/**
 The base class of Horizon SDK
 
 Use this class to initiate the SDK using the activateWithAPIKey: method by accessing the singleton instance sharedInstance
 */
@interface HVTSDK : NSObject

/**
 Returns the singleton HVTSDK reference to use in your application.

 Do not initialize the HVTSDK class yourself.
 
 @return The HVTSDK singleton instance reference.
 */
+ (HVTSDK*) sharedInstance;

/**
 @brief Start the shared HorizonSDK session

 This method serves as the entry point to Horizon SDK.  It must be
 called before instantiating a new HVTCamera object, preferably in the scope
 of application:didFinishLaunchingWithOptions: method of your UIApplicationDelegate class.
 
 @param apiKey The API key for this app.
 @return YES if the HVTSDK was successfully activated, NO otherwise (with NSInternalInconsistencyException for exception handling)
 */
- (BOOL)activateWithAPIKey:(NSString*)apiKey API_DEPRECATED_WITH_REPLACEMENT("activateWithAPIKey:error:", ios(8.0, 15.0)) NS_SWIFT_UNAVAILABLE("Raises an Objective-C exception; use the throwing activate(withAPIKey:)");

/**
 @brief Start the shared HorizonSDK session with extra settings
 
 This method serves as the entry point to Horizon SDK.  It must be
 called before instantiating a new HVTCamera object, preferably in the scope
 of application:didFinishLaunchingWithOptions: method of your UIApplicationDelegate class.
 
 @param apiKey The API key for this app.
 @param settingsDictionary You can control various extra parameters using settings dictionary (@see HVTScaleFilterFrequencySettingsKey)
 @return YES if the HVTSDK was successfully activated, NO otherwise (with NSInternalInconsistencyException for exception handling)
 */
- (BOOL)activateWithAPIKey:(NSString*)apiKey settings:(nullable NSDictionary<NSString *, id> *)settingsDictionary API_DEPRECATED_WITH_REPLACEMENT("activateWithAPIKey:settings:error:", ios(8.0, 15.0)) NS_SWIFT_UNAVAILABLE("Raises an Objective-C exception; use the throwing activate(withAPIKey:settings:)");

/**
 @brief Start the shared HorizonSDK session, reporting failures as an error

 Same as activateWithAPIKey: but never throws, so it is safe to call from Swift
 (`try HVTSDK.sharedInstance().activate(withAPIKey:)`).

 @param apiKey The API key for this app.
 @param error On failure, an error in HVTErrorDomain with code HVTErrorInvalidAPIKey; its
 localizedDescription names the reason (e.g. "App bundle id does not match").
 @return YES if the HVTSDK was successfully activated, NO otherwise.
 */
- (BOOL)activateWithAPIKey:(NSString*)apiKey error:(NSError **)error;

/**
 @brief Start the shared HorizonSDK session with extra settings, reporting failures as an error

 @param apiKey The API key for this app.
 @param settingsDictionary You can control various extra parameters using settings dictionary (@see HVTScaleFilterFrequencySettingsKey)
 @param error On failure, an error in HVTErrorDomain with code HVTErrorInvalidAPIKey.
 @return YES if the HVTSDK was successfully activated, NO otherwise.
 */
- (BOOL)activateWithAPIKey:(NSString*)apiKey settings:(nullable NSDictionary<NSString *, id> *)settingsDictionary error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
