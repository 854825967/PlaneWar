#include "sqlite3/include/sqlite3.h"
#include "DataManager.h"
#include "Header.h"
#include "Tools.h"
USING_NS_CC;

DataManager * g_pDataManager = DataManager::getInterface();
DataManager * DataManager::s_pSelf = NULL;
sqlite3 * g_db = NULL;

DataManager::DataManager() {
    
}

DataManager::~DataManager() {
    if (g_db) {
        sqlite3_close(g_db);
    }
}

DataManager * DataManager::getInterface() {
    if (NULL == s_pSelf) {
        s_pSelf = NEW DataManager;
    }
    
    return s_pSelf;
}

bool DataManager::getUidPtoken(string & uid, string & ptoken) {
    if (UserDefault::getInstance()->getBoolForKey("inited")) {
        s64 lLastTimeTick = StringAsInt64(UserDefault::getInstance()->getStringForKey("lastLoginTimeTick").c_str());
        if(::GetCurrentTimeTick() - lLastTimeTick > LOGIN_DATA_VALIDITY) {
            return false;
        }
        
        uid = UserDefault::getInstance()->getStringForKey("uid");
        ptoken = UserDefault::getInstance()->getStringForKey("ptoken");
        g_uid = uid;
        g_token = ptoken;
        return true;
    }
    
    return false;
}

bool DataManager::initTable() {
    char * errorMsg = NULL;
    //create equip table
    const char * equipSql = "create table if not exists equip (id integer primary key, type integer, level integer, exp interget, model text, isequiped integer)";
    
    if (sqlite3_exec(g_db, equipSql, NULL, NULL, &errorMsg) != SQLITE_OK) {
        LOG("can not create equip table, error %s", errorMsg);
        return false;
    }
    
    const char * lockSql = "create table if not exists lock (id integer primary key)";
    if (sqlite3_exec(g_db, lockSql, NULL, NULL, &errorMsg) != SQLITE_OK) {
        LOG("can not create lock table, error %s", errorMsg);
        return false;
    }
    
    return true;
}

void DataManager::saveUidPtoken(const char * uid, const char * pToken) {
    g_uid = uid;
    g_token = pToken;
    UserDefault::getInstance()->setBoolForKey("inited", true);
    UserDefault::getInstance()->setStringForKey("lastLoginTimeTick", Int64AsString(::GetCurrentTimeTick()));
    UserDefault::getInstance()->setStringForKey("uid", uid);
    UserDefault::getInstance()->setStringForKey("ptoken", pToken);
    UserDefault::getInstance()->flush();
}

#if defined WIN32 || defined __ANDROID__
#include "Tools.h"

bool DataManager::initDB(const char * uid) {
    char szPath[512] = {0};
    SafeSprintf(szPath, sizeof(szPath), "%sdb@%s.sql", FileUtils::getInstance()->getWritablePath().c_str(), uid);
    ECHO("sql path:%s", szPath);
    if (sqlite3_open(szPath, &g_db) == SQLITE_OK && initTable()) {
    	LOG("open sqlite db ok.");
        return true;
    } else {
    	LOG("can not open sqlite db " );
        //close database
        sqlite3_close(g_db);
        g_db = NULL;
        return false;
    }
}

#endif //__WIN32__
