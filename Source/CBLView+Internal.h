//
//  CBLView+Internal.h
//  CouchbaseLite
//
//  Created by Jens Alfke on 12/8/11.
//  Copyright (c) 2011-2013 Couchbase, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CBLDatabase+Internal.h"
#import "CBLView.h"
#import "CBLQuery.h"
@class CBForestMapReduceIndex;


#define kViewIndexPathExtension @"viewindex"


/** Standard query options for views. */
@interface CBLQueryOptions : NSObject
{
    @public
    const struct CBLGeoRect* bbox;
    unsigned skip;
    unsigned limit;
    unsigned groupLevel;
    CBLContentOptions content;
    BOOL descending;
    BOOL includeDocs;
    BOOL updateSeq;
    BOOL localSeq;
    BOOL inclusiveEnd;
    BOOL reduceSpecified;
    BOOL reduce;                   // Ignore if !reduceSpecified
    BOOL group;
    BOOL fullTextSnippets;
    BOOL fullTextRanking;
    CBLIndexUpdateMode indexUpdateMode;
    CBLAllDocsMode allDocsMode;
}

@property (copy, nonatomic) id startKey;
@property (copy, nonatomic) id endKey;
@property (copy, nonatomic) NSString* startKeyDocID;
@property (copy, nonatomic) NSString* endKeyDocID;
@property (copy, nonatomic) NSArray* keys;
@property (copy, nonatomic) NSString* fullTextQuery;

@end


typedef enum {
    kCBLViewCollationUnicode,
    kCBLViewCollationRaw,
    kCBLViewCollationASCII
} CBLViewCollation;


@interface CBLView ()
{
    @private
    CBLDatabase* __weak _weakDB;
    NSString* _name;
    uint8_t _collation;
    CBLContentOptions _mapContentOptions;
}

- (instancetype) initWithDatabase: (CBLDatabase*)db name: (NSString*)name create: (BOOL)create;

- (void) databaseClosing;

+ (NSString*) fileNameToViewName: (NSString*)fileName;

@property (readonly) CBForestMapReduceIndex* index;

@property (readonly) NSString* mapVersion;

#if DEBUG  // for unit tests only
@property (readonly) NSString* indexFilePath;
- (void) setCollation: (CBLViewCollation)collation;
#endif

@end


@interface CBLView (Internal)

/** Compiles a view (using the registered CBLViewCompiler) from the properties found in a CouchDB-style design document. */
- (BOOL) compileFromProperties: (NSDictionary*)viewProps
                      language: (NSString*)language;

/** Updates the view's index (incrementally) if necessary.
 @return  200 if updated, 304 if already up-to-date, else an error code */
- (CBLStatus) updateIndex;

@end


@interface CBLView (Querying)

/** Queries the view. Does NOT first update the index.
    @param options  The options to use.
    @return  An array of CBLQueryRow. */
- (CBLQueryIteratorBlock) _queryWithOptions: (CBLQueryOptions*)options
                                     status: (CBLStatus*)outStatus;
#if DEBUG
- (NSArray*) dump;
#endif

@end
