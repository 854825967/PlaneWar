#include "EquipManager.h"
#include "Header.h"
#include "DataManager.h"
#include "GameConfig.h"
#include "CEventEngine.h"
#include "CHttpEngine.h"
#include "SInterfaceDefine.h"
#include "sqlite3/include/sqlite3.h"

EquipManager * g_pEquipManager = EquipManager::getInterface();

EquipManager * EquipManager::s_pSelf = NULL;

EquipManager::EquipManager() {
    
}

EquipManager::~EquipManager() {
    s_pSelf = NULL;
}

void EquipManager::OnHttpRes(const s32 id, const Json::Value * root,const void * context, const s32 size) {
    switch (id) {
        case HTTP_REQUEST_ID_REPLACEEQUIP:
        {
            if (NULL ==  root) {
                LOG("replace equip error");
                return;
            }

            s32 nCode = (*root)["code"].asInt();
            switch (nCode) {
                case ERROR_CODE::ERROR_CODE_NO_ERROR:
                {
                    SEquip * pEquip = (SEquip *)context;
                    equip(pEquip->id, pEquip->type);
                    break;
                }
            }
            
            g_pEventEngine->BundlerCall(BUND_ID_EQUIP_REPLACE_EQUIP_RES, &nCode, sizeof(nCode));

            break;
        }
        case HTTP_REQUEST_ID_SELL:
        {
            if (NULL ==  root) {
                LOG("sell equip error");
                return;
            }
            
            s32 nCode = (*root)["code"].asInt();
            switch (nCode) {
                case ERROR_CODE::ERROR_CODE_NO_ERROR:
                {
                    const ID_ARRAY * pVector = (const ID_ARRAY *)context;
                    deleteEquip(*pVector);
                    s32 nMoney = (*root)["money"].asInt();
                    g_money = nMoney;
                    break;
                }
            }
            
            g_pEventEngine->BundlerCall(BUND_ID_EQUIP_SELL_RES, &nCode, sizeof(nCode));
            break;
        }
        case HTTP_REQUEST_ID_LEVEL_UP:
        {
            if (NULL == root) {
                LOG("level up error");
                return;
            }
            
            s32 nCode = (*root)["code"].asInt();
            switch (nCode) {
                case ERROR_CODE::ERROR_CODE_NO_ERROR:
                {
                    const ID_ARRAY * pVector = (const ID_ARRAY *)context;
                    deleteEquip(*pVector);
                    s32 nMoney = (*root)["money"].asInt();
                    s32 eid = (*root)["eid"].asInt();
                    s32 level = (*root)["level"].asInt();
                    s32 exp = (*root)["exp"].asInt();
                    updateEquipLevelExp(eid, level, exp);
                    g_money -= nMoney;
                    break;
                }
            }
            
            g_pEventEngine->BundlerCall(BUND_ID_LEVEL_UP_EQUIP, &nCode, sizeof(nCode));
            break;
        }
        default:
            break;
    }
}

bool EquipManager::levelupEquip(const SEquip &equip, const ID_ARRAY &materials) {
    CCASSERT(materials.size() != 0, "no material u level up a jb");
    
    string eids = IntAsString(equip.id);
    for (s32 i=0; i<materials.size(); i++) {
        eids += "," + IntAsString(materials[i]);
    }
    
    char szUrl[256] = {0};
    SafeSprintf(szUrl, sizeof(szUrl), SERVER::levelup_url, g_uid.c_str(), g_token.c_str(), eids.c_str());
    g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_LEVEL_UP, szUrl, this, &materials, materials.length());
    LOG("level up url : %s", szUrl);
}

bool EquipManager::updateEquipLevelExp(const s32 id, const s32 level, const s32 exp) {
    char * errorMsg = NULL;
    const char * sql = "update equip set level=%d,exp=%d where id=%d";
    char szSql[128] = {0};
    SafeSprintf(szSql, sizeof(szSql), sql, level, exp, id);

    bool res = (sqlite3_exec(g_db, szSql, NULL, NULL, &errorMsg) ==  SQLITE_OK);
    CCASSERT(res, "救命啊,数据库爆炸啦");
    return res;
}

bool EquipManager::getGrooveEquip(const s8 type, SEquip &equip) {
    bool res = false;
    
    const char * error = NULL;
    static char * model = "select * from equip where isequiped=1 and type=%d";
    char szSql[128] = {0};
    SafeSprintf(szSql, sizeof(szSql), model, type);
    
    sqlite3_stmt * statement;
    if (sqlite3_prepare(g_db, szSql, -1, &statement, &error) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) { //SQLITE_OK SQLITE_ROW
            equip.id = sqlite3_column_int(statement, 0);
            equip.type = sqlite3_column_int(statement, 1);
            equip.level = sqlite3_column_int(statement, 2);
            equip.exp = sqlite3_column_int(statement, 3);
            equip.model = (const char  * )sqlite3_column_text(statement, 4);
            equip.isequiped = sqlite3_column_int(statement, 5);
            equip.islocked = isLocked(equip.id);
            
            res = true;
        }
    } else {
        LOG("db error %s", error);
    }
    sqlite3_finalize(statement);
    
    CCASSERT(res, "select groove equip error");
    return res;
}

void EquipManager::getGrooveEquips(EQUIP_LIST & equips) {
    const char * error = NULL;
    char * sql = "select * from equip where isequiped=1";
    
    sqlite3_stmt * statement;
    if (sqlite3_prepare(g_db, sql, -1, &statement, &error) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) { //SQLITE_OK SQLITE_ROW
            SEquip equip;
            equip.id = sqlite3_column_int(statement, 0);
            equip.type = sqlite3_column_int(statement, 1);
            equip.level = sqlite3_column_int(statement, 2);
            equip.exp = sqlite3_column_int(statement, 3);
            equip.model = (const char  * )sqlite3_column_text(statement, 4);
            equip.isequiped = sqlite3_column_int(statement, 5);
            equip.islocked = isLocked(equip.id);
            
            equips.push_back(equip);
        }
    } else {
        LOG("db error %s", error);
    }
    sqlite3_finalize(statement);
}

bool EquipManager::queryEquips(EQUIP_VECTOR & equips, const s8 type, const char * orderby, const char * sort) {
    CCASSERT(type <= EQUIP_TYPE_ALL && orderby != NULL && sort != NULL, "check ur args, fuck u");
    equips.clear();
    char szSql[128] = {0};
    const char * szModel = NULL;
    if (type == EQUIP_TYPE_ALL) {
        szModel = "select * from equip order by %s %s";
        SafeSprintf(szSql, sizeof(szSql), szModel, orderby, sort);
    } else {
        szModel = "select * from equip where type=%d order by %s %s";
        SafeSprintf(szSql, sizeof(szSql), szModel, type, orderby, sort);
    }
    const char * error = NULL;
    sqlite3_stmt * statement;
    if (sqlite3_prepare(g_db, szSql, -1, &statement, &error) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) { //SQLITE_OK SQLITE_ROW
            SEquip equip;
            equip.id = sqlite3_column_int(statement, 0);
            equip.type = sqlite3_column_int(statement, 1);
            equip.level = sqlite3_column_int(statement, 2);
            equip.exp = sqlite3_column_int(statement, 3);
            equip.model = (const char  * )sqlite3_column_text(statement, 4);
            equip.isequiped = sqlite3_column_int(statement, 5);
            equip.islocked = isLocked(equip.id);
            
            equips.push_back(equip);
        }
    } else {
        return false;
    }
    sqlite3_finalize(statement);
    
    return true;
}

bool EquipManager::saveEquips(const EQUIP_LIST & equips) {
    char * errorMsg = NULL;
    if (sqlite3_exec(g_db, "delete from equip;", NULL, NULL, &errorMsg) !=  SQLITE_OK) {
        LOG("can not delete from equip, error %s", errorMsg);
        return false;
    }
    
    sqlite3_exec(g_db, "BEGIN TRANSACTION", 0, 0, 0);
    const char  * sqlStatement = "INSERT INTO equip (id, type, level, exp, model, isequiped) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt * compiledStatement;

    bool hasError = false;
    if (sqlite3_prepare(g_db, sqlStatement, -1, &compiledStatement, NULL) ==  SQLITE_OK) {
        EQUIP_LIST::const_iterator citor = equips.cbegin();
        EQUIP_LIST::const_iterator ciend = equips.cend();
        for(;citor !=  ciend; citor ++) {
            sqlite3_bind_int(compiledStatement, 1, citor->id);
            sqlite3_bind_int(compiledStatement, 2, citor->type);
            sqlite3_bind_int(compiledStatement, 3, citor->level);
            sqlite3_bind_int(compiledStatement, 4, citor->exp);
            sqlite3_bind_text(compiledStatement, 5, citor->model.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(compiledStatement, 6, citor->isequiped);
            
            if (sqlite3_step(compiledStatement) !=  SQLITE_DONE) {
                LOG("Prepare-error %s", sqlite3_errmsg(g_db));
                hasError = true;
            }
            
            sqlite3_reset(compiledStatement);
        }
        
        sqlite3_finalize(compiledStatement);
        if(!hasError) {
            LOG("save equips ok, commit");
            CCASSERT(sqlite3_exec(g_db, "COMMIT", NULL, NULL, &errorMsg) ==  SQLITE_OK, "save equips COMMIT error");
        } else {
            LOG("save equips error, rollback");
            CCASSERT(sqlite3_exec(g_db, "ROLLBACK", NULL, NULL, &errorMsg) ==  SQLITE_OK, "save equips rollback error");
        }
    }

    CCASSERT(!hasError, "save equip error");
    return !hasError;
}

bool EquipManager::sellEquips(const ID_ARRAY & id_array) {
    if (0 != id_array.size()) {
        string ids;
        SEquip equip;
        s32 index=0;
        queryEquip(id_array[index], equip);
        ids = IntAsString(id_array[index]);
        index++;
        
        while (index < id_array.size()) {
            CCASSERT(queryEquip(id_array[index], equip)
                     && equip.isequiped != IS_EQUIPED
                     && equip.islocked != LOCKED,
                     "干你妹,逼老子打中文注释骂你个逼玩意,赶紧去查你的装备管理逻辑,没用的费ID,你卖你MB啊...");
            ids += "," + IntAsString(id_array[index]);
            
            index++;
        }
        
        char szUrl[1024] = {0};
        SafeSprintf(szUrl, sizeof(szUrl), SERVER::sellequips_url, g_uid.c_str(), g_token.c_str(), ids.c_str());
        LOG("%s", szUrl);
        g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_SELL, szUrl, this, &id_array, id_array.length());
    }
    
    
    return true;
}

bool EquipManager::queryEquip(const s32 id, SEquip & equip) {
    char szSql[128] = {0};
    SafeSprintf(szSql, sizeof(szSql), "select * from equip where id=%d", id);
    sqlite3_stmt * statement;
    bool res = false;
    if (sqlite3_prepare(g_db, szSql, -1, &statement, NULL) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) { //SQLITE_OK SQLITE_ROW
            equip.id = sqlite3_column_int(statement, 0);
            equip.type = sqlite3_column_int(statement, 1);
            equip.level = sqlite3_column_int(statement, 2);
            equip.exp = sqlite3_column_int(statement, 3);
            equip.model = (const char  * )sqlite3_column_text(statement, 4);
            equip.isequiped = sqlite3_column_int(statement, 5);
            equip.islocked = isLocked(equip.id);
            res = true;
        }
    }
    sqlite3_finalize(statement);
    return res;
}

bool EquipManager::replaceEquip(const s32 id, const s8 type) {
    CCASSERT(type < EQUIP_TYPE_CAN_EQUIPED, "check ur equip type");
    SEquip equip;
    if (!queryEquip(id, equip) || equip.isequiped || equip.type != type) {
        CCASSERT(false, "fuck, check equip data manager logic, right now");
        return false;
    }
    
    char szUrl[256] = {0};
    SafeSprintf(szUrl, sizeof(szUrl), SERVER::replaceequip_url, g_uid.c_str(), g_token.c_str(), id);
    g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_REPLACEEQUIP, szUrl, this, &equip, sizeof(equip));
    return true;
}

bool EquipManager::saveEquip(const SEquip & equip) {
    char * errorMsg = NULL;
    const char * sql = "insert into equip (id, type, level, exp, model, isequiped) values(%d, %d, %d, %d, \"%s\", %d)";
    char szSql[256] = {0};
    SafeSprintf(szSql, sizeof(szSql), sql, equip.id, equip.type, equip.level, equip.exp, equip.model.c_str(), equip.isequiped);
    bool res = (sqlite3_exec(g_db, szSql, NULL, NULL, &errorMsg) ==  SQLITE_OK);
    CCASSERT(res, "救命啊,杀人啦");
    return res;
}

bool EquipManager::deleteEquip(const ID_ARRAY & equips) {
    bool noError = true;
    
    const char  * sqlStatement = "delete from equip where id=?";
    sqlite3_exec(g_db, "BEGIN TRANSACTION", 0, 0, 0);
    sqlite3_stmt * compiledStatement;
    
    if (sqlite3_prepare(g_db, sqlStatement, -1, &compiledStatement, NULL) ==  SQLITE_OK) {
        
        for(s32 i=0; i<equips.size(); i ++) {
            sqlite3_bind_int(compiledStatement, 1, equips[i]);
            if (sqlite3_step(compiledStatement) !=  SQLITE_DONE) {
                noError = false;
                LOG("Prepare-error %s", sqlite3_errmsg(g_db));
                break;
            }
            LOG("删除装备 id %d", equips[i]);
            sqlite3_reset(compiledStatement);
        }
        char * error = NULL;
        if(noError) {
            noError = (sqlite3_exec(g_db, "COMMIT", NULL, NULL, &error) ==  SQLITE_OK);
        } else {
            CCASSERT(sqlite3_exec(g_db, "ROLLBACK", NULL, NULL, &error) ==  SQLITE_OK, "救命啊,杀人啦");
        }
        sqlite3_finalize(compiledStatement);
    }
    CCASSERT(noError, "check ur logic");
    return noError;
}

s32 EquipManager::countCapacity(const SEquip &equip) {
    switch (equip.type) {
        case EQUIP_TYPE_PLANE:
        {
            const PlaneConfig * pConfig = g_pGameConfig->getPlaneConfig(equip.model);
            return pConfig->atk + (equip.level - 1) * pConfig->atk/20;
        }
        case EQUIP_TYPE_WEAPON:
        {
            const WeaponConfig * pConfig = g_pGameConfig->getWeaponConfig(equip.model);
            return pConfig->atk + (equip.level - 1) * pConfig->atk/20;
        }
        case EQUIP_TYPE_WINGMAN:
        {
            const WingmanConfig * pConfig = g_pGameConfig->getWingmanConfig(equip.model);
            return pConfig->atk + (equip.level - 1) * pConfig->atk/20;
        }
        case EQUIP_TYPE_ARMOR:
        {
            const ArmorConfig * pConfig = g_pGameConfig->getArmorConfig(equip.model);
            return (pConfig->armor + (equip.level - 1) * pConfig->armor/200)/20;
        }
        default:
            break;
    }
}

s32 EquipManager::countCapacity(const EQUIP_LIST & equips) {
    s32 nCapacity = 0;
    for (EQUIP_LIST::const_iterator itor = equips.cbegin(); itor != equips.cend(); itor++) {
        nCapacity += countCapacity(*itor);
    }

    return nCapacity;
}

s32 EquipManager::levelExp(const s32 level) {
    return level * (level - 1) * (level + 38);
}

s32 EquipManager::provideExp(const s32 level) {
    return levelExp(level) * .8f + 400;
}

bool EquipManager::equip(const s32 id, const s8 type) {
    char * errorMsg = NULL;
    const char * sql = "update equip set isequiped=0 where type=%d; update equip set isequiped=1 where id=%d";
    char szSql[256] = {0};
    SafeSprintf(szSql, sizeof(szSql), sql, type, id);
    if (sqlite3_exec(g_db, szSql, NULL, NULL, &errorMsg) !=  SQLITE_OK) {
        LOG("can not equip, error %s", errorMsg);
        return false;
    } else {
        LOG("equip ok");
    }
    
    return true;
}

bool EquipManager::isLocked(const s32 id) {
    const char * errorMsg = NULL;
    const char * sql = "select * from lock where id = %d";
    char szSql[64] = {0};
    SafeSprintf(szSql, sizeof(szSql), sql, id);
    sqlite3_stmt * statement;
    
    bool res = false;
    if (sqlite3_prepare(g_db, szSql, -1, &statement, &errorMsg) ==  SQLITE_OK) {
        if (sqlite3_step(statement) ==  SQLITE_ROW) { //SQLITE_OK SQLITE_ROW
            CCASSERT(id ==  sqlite3_column_int(statement, 0), "db error unknown");
            res = true;
        }
    } else {
        LOG("db opt error %s", errorMsg);
    }
    sqlite3_finalize(statement);
    
    return res;
}

bool EquipManager::lockEquip(const s32 id) {
    char * errorMsg = NULL;
    const char * sql = "insert into lock (id) values(%d)";
    char szSql[256] = {0};
    SafeSprintf(szSql, sizeof(szSql), sql, id);
    if (sqlite3_exec(g_db, szSql, NULL, NULL, &errorMsg) !=  SQLITE_OK) {
        LOG("can not lock equip, error %s", errorMsg);
        return false;
    }
    
    LOG("lock equip %d", id);
    return true;
}

bool EquipManager::unlockEquip(const s32 id) {
    char * errorMsg = NULL;
    const char * sql = "delete from lock where id = %d";
    char szSql[256] = {0};
    SafeSprintf(szSql, sizeof(szSql), sql, id);
    if (sqlite3_exec(g_db, szSql, NULL, NULL, &errorMsg) !=  SQLITE_OK) {
        LOG("can not unlock equip, error %s", errorMsg);
        return false;
    }
    LOG("unlock equip %d", id);
    
    return true;
}

EquipManager * EquipManager::getInterface() {
    if(NULL ==  s_pSelf) {
        s_pSelf = NEW EquipManager;
    }
    
    return s_pSelf;
}
