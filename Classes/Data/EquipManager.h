#ifndef EQUIPMANAGER_H
#define EQUIPMANAGER_H

#include "Struct.h"
#include "IHttpRes.h"
#include "CHashMap.h"

class EquipManager : IHttpRes {
public:
    static EquipManager * getInterface();
    s32 countCapacity(const EQUIP_LIST & equips);
    s32 countCapacity(const SEquip & equip);
    //从0级升到level所需经验
    s32 levelExp(const s32 level);
    //当前等级素材提供多少经验
    s32 provideExp(const s32 level);

    bool saveEquips(const EQUIP_LIST & equips);
    bool queryEquips(EQUIP_VECTOR & eqiups, const s8 type = EQUIP_TYPE_ALL, const char * orderby = "level", const char * sort = "desc");
    bool updateEquipLevelExp(const s32 id, const s32 level, const s32 exp);
    
    bool replaceEquip(const s32 id, const s8 type);
    bool queryEquip(const s32 id, SEquip & equip);
    
    bool saveEquip(const SEquip & equips);
    bool deleteEquip(const ID_ARRAY & equips);
    bool equip(const s32 id, const s8 type);
    
    bool lockEquip(const s32 id);
    bool unlockEquip(const s32 id);
    bool isLocked(const s32 id);
    
    bool sellEquips(const ID_ARRAY & id_vector);
    void getGrooveEquips(EQUIP_LIST & equips);
    bool getGrooveEquip(const s8 type, SEquip & equip);
    
    bool levelupEquip(const SEquip & equip, const ID_ARRAY & materials);
    
public:
    ~EquipManager();
    
protected:
    EquipManager();
    void initEquips(const EQUIP_LIST & equips);
    void OnHttpRes(const s32 id, const Json::Value * root, const void * context, const s32 size);
    
private:
    static EquipManager * s_pSelf;
};

extern EquipManager * g_pEquipManager;

#endif //EQUIPMANAGER_H
