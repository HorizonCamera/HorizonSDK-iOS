Pod::Spec.new do |s|
  s.name             = 'HorizonSDK'
  s.version          = '2.5'
  s.license          =  { :type => 'Commercial', :text => "See https://horizon.camera/sdk/termsofservice" }
  s.homepage         = 'https://horizon.camera/sdk/'
  s.documentation_url = 'https://horizon.camera/sdk/docs/'
  s.summary          = 'Horizon SDK makes it easy to integrate video and photo recording capabilities to your mobile apps.'
  s.authors          = 'Horizon Video Technologies Inc.'
  s.social_media_url = 'https://twitter.com/HorizonCamera'
  s.description      = <<-DESC
Horizon SDK is a state of the art real-time video recording / photo shooting iOS library.

Some of the features of Horizon SDK include:

Horizon real-time leveling algorithm (you can find more at https://horizon.camera)
Support for multiple resolutions (up to 2K recording) and frame rates.
Custom filter support using the Core Image framework.
Multiple previews.
A simple view controller that makes integrating the SDK to your app really easy.
Access to the video buffer so that it can be further processed or live streamed using other libraries.
In order to use Horizon SDK on your app, you have to receive an API key by contacting us. The provided sample apps contain their own API keys.

The library can be used on devices running iOS version 8 or higher. You can build your application using the latest Xcode.

For more information take a look at the Installation and Quick Start guides of the documentation.

#leveling #filter #capture #video #photo
DESC
  s.source           = { :git => "https://github.com/HorizonCamera/HorizonSDK-iOS.git", :tag => s.version.to_s }

  s.platform = :ios
  
  s.ios.deployment_target = '8.0'
  
  s.preserve_paths = 'README.md'

  s.vendored_frameworks = 'HorizonSDK/HorizonSDK.framework'
end
