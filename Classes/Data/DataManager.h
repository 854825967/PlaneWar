#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "Header.h"
#include "Struct.h"
#include <list>

typedef struct sqlite3 sqlite3;

class DataManager {
public:
    static DataManager * getInterface();
    bool initDB(const char * uid);
    
    bool getUidPtoken(string & uid, string & ptoken);
    void saveUidPtoken(const char * uid, const char * pToken);
    
    //equip data interface

    
    ~DataManager();
    
private:
    DataManager();
    bool initTable();
private:
    static DataManager * s_pSelf;
};

extern DataManager * g_pDataManager;
extern sqlite3 * g_db;

#endif //DATA_MANAGER_H
