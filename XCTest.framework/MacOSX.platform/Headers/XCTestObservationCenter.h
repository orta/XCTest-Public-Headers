//
// Copyright (c) 2013-2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <XCTest/XCTestObservation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 * @class XCTestObservationCenter
 *
 * The XCTestObservationCenter distributes information about the progress of test runs to registered observers. Observers
 * can be any object conforming to the XCTestObservation protocol.
 */
@interface XCTestObservationCenter : NSObject {
#ifndef __OBJC2__
@private
    id _internalImplementation;
#endif
}

/*!
 * @method +sharedTestObservationCenter
 *
 * @return The shared XCTestObservationCenter singleton instance.
 */
+ (XCTestObservationCenter *)sharedTestObservationCenter;

/*!
 * @method -addTestObserver:
 *
 * Register an object conforming to XCTestObservation as an observer for the current test session. Observers may be added
 * at any time, but will not receive events that occurred before they were registered. The observation center maintains a strong
 * reference to observers.
 *
 * Events may be delivered to observers in any order - given observers A and B, A may be notified of a test failure before
 * or after B. Any ordering dependencies or serialization requirements must be managed by clients.
 */
- (void)addTestObserver:(id <XCTestObservation>)testObserver;

/*!
 * @method -removeTestObserver:
 *
 * Unregister an object conforming to XCTestObservation as an observer for the current test session.
 */
- (void)removeTestObserver:(id <XCTestObservation>)testObserver;

@end

NS_ASSUME_NONNULL_END
