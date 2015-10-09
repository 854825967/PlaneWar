#include "DataManager.h"
#include <sqlite3/include/sqlite3.h>

bool DataManager::initDB(const char * uid) {
    NSArray * path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString * dbFile = [[path objectAtIndex:0] stringByAppendingPathComponent:[NSString stringWithFormat:@"db@%@.sql", [NSString stringWithUTF8String:uid]]];
    
    if (sqlite3_open([dbFile UTF8String], &g_db) == SQLITE_OK && initTable()) {
        NSLog(@"open sqlite db ok.");
        return true;
    } else {
        NSLog( @"can not open sqlite db " );
        //close database
        sqlite3_close(g_db);
        g_db = nil;
        return false;
    }
}
