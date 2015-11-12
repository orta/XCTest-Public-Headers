//
//  Copyright (c) 2014-2015 Apple Inc. All rights reserved.
//

#import <XCTest/XCTestDefines.h>

#if XCT_UI_TESTING_AVAILABLE

#if TARGET_OS_IPHONE

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/*! Represents a device, providing an interface for simulating events involving physical buttons and device state. */
NS_CLASS_AVAILABLE(NA, 9_0)
@interface XCUIDevice : NSObject

/*! The current device. */
+ (XCUIDevice *)sharedDevice;

/*! The orientation of the device. */
@property (nonatomic) UIDeviceOrientation orientation;

@end

NS_ASSUME_NONNULL_END

#endif

#endif

