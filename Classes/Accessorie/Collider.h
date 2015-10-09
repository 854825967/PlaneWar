#ifndef COLLIDER_H
#define COLLIDER_H

#include "MultiSys.h"
#include "Header.h"
#include "Struct.h"

class Collider : public cocos2d::Node {
public:
    //void setPosition(const cocos2d::Point & point);
    bool isCollider(const Collider * target);
    void addAction(const ACTION_CONFIG_LIST & actionGroup);
    virtual void setScale(float scale);
    virtual void setContentSize(const cocos2d::Size& contentSize);
protected:
    Collider();
    void init(const RadiusConfig & config);
private:
    void actionOver(cocos2d::Node * pSender);
    void updateRotate(float);

private:
    cocos2d::Rect m_rect;
    cocos2d::Point m_lastPoint;
    float m_radius;
    std::list<ACTION_CONFIG_LIST> m_actionGrpList;
    bool m_updateRotate;
};

#endif //ICOLLIDER_H
