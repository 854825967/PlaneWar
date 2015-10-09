#include "Weapon.h"
#include "Plane.h"
#include "Ejector.h"
#include "GameConfig.h"
#include "CEventEngine.h"

Weapon * Weapon::create(const SEquip & equip) {
    CCASSERT(equip.type == EQUIP_TYPE_WEAPON, "用你麻痹的其它类型");
    const WeaponConfig * pConfig = g_pGameConfig->getWeaponConfig(equip.model);
    Weapon * pWeapon = NEW Weapon(pConfig);
    pWeapon->m_atk = pConfig->atk + (pConfig->atk/20) * (equip.level - 1);
    CCASSERT(NULL !=  pWeapon, "check ur memory");
    return pWeapon;
}

Weapon::Weapon(const WeaponConfig * pConfig) {
    m_pConfig = pConfig;
    m_bullet_level = BULLET_LEVEL_1;
    
    BIND(BUND_ID_WEAPON_FIRE, this, Weapon::OnCmdFire);
    BIND(BUND_ID_BULLET_LEVEL_UP, this, Weapon::OnCmdBulletUp);
}

Weapon::~Weapon() {
    UNBIND(BUND_ID_WEAPON_FIRE, this, Weapon::OnCmdFire);
    UNBIND(BUND_ID_BULLET_LEVEL_UP, this, Weapon::OnCmdBulletUp);
}

void Weapon::OnCmdBulletUp(void *context, const s32 size) {
    if (getParent() == context) {
        m_bullet_level++;
        if (m_bullet_level >= BULLET_LEVEL_COUNT) {
            m_bullet_level = BULLET_LEVEL_1;
        }
        
        if (m_bullet_level == BULLET_LEVEL_GO) {
            m_bullet_level = BULLET_LEVEL_1;
        }
    }
}

void Weapon::OnCmdFire(void * context, const s32 size) {
    if (getParent() == context) {
        Plane * plane = (Plane *)context;
        BULLET_MAP::const_iterator itor = m_pConfig->bulletMap.find(m_bullet_level);
        if (m_pConfig->bulletMap.cend() != itor) {
            BULLET_LIST::const_iterator list_itor = itor->second.cbegin();
            BULLET_LIST::const_iterator list_iend = itor->second.cend();
            while (list_iend != list_itor) {
                Ejector * ejector = Ejector::create(&(*list_itor), m_atk/itor->second.size(), getPosition(), BULLET_TYPE_PLAYER);
                plane->addChild(ejector);
                
                list_itor++;
            }
        } else {
            CCASSERT(false, "没有子弹的飞机,能战斗么?我次奥,我的三观,查配置文件去");
        }
    }
}
