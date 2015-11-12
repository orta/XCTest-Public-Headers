//
// Copyright (c) 2013 Apple Inc. All rights reserved.
//
// Copyright (c) 1997-2005, Sen:te (Sente SA).  All rights reserved.
//
// Use of this source code is governed by the following license:
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// (1) Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
// 
// (2) Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL Sente SA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Note: this license is equivalent to the FreeBSD license.
// 
// This notice may not be removed from this file.

#import <XCTest/XCTestCase.h>
#import <XCTest/XCTestDefines.h>

XCT_EXPORT void _XCTFailureHandler (XCTestCase *test, BOOL expected, const char *filePath, NSUInteger lineNumber, NSString * condition, NSString * format, ...) NS_FORMAT_FUNCTION(6,7);

#define _XCTRegisterFailure(condition, format...) \
({ \
    _XCTFailureHandler(self, YES, __FILE__, __LINE__, condition, @"" format); \
})

#pragma mark -

typedef NS_OPTIONS(NSUInteger, _XCTAssertionType) {
    _XCTAssertion_Fail,
    _XCTAssertion_Nil,
    _XCTAssertion_NotNil,
    _XCTAssertion_EqualObjects,
    _XCTAssertion_NotEqualObjects,
    _XCTAssertion_Equal,
    _XCTAssertion_NotEqual,
    _XCTAssertion_EqualWithAccuracy,
    _XCTAssertion_NotEqualWithAccuracy,
    _XCTAssertion_True,
    _XCTAssertion_False,
    _XCTAssertion_Throws,
    _XCTAssertion_ThrowsSpecific,
    _XCTAssertion_ThrowsSpecificNamed,
    _XCTAssertion_NoThrow,
    _XCTAssertion_NoThrowSpecific,
    _XCTAssertion_NoThrowSpecificNamed
};

XCT_EXPORT NSString * _XCTFailureFormat (_XCTAssertionType assertionType, NSUInteger formatIndex);

#define _XCTFailureDescription(assertion_type, uncaught_exception, format...) \
({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wformat-nonliteral\"") \
    [NSString stringWithFormat:_XCTFailureFormat(assertion_type, uncaught_exception), @"" format]; \
    _Pragma("clang diagnostic pop") \
})

#pragma mark -

XCT_EXPORT NSString * _XCTDescriptionForValue (NSValue *value);

#pragma mark -

#define _XCTPrimitiveFail(format...) \
({ \
    _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_Fail, 0),format); \
})

#define _XCTPrimitiveAssertNil(a1, format...) \
({ \
    @try { \
        id a1value = (a1); \
        if (a1value != nil) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_Nil, 0, @#a1, a1value),format); \
        } \
    }\
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_Nil, 1, @#a1, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertNotNil(a1, format...) \
({ \
    @try { \
        id a1value = (a1); \
        if (a1value == nil) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotNil, 0, @#a1),format); \
        } \
    }\
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotNil, 1, @#a1, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertEqualObjects(a1, a2, format...) \
({ \
    @try { \
        id a1value = (a1); \
        id a2value = (a2); \
        if ((a1value != a2value) && ![a1value isEqual:a2value]) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_EqualObjects, 0, @#a1, @#a2, a1value, a2value),format); \
        } \
    }\
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_EqualObjects, 1, @#a1, @#a2, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertNotEqualObjects(a1, a2, format...) \
({ \
    @try { \
        id a1value = (a1); \
        id a2value = (a2); \
        if ((a1value == a2value) || [a1value isEqual:a2value]) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotEqualObjects, 0, @#a1, @#a2, a1value, a2value),format); \
        } \
    }\
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotEqualObjects, 1, @#a1, @#a2, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertEqual(a1, a2, format...) \
({ \
    @try { \
        __typeof__(a1) a1value = (a1); \
        __typeof__(a2) a2value = (a2); \
        NSValue *a1encoded = [NSValue value:&a1value withObjCType:@encode(__typeof__(a1))]; \
        NSValue *a2encoded = [NSValue value:&a2value withObjCType:@encode(__typeof__(a2))]; \
        float aNaN = NAN; \
        NSValue *aNaNencoded = [NSValue value:&aNaN withObjCType:@encode(__typeof__(aNaN))]; \
        if ([a1encoded isEqualToValue:aNaNencoded] || [a2encoded isEqualToValue:aNaNencoded] || ![a1encoded isEqualToValue:a2encoded]) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_Equal, 0, @#a1, @#a2, _XCTDescriptionForValue(a1encoded), _XCTDescriptionForValue(a2encoded)),format); \
        } \
    } \
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_Equal, 1, @#a1, @#a2, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertNotEqual(a1, a2, format...) \
({ \
    @try { \
        __typeof__(a1) a1value = (a1); \
        __typeof__(a2) a2value = (a2); \
        NSValue *a1encoded = [NSValue value:&a1value withObjCType:@encode(__typeof__(a1))]; \
        NSValue *a2encoded = [NSValue value:&a2value withObjCType:@encode(__typeof__(a2))]; \
        float aNaN = NAN; \
        NSValue *aNaNencoded = [NSValue value:&aNaN withObjCType:@encode(__typeof__(aNaN))]; \
        if ((NO == [a1encoded isEqualToValue:aNaNencoded]) && (NO == [a2encoded isEqualToValue:aNaNencoded]) && [a1encoded isEqualToValue:a2encoded]) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotEqual, 0, @#a1, @#a2, _XCTDescriptionForValue(a1encoded), _XCTDescriptionForValue(a2encoded)),format); \
        } \
    } \
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotEqual, 1, @#a1, @#a2, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertEqualWithAccuracy(a1, a2, accuracy, format...) \
({ \
    @try { \
        __typeof__(a1) a1value = (a1); \
        __typeof__(a2) a2value = (a2); \
        __typeof__(accuracy) accuracyvalue = (accuracy); \
        if (isnan(a1value) || isnan(a2value) || ((MAX(a1value, a2value) - MIN(a1value, a2value)) > accuracyvalue)) { \
            NSValue *a1encoded = [NSValue value:&a1value withObjCType:@encode(__typeof__(a1))]; \
            NSValue *a2encoded = [NSValue value:&a2value withObjCType:@encode(__typeof__(a2))]; \
            NSValue *accuracyencoded = [NSValue value:&accuracyvalue withObjCType:@encode(__typeof__(accuracy))]; \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_EqualWithAccuracy, 0, @#a1, @#a2, @#accuracy, _XCTDescriptionForValue(a1encoded), _XCTDescriptionForValue(a2encoded), _XCTDescriptionForValue(accuracyencoded)), ## format); \
        } \
    } \
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_EqualWithAccuracy, 1, @#a1, @#a2, @#accuracy, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertNotEqualWithAccuracy(a1, a2, accuracy, format...) \
({ \
    @try { \
        __typeof__(a1) a1value = (a1); \
        __typeof__(a2) a2value = (a2); \
        __typeof__(accuracy) accuracyvalue = (accuracy); \
        if ((0 != isnan(a1value)) && (0 != isnan(a2value)) && ((MAX(a1value, a2value) - MIN(a1value, a2value)) <= accuracyvalue)) { \
            NSValue *a1encoded = [NSValue value:&a1value withObjCType:@encode(__typeof__(a1))]; \
            NSValue *a2encoded = [NSValue value:&a2value withObjCType:@encode(__typeof__(a2))]; \
            NSValue *accuracyencoded = [NSValue value:&accuracyvalue withObjCType:@encode(__typeof__(accuracy))]; \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotEqualWithAccuracy, 0, @#a1, @#a2, @#accuracy, _XCTDescriptionForValue(a1encoded), _XCTDescriptionForValue(a2encoded), _XCTDescriptionForValue(accuracyencoded)), ## format); \
        } \
    } \
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NotEqualWithAccuracy, 1, @#a1, @#a2, @#accuracy, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertTrue(expression, format...) \
({ \
    @try { \
        BOOL _evaluatedExpression = !!(expression); \
        if (!_evaluatedExpression) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_True, 0, @#expression),format); \
        } \
    } \
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_True, 1, @#expression, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertFalse(expression, format...) \
({ \
    @try { \
        BOOL _evaluatedExpression = !!(expression); \
        if (_evaluatedExpression) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_False, 0, @#expression),format); \
        } \
    } \
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_False, 1, @#expression, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertThrows(expression, format...) \
({ \
    BOOL __caughtException = NO; \
    @try { \
        (expression); \
    } \
    @catch (id exception) { \
        __caughtException = YES; \
    }\
    if (!__caughtException) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_Throws, 0, @#expression),format); \
    } \
})

#define _XCTPrimitiveAssertThrowsSpecific(expression, exception_class, format...) \
({ \
    BOOL __caughtException = NO; \
    @try { \
        (expression); \
    } \
    @catch (exception_class *exception) { \
        __caughtException = YES; \
    }\
    @catch (id exception) { \
        __caughtException = YES; \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_ThrowsSpecific, 0, @#expression, @#exception_class, [exception class], [exception reason]),format); \
    }\
    if (!__caughtException) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_ThrowsSpecific, 1, @#expression, @#exception_class),format); \
    } \
})

#define _XCTPrimitiveAssertThrowsSpecificNamed(expression, exception_class, exception_name, format...) \
({ \
    BOOL __caughtException = NO; \
    @try { \
        (expression); \
    } \
    @catch (exception_class *exception) { \
        __caughtException = YES; \
        if (![exception_name isEqualToString:[exception name]]) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_ThrowsSpecificNamed, 0, @#expression, @#exception_class, exception_name, [exception class], [exception name], [exception reason]),format); \
        } \
    }\
    @catch (id exception) { \
        __caughtException = YES; \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_ThrowsSpecificNamed, 1, @#expression, @#exception_class, exception_name, [exception class], [exception name], [exception reason]),format); \
    }\
    if (!__caughtException) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_ThrowsSpecificNamed, 2, @#expression, @#exception_class, exception_name),format); \
    } \
})

#define _XCTPrimitiveAssertNoThrow(expression, format...) \
({ \
    @try { \
        (expression); \
    } \
    @catch (id exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NoThrow, 0, @#expression, [exception reason]),format); \
    }\
})

#define _XCTPrimitiveAssertNoThrowSpecific(expression, exception_class, format...) \
({ \
    @try { \
        (expression); \
    } \
    @catch (exception_class *exception) { \
        _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NoThrowSpecific, 0, @#expression, @#exception_class, [exception class], [exception reason]),format); \
    } \
    @catch (...) { \
        ; \
    }\
})

#define _XCTPrimitiveAssertNoThrowSpecificNamed(expression, exception_class, exception_name, format...) \
({ \
    @try { \
        (expression); \
    } \
    @catch (exception_class *exception) { \
        if ([exception_name isEqualToString:[exception name]]) { \
            _XCTRegisterFailure(_XCTFailureDescription(_XCTAssertion_NoThrowSpecificNamed, 0, @#expression, @#exception_class, exception_name, [exception class], [exception name], [exception reason]),format); \
        } \
    }\
    @catch (...) { \
        ; \
    }\
})
