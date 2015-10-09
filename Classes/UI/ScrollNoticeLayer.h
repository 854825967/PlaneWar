#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ScrollNoticeLayer :public Sprite{
public:
	ScrollNoticeLayer(void);
	~ScrollNoticeLayer(void);

	CREATE_FUNC(ScrollNoticeLayer);
	virtual bool init();

	virtual void update(float delta);
	Label *  text2;
};

