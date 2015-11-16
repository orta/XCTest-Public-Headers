//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <XCTest/XCTestCase.h>

/*!
 * @class XCTestExpectation
 *
 * @discussion
 * Expectations represent specific conditions in asynchronous testing.
 */
@interface XCTestExpectation : NSObject {
#ifndef __OBJC2__
    id _internalImplementation;
#endif
}

/*!
 * @method -fulfill
 *
 * @discussion
 * Call -fulfill to mark an expectation as having been met. It's an error to call
 * -fulfill on an expectation that has already been fulfilled or when the test case
 * that vended the expectation has already completed.
 */
- (void)fulfill;

@end

/*!
 * @category AsynchronousTesting
 *
 * @discussion
 * This category introduces support for asynchronous testing in XCTestCase. The mechanism
 * allows you to specify one or more "expectations" that will occur asynchronously
 * as a result of actions in the test. Once all expectations have been set, a "wait"
 * API is called that will block execution of subsequent test code until all expected
 * conditions have been fulfilled or a timeout occurs.
 */
@interface XCTestCase (AsynchronousTesting)

/*!
 * @method +expectationWithDescription:
 *
 * @param description
 * This string will be displayed in the test log to help diagnose failures.
 *
 * @discussion
 * Creates and returns an expectation associated with the test case.
 */
- (XCTestExpectation *)expectationWithDescription:(NSString *)description;

/*!
 * @typedef XCWaitCompletionHandler
 * A block to be invoked when a call to -waitForExpectationsWithTimeout:handler: times out or has
 * had all associated expectations fulfilled.
 *
 * @param error
 * If the wait timed out or a failure was raised while waiting, the error's code
 * will specify the type of failure. Otherwise error will be nil.
 */
typedef void (^XCWaitCompletionHandler)(NSError *error);

/*!
 * @method -waitForExpectationsWithTimeout:handler:
 *
 * @param timeout
 * The amount of time within which all expectations must be fulfilled.
 *
 * @param handlerOrNil
 * If provided, the handler will be invoked both on timeout or fulfillment of all
 * expectations. Timeout is always treated as a test failure.
 *
 * @discussion
 * -waitForExpectationsWithTimeout:handler: creates a point of synchronization in the flow of a
 * test. Only one -waitForExpectationsWithTimeout:handler: can be active at any given time, but
 * multiple discrete sequences of { expectations -> wait } can be chained together.
 *
 * -waitForExpectationsWithTimeout:handler: runs the run loop while handling events until all expectations
 * are fulfilled or the timeout is reached. Clients should not manipulate the run
 * loop while using this API.
 */
- (void)waitForExpectationsWithTimeout:(NSTimeInterval)timeout handler:(XCWaitCompletionHandler)handlerOrNil;

#pragma mark Convenience APIs

/*!
 * @method keyValueObservingExpectationForObject:keyPath:expectedValue:
 *
 * @discussion
 * A convenience method for asynchronous tests that use Key Value Observing to detect changes
 * to values on an object. This variant takes an expected value and observes changes on the object
 * until the keyPath's value matches the expected value using -[NSObject isEqual:]. If
 * other comparisions are needed, use the variant below that takes a handler block.
 *
 * @param objectToObserve
 * The object to observe.
 *
 * @param keyPath
 * The key path to observe.
 *
 * @param expectedValue
 * Expected value of the keyPath for the object. The expectation will fulfill itself when the
 * keyPath is equal, as tested using -[NSObject isEqual:]. If nil, the expectation will be
 * fulfilled by the first change to the key path of the observed object.
 *
 * @return
 * Creates and returns an expectation associated with the test case.
 */
- (XCTestExpectation *)keyValueObservingExpectationForObject:(id)objectToObserve keyPath:(NSString *)keyPath expectedValue:(id)expectedValue;

/*!
 * @typedef
 * A block to be invoked when a change is observed for the keyPath of the observed object.
 *
 * @param observedObject
 * The observed object, provided to avoid block capture issues.
 *
 * @param change
 * The KVO change dictionary.
 *
 * @return
 * Return YES if the expectation is fulfilled, NO if it is not.
 */
typedef BOOL (^XCKeyValueObservingExpectationHandler)(id observedObject, NSDictionary *change);

/*!
 * @method keyValueObservingExpectationForObject:keyPath:handler:
 *
 * @discussion
 * Variant of the convenience for tests that use Key Value Observing. Takes a handler
 * block instead of an expected value. Every KVO change will run the handler block until
 * it returns YES (or the wait times out). Returning YES from the block will fulfill the
 * expectation. XCTAssert and related APIs can be used in the block to report a failure.
 *
 * @param objectToObserve
 * The object to observe.
 *
 * @param keyPath
 * The key path to observe.
 *
 * @param handlerOrNil
 * Optional handler, /see XCKeyValueObservingExpectationHandler. If not provided, the expectation will
 * be fulfilled by the first change to the key path of the observed object.
 *
 * @return
 * Creates and returns an expectation associated with the test case.
 */
- (XCTestExpectation *)keyValueObservingExpectationForObject:(id)objectToObserve keyPath:(NSString *)keyPath handler:(XCKeyValueObservingExpectationHandler)handlerOrNil;

/*!
 * @typedef
 * A block to be invoked when a notification matching the specified name is observed
 * from the object.
 *
 * @param notification
 * The notification object.
 *
 * @return
 * Return YES if the expectation is fulfilled, NO if it is not.
 */
typedef BOOL (^XCNotificationExpectationHandler)(NSNotification *notification);

/*!
 * @method expectationForNotification:object:handler:
 *
 * @discussion
 * A convenience method for asynchronous tests that observe NSNotifications.
 *
 * @param notificationName
 * The notification to register for.
 *
 * @param objectToObserve
 * The object to observe.
 *
 * @param handlerOrNil
 * Optional handler, /see XCNotificationExpectationHandler. If not provided, the expectation
 * will be fulfilled by the first notification matching the specified name from the
 * observed object.
 *
 * @return
 * Creates and returns an expectation associated with the test case.
 */
- (XCTestExpectation *)expectationForNotification:(NSString *)notificationName object:(id)objectToObserve handler:(XCNotificationExpectationHandler)handlerOrNil;

@end

