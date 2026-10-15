// swift-tools-version:5.9
import PackageDescription

let package = Package(
    name: "HorizonSDK",
    platforms: [.iOS(.v15)],
    products: [.library(name: "HorizonSDK", targets: ["HorizonSDK"])],
    targets: [.binaryTarget(name: "HorizonSDK", path: "HorizonSDK/HorizonSDK.xcframework")]
)
