//
//  SPRuleFilterController.h
//  sequel-pro
//
//  Created by Max Lohrmann on 04.05.18.
//  Copyright (c) 2018 Max Lohrmann. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//
//  More info at <https://github.com/sequelpro/sequelpro>

@class SPTableData;
@class SPDatabaseDocument;
@class SPTablesList;
@class SPContentFilterManager;

NSString * const SPRuleFilterHeightChangedNotification;

@interface SPRuleFilterController : NSObject {
	IBOutlet NSRuleEditor *filterRuleEditor;
	IBOutlet SPTableData *tableDataInstance;
	IBOutlet SPDatabaseDocument *tableDocumentInstance;
	IBOutlet SPTablesList *tablesListInstance;
	IBOutlet NSView *tableContentViewBelow;
	IBOutlet NSButton *filterButton;
	IBOutlet NSButton *addFilterButton;
	IBOutlet NSButton *resetButton;

	NSMutableArray *columns;
	NSMutableDictionary *contentFilters;
	NSMutableDictionary *numberOfDefaultFilters;

	id _modelContainer; // private class

	SPContentFilterManager *contentFilterManager;

	CGFloat preferredHeight;
	
	id __unsafe_unretained target;
	SEL action;

	BOOL enabled;
	
	NSUInteger opNodeCacheVersion;
	BOOL isDoingChangeCausedOutsideOfRuleEditor;
	NSInteger previousRowCount;
}

/**
 * Returns the rule editor view that is managed by this object
 *
 * SHOULD be called on the UI thread, or results may be inconsistent!
 */
- (NSRuleEditor *)view;

/**
 * Makes the first NSTextField found in the rule editor the first responder
 *
 * MUST BE CALLED ON THE UI THREAD!
 */
- (void)focusFirstInputField;

/**
 * Will reconfigure the columns of the rule editor from the given array.
 * Call with nil to reset the editor to its initial empty state.
 * Existing rows will be removed in any case!
 *
 * MUST BE CALLED ON THE UI THREAD!
 */
- (void)setColumns:(NSArray *)dataColumns;

/**
 * Converts the current filter expression displayed in the UI into an
 * SQL string suitable for use in a WHERE clause.
 *
 * @param isBINARY Indicates that the filter should use the BINARY qualifier for ignoring
 *                 collations during search.
 * @param err      Upon return contains and object describing why the SQL conversion failed,
 *                 if it failed or nil, if no errors occurred.
 * @return         The SQL filter string. Will always be non-null unless an error occurred.
 *                 Can be an empty string (if no filters exist or all are disabled)!
 *
 * MUST BE CALLED ON THE UI THREAD!
 */
- (NSString *)sqlWhereExpressionWithBinary:(BOOL)isBINARY error:(NSError **)err;

/**
 * Returns the current filter configuration in a serialized form that can be exported and
 * reapplied later.
 *
 * MUST BE CALLED ON THE UI THREAD!
 */
- (NSDictionary *)serializedFilter;

/**
 * Restores the filter rule configuration from a given dictionary.
 * The current column configuration must match the schema that was used when generating
 * the serialized data, otherwise the invalid rules will be ignored.
 *
 * @param serialized A dictionary previously generated by calling -serializedFilter.
 * @return A serialized filter
 *
 * MUST BE CALLED ON THE UI THREAD!
 */
- (void)restoreSerializedFilters:(NSDictionary *)serialized;

/**
 * Create a serialized filter from a given column, operator and operand.
 * This is used when navigating foreign key links between tables to create the filter for the target table.
 *
 * @param colName Name of the column to filter (left side operand)
 * @param opName  Name of the filter (operator)
 * @param values  The values to filter with (right side operand)
 * @return A serialized filter
 *
 * This method is thread-safe.
 */
+ (NSDictionary *)makeSerializedFilterForColumn:(NSString *)colName operator:(NSString *)opName values:(NSArray *)values;

/**
 * The view height the rule editor needs in order to not have to resort to scrollbars
 *
 * SHOULD be called on the UI thread, or results may be inconsistent!
 */
@property (readonly, assign, nonatomic) CGFloat preferredHeight;

/**
 * Indicates whether the rule editor has no filter expressions
 *
 * SHOULD be called on the UI thread, or results may be inconsistent!
 */
- (BOOL)isEmpty;

/**
 * Adds a new row to the rule editor
 *
 * MUST BE CALLED ON THE UI THREAD!
 */
- (void)addFilterExpression;

/**
 * Used when the rule editor wants to trigger filtering
 *
 * SHOULD be called on the UI thread, or results may be inconsistent!
 */
@property (unsafe_unretained, nonatomic) id target;
@property (assign, nonatomic) SEL action;

- (BOOL)isEnabled;
- (void)setEnabled:(BOOL)enabled;

@end
