#include "Wingman.h"
#include "Plane.h"
#include "Ejector.h"
#include "CEventEngine.h"
#include "GameConfig.h"

Wingman * Wingman::create(const SEquip & equip, const s8 wingmanType) {
    CCASSERT(equip.type == EQUIP_TYPE_WINGMAN, "检查类型");
    const WingmanConfig * pConfig = g_pGameConfig->getWingmanConfig(equip.model);
    Wingman * ret = NEW Wingman(pConfig, wingmanType);
    ret->m_atk = pConfig->atk + (pConfig->atk/20) * (equip.level - 1);
    ret->autorelease();
    return ret;
}

Wingman::~Wingman() {
    UNBIND(BUND_ID_WINGMAN_FIRE, this, Wingman::OnCmdFire);
    UNBIND(BUND_ID_BULLET_LEVEL_UP, this, Wingman::OnCmdBulletUp);
}

Wingman::Wingman(const WingmanConfig * pConfig, const s8 wingmanType) {
    m_pConfig = pConfig;
    string ejson = string("Animation/Wingman/") + m_pConfig->res + ".ExportJson";
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    m_pWingman = Armature::create(m_pConfig->res);
    
    m_type = wingmanType;
    if (WINGMAN_RIGHT == m_type) {
        OrbitCamera * od = OrbitCamera::create(0.0f, 0.5f, 1.0f, 0.0f, 180.0f, 0.0f, 0.0f);
        runAction(od);
    }
    
    m_bullet_level = BULLET_LEVEL_1;
    this->addChild(m_pWingman);
    m_pWingman->getAnimation()->play("fly");
    
    BIND(BUND_ID_WINGMAN_FIRE, this, Wingman::OnCmdFire);
    BIND(BUND_ID_BULLET_LEVEL_UP, this, Wingman::OnCmdBulletUp);
    
    m_pWingman->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Wingman::playActionOver));
}

void Wingman::playActionOver(Armature * armature, MovementEventType type, const char * name) {
    if (string(name) == "transform" && type != START) {
        CCASSERT(type == COMPLETE, "transform is not a loop action, fuck u, small tank");
        m_pWingman->getAnimation()->play("rage");
    }
    
    if (string(name) == "recover" && type != START) {
        CCASSERT(type == COMPLETE, "recover is not a loop action, fuck u, small tank");
        m_pWingman->getAnimation()->play("fly");
    }
}

void Wingman::OnCmdFire(void * context, const s32 size) {
    if (getParent() == context) {
        Plane * plane = (Plane *)context;
        BULLET_MAP::const_iterator itor = m_pConfig->bulletMap[m_type].find(m_bullet_level);
        if (m_pConfig->bulletMap[m_type].cend() != itor) {
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

void Wingman::OnCmdBulletUp(void * context, const s32 size) {
    if (getParent() == context) {
        m_bullet_level++;
        if (m_bullet_level >= BULLET_LEVEL_COUNT) {
            m_bullet_level = BULLET_LEVEL_1;
            m_pWingman->getAnimation()->play("recover");
        }
        
        if (m_bullet_level == BULLET_LEVEL_GO) {
            m_pWingman->getAnimation()->play("transform");
        }
    }
}
