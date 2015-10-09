#include "Plane.h"
#include "Struct.h"
#include "Header.h"
#include "Weapon.h"
#include "Wingman.h"
#include "Ejector.h"
#include "GameConfig.h"
#include "EquipManager.h"
#include "CEventEngine.h"

Plane * Plane::create(const SEquip & equip) {
    CCASSERT(equip.type == EQUIP_TYPE_PLANE, "用你麻痹的其它类型");
    const PlaneConfig * pConfig = g_pGameConfig->getPlaneConfig(equip.model);
    Plane * pPlane = NEW Plane(pConfig);
    CCASSERT(NULL !=  pPlane, "check ur memory");
    pPlane->m_atk = pConfig->atk + (pConfig->atk/20) * (equip.level - 1);
    return pPlane;
}

Plane::Plane(const PlaneConfig * pConfig) {
    m_pConfig = pConfig;
    init(m_pConfig->radius);
    string ejson = string("Animation/Plane/") + m_pConfig->res + ".ExportJson";
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    m_pPlane = Armature::create(m_pConfig->res);
    m_pPlane->setAnchorPoint(Vec2(.5f, .5f));
    m_pPlane->getAnimation()->play("fly");
    m_pPlane->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Plane::playActionOver));
    m_bullet_level = BULLET_LEVEL_1;
    this->addChild(m_pPlane);
    this->setContentSize(m_pPlane->getContentSize());

    BIND(BUND_ID_PLANE_FIRE, this, Plane::OnCmdFire);
    BIND(BUND_ID_PLANE_HOLD_FIRE, this, Plane::OnCmdHoldFire);
}

Plane::~Plane() {
    UNBIND(BUND_ID_PLANE_FIRE, this, Plane::OnCmdFire);
    UNBIND(BUND_ID_PLANE_HOLD_FIRE, this, Plane::OnCmdHoldFire);
}

void Plane::addWeapon(const SEquip &equip) {
    CCASSERT(equip.type == EQUIP_TYPE_WEAPON, "用你麻痹的其它类型");
    
    Weapon * weapon = Weapon::create(equip);
    weapon->setPosition(m_pConfig->weapon_pos);
    addChild(weapon);
}

void Plane::addWingman(const SEquip &equip) {
    CCASSERT(equip.type == EQUIP_TYPE_WINGMAN, "用你麻痹的其它类型");
    
    Wingman * wingman = Wingman::create(equip, WINGMAN_LEFT);
    wingman->setPosition(m_pConfig->wingman_pos);
    addChild(wingman);
    
    wingman = Wingman::create(equip, WINGMAN_RIGHT);
    wingman->setPosition(Vec2(-m_pConfig->wingman_pos.x, m_pConfig->wingman_pos.y));
    addChild(wingman);
}

void Plane::addArmor(const SEquip &equip) {
    const ArmorConfig * config = g_pGameConfig->getArmorConfig(equip.model);
    m_armor = config->armor + (config->armor/200) * (equip.level-1);
    ECHO("plane armor %d", m_armor);
}

void Plane::fire() {
    OnCmdFire(NULL, 0);
}

void Plane::OnCmdFire(void * context, const s32 size) {
    BULLET_MAP::const_iterator itor = m_pConfig->bulletMap.find(m_bullet_level);
    if (m_pConfig->bulletMap.cend() != itor) {
        BULLET_LIST::const_iterator list_itor = itor->second.cbegin();
        BULLET_LIST::const_iterator list_iend = itor->second.cend();
        while (list_iend != list_itor) {
            Ejector * ejector = Ejector::create(&(*list_itor), m_atk/itor->second.size(), Vec2(0, 0), BULLET_TYPE_PLAYER);
            addChild(ejector);
            list_itor++;
        }
    } else {
        CCASSERT(false, "没有子弹的飞机,能战斗么?我次奥,我的三观");
    }
    
    g_pEventEngine->BundlerCall(BUND_ID_WEAPON_FIRE, this, sizeof(this));
    g_pEventEngine->BundlerCall(BUND_ID_WINGMAN_FIRE, this, sizeof(this));
    g_pEventEngine->BundlerCall(BUND_ID_EJECTOR_FIRE, this, sizeof(this));
}

void Plane::OnCmdHoldFire(void *context, const s32 size) {
    holdFire();
}

void Plane::holdFire() {
    g_pEventEngine->BundlerCall(BUND_ID_WEAPON_HOLD_FIRE, this, sizeof(this));
    g_pEventEngine->BundlerCall(BUND_ID_WINGMAN_HOLD_FIRE, this, sizeof(this));
    g_pEventEngine->BundlerCall(BUND_ID_EJECTOR_HOLD_FIRE, this, sizeof(this));
}

void Plane::bulletUp() {
    holdFire();
    m_bullet_level++;
    if (m_bullet_level >= BULLET_LEVEL_COUNT) {
        m_bullet_level = BULLET_LEVEL_1;
        m_pPlane->getAnimation()->play("recover");
    }
    
    if (m_bullet_level == BULLET_LEVEL_GO) {
        m_pPlane->getAnimation()->play("transform");
    }
    fire();
}

void Plane::playActionOver(Armature * armature, MovementEventType type, const char * name) {
    if (armature == m_pPlane) {
        if (string(name) == "transform" && type == COMPLETE) {
            CCASSERT(type == COMPLETE, "transform is not a loop action, fuck u, small tank");
            m_pPlane->getAnimation()->play("rage");
        }
        
        if (string(name) == "recover" && type == COMPLETE) {
            CCASSERT(type == COMPLETE, "recover is not a loop action, fuck u, small tank");
            m_pPlane->getAnimation()->play("fly");
        }
    } else if (armature == m_pBoom) {
        removeFromParentAndCleanup(true);
    }
}

void Plane::beInjured(const s32 value) {
    m_armor -= value;
    if (m_armor <= 0) {
        disappear();
    } else {
        g_pEventEngine->BundlerCall(BUND_ID_PLANE_INJURED, this, sizeof(this));
    }
}

void Plane::disappear() {
    g_pEventEngine->BundlerCall(BUND_ID_PLANE_DISAPPEAR, this, sizeof(this));
    
    string ejson = string("Animation/Effect/") + m_pConfig->boom + ".ExportJson";
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    m_pBoom = Armature::create(m_pConfig->boom);
    m_pBoom->getAnimation()->play("boom");
    this->addChild(m_pBoom);
    m_pBoom->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Plane::playActionOver));
    
    unscheduleAllSelectors();
}

void Plane::showtime() {
    schedule(schedule_selector(Plane::showtime), 4.0f);
}

void Plane::showtime(float) {
    bulletUp();
    g_pEventEngine->BundlerCall(BUND_ID_BULLET_LEVEL_UP, this, sizeof(this));
}
