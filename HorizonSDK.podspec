Pod::Spec.new do |s|
  s.name             = 'HorizonSDK'
  s.version          = '1.7'
  s.license          =  { :type => 'Commercial', :text => "See https://horizon.camera/sdk/termsofservice" }
  s.homepage         = 'https://horizon.camera/sdk/'
  s.documentation_url = 'https://horizon.camera/sdk/docs/'
  s.summary          = 'Horizon SDK makes it easy to integrate video and photo recording capabilities to your mobile apps.'
  s.authors          = 'Horizon Video Technologies Inc.'

  s.source           = { :git => "https://github.com/HorizonCamera/HorizonSDK-iOS.git", :tag => s.version.to_s }

  s.platform = :ios
  
  s.ios.deployment_target = '7.1'
  
  s.requires_arc = true

  s.preserve_paths = 'README.md'

  s.vendored_frameworks = 'HorizonSDK/HorizonSDK.framework'
  s.resource = 'HorizonSDK/HorizonSDK.bundle'

  s.frameworks = 'CoreMotion'
  s.libraries = 'ObjC', 'stdc++'
end
