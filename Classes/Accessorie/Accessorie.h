#ifndef ACCESSORIE_H
#define ACCESSORIE_H

#include "Header.h"
#include "Struct.h"

class Accessorie {
public:
    static cocos2d::Node * getAccessorie(const s32 type, const char * name);
};

#endif //ACCESSORIE_H
