#include "Collider.h"
#include "Header.h"
#include "GameConfig.h"
USING_NS_CC;

Collider::Collider() {
    m_updateRotate = false;
    m_radius = .0f;
}

void Collider::init(const RadiusConfig & config) {
//    m_rect.setRect(config.tlPoint.x * g_pGameConfig->scaleEleMin,
//                   config.tlPoint.y * g_pGameConfig->scaleEleMin,
//                   (config.brPoint.x - config.tlPoint.x) * g_pGameConfig->scaleEleMin,
//                   (config.tlPoint.y - config.brPoint.y) * g_pGameConfig->scaleEleMin);
    
    //m_radius = config.tlPoint.distance(config.brPoint)/2.0f;
    
    //#ifdef COLLIDER_DEBUG
    Sprite * debug = Sprite::create("debug.png");
    addChild(debug, 10);

//#endif //def COLLIDER_DEBUG
}

void Collider::setContentSize(const cocos2d::Size &contentSize) {
    Node::setContentSize(contentSize);
    (contentSize.width < contentSize.height)?(m_radius = contentSize.width/2.0f):(m_radius = contentSize.height/2.0f);
    
    
//    for (s32 i=0; i<m_radius/getScale(); i+=5) {
//        Sprite * debug = Sprite::create("debug.png");
//        debug->setPosition(0, i);
//        addChild(debug, 10);
//    }
//    
//    for (s32 i=0; i<m_radius/getScale(); i+=5) {
//        Sprite * debug = Sprite::create("debug.png");
//        debug->setPosition(i, 0);
//        addChild(debug, 10);
//    }
}

void Collider::setScale(float scale) {
    CCASSERT(m_radius != .0f, "init first");
    m_radius = (m_radius/getScale()) * scale;
    Node::setScale(scale);
    
//    for (s32 i=0; i<m_radius; i+=5) {
//        Sprite * debug = Sprite::create("debug.png");
//        debug->setPosition(0, i);
//        addChild(debug, 10);
//    }
//    
//    for (s32 i=0; i<m_radius; i+=5) {
//        Sprite * debug = Sprite::create("debug.png");
//        debug->setPosition(i, 0);
//        addChild(debug, 10);
//    }

}

bool Collider::isCollider(const Collider * target) {
    CCASSERT(NULL !=  target, "check ur point, it's null, fuck u");
    return (getPosition().distance(target->getPosition()) <= (m_radius + target->m_radius)/2.0f);
}

void Collider::updateRotate(float) {
    Point point = getPosition();
    if (m_lastPoint !=  point) {
        float rotate = diyfun::getAngle(m_lastPoint, point);
        this->setRotation(rotate);
        m_lastPoint = point;
    }
}

void Collider::addAction(const ACTION_CONFIG_LIST & actionGroup) {
    if (m_actionGrpList.empty()) {
        ACTION_CONFIG_LIST::const_iterator citor = actionGroup.cbegin();
        ACTION_CONFIG_LIST::const_iterator ciend = actionGroup.cend();

        Vector<FiniteTimeAction * > actionGrp;
        while (citor !=  ciend) {
            ActionInterval * pAction = NULL;
            if (citor->relative) {
                pAction = g_pGameConfig->getActionByConfig(&( * citor), getPosition(), g_pGameConfig->scaleEleX, g_pGameConfig->scaleEleY);
            } else {
                pAction = g_pGameConfig->getActionByConfig(&( * citor), Vec2(0.0f, 0.0f), g_pGameConfig->scaleEleX, g_pGameConfig->scaleEleY);
            }

            if (citor->updaterotate && !m_updateRotate) {
                m_updateRotate = true;
                schedule(schedule_selector(Collider::updateRotate), 1/60.0f);
            } else if (!citor->updaterotate && m_updateRotate){
                m_updateRotate = false;
                unschedule(schedule_selector(Collider::updateRotate));
            }
            
            actionGrp.pushBack(pAction);
            citor++;
        }
        actionGrp.pushBack(CallFuncN::create(this, callfuncN_selector(Collider::actionOver)));
        this->runAction(Sequence::create(actionGrp));
    }

    m_actionGrpList.push_back(actionGroup);
}

void Collider::actionOver(Node * pSender) {
    m_actionGrpList.erase(m_actionGrpList.begin());
    if (m_actionGrpList.begin() !=  m_actionGrpList.end()) {
        Vector<FiniteTimeAction * > actionGrp;
        ACTION_CONFIG_LIST::const_iterator citor = m_actionGrpList.begin()->cbegin();
        ACTION_CONFIG_LIST::const_iterator ciend = m_actionGrpList.begin()->cend();

        while (citor !=  ciend) {
            ActionInterval * pAction = NULL;
            if (citor->relative) {
                pAction = g_pGameConfig->getActionByConfig(&( * citor), getPosition(), g_pGameConfig->scaleEleX, g_pGameConfig->scaleEleY);
            } else {
                pAction = g_pGameConfig->getActionByConfig(&( * citor), Vec2(0.0f, 0.0f), g_pGameConfig->scaleEleX, g_pGameConfig->scaleEleY);
            }
            
            if (citor->updaterotate && !m_updateRotate) {
                m_updateRotate = true;
                schedule(schedule_selector(Collider::updateRotate), 1/60.0f);
            } else if (!citor->updaterotate && m_updateRotate){
                m_updateRotate = false;
                unschedule(schedule_selector(Collider::updateRotate));
            }

            actionGrp.pushBack(pAction);
            citor++;
        }
        actionGrp.pushBack(CallFuncN::create(this, callfuncN_selector(Collider::actionOver)));
        this->runAction(Sequence::create(actionGrp));
    }
}