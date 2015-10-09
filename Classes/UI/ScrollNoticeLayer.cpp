#include "ScrollNoticeLayer.h"
ScrollNoticeLayer::ScrollNoticeLayer(void)
{
}


ScrollNoticeLayer::~ScrollNoticeLayer(void)
{
}

bool ScrollNoticeLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Sprite::init());

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		//水平滚动公告
		CCSprite *  textbase = Sprite::create("UI/Public/ScrollLayer/noticeBk.png");
		this->addChild(textbase);
		text2 = Label::create("A Ming defeats MonkeyMao in Challenge Mode!", "WRYH.ttf", 60);
		text2->setPosition(Vec2(visibleSize.width+text2->getContentSize().width/2, 50));
		text2->setColor(ccc3(255, 0, 0));
		textbase->addChild(text2);

		this->scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void ScrollNoticeLayer::update(float delta)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	int newX = text2->getPositionX()-7;
	if (newX<= -text2->getContentSize().width)
	{
		newX = visibleSize.width+text2->getContentSize().width/2;
	}
	text2->setPositionX(newX);
}