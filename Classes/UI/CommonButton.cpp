#include "CommonButton.h"
#include "Data/GameConfig.h"
USING_NS_CC;

CommonButton * CommonButton::create(const s32 nEventID) {
	CommonButton * ret = NEW CommonButton(nEventID);
	if (ret) {
		ret->autorelease();
	} else {
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void CommonButton::changePic(const char * fileName) {
	m_pSprite->setTexture(fileName);
}

void CommonButton::setScale(float scale) {
	Node::setScale(scale);
	m_fScale = scale;
}

void CommonButton::setOpacity(GLubyte opacity) {
	Vector<Node  * > children_list = getChildren();
	Vector<Node  * >::iterator itor = children_list.begin();
	Vector<Node  * >::iterator iend = children_list.end();

	while (itor !=  iend) {
		( * itor)->setOpacity(opacity);
		itor++;
	}
	Node::setOpacity(opacity);
}

s32 CommonButton::GetTouchIndex() {
	return 0;
}

bool CommonButton::IsTouch(const cocos2d::Point & point){
	float scaleEle = g_pGameConfig->scaleEle;
	float scaleEleX = g_pGameConfig->scaleEleX;
    
	Point locationInNode = this->convertToNodeSpace(point) * scaleEleX;
	Size s = this->getContentSize() * scaleEleX;
	Rect rect = Rect(130 * scaleEleX, -m_pBottomlabel->getContentSize().height * scaleEleX,
                     s.width - 130 * scaleEleX, s.height+m_pBottomlabel->getContentSize().height * scaleEleX);
	if (rect.containsPoint(locationInNode)){
		return true;
	}
	return false;
}

void CommonButton::TouchEvent(const cocos2d::Point & point, const CTouchEventType nEventType){
    switch(nEventType) {
    case TOUCH_EVENT_BEGAN:
        {
            selected(true);
            break;
        }
    case TOUCH_EVENT_MOVED:
        {

            break;
        }
    case TOUCH_EVENT_ENDED:
        {
            selected(false);
            break;
        }
    case TOUCH_EVENT_CANCELED:
        {
            selected(false);
            break;
        }
    }
}

CommonButton::CommonButton(const s32 nEventID) {
    m_pNormalImage = NULL;
    m_pSelectedImage = NULL;
    m_pDisabledImage = NULL;
    m_nEventID = nEventID;
    m_fScale = 1;
    setAnchorPoint(ccp(.5f, .5f));
}

CommonButton::~CommonButton(void) {

}

CommonButton *  CommonButton::createWithNode(Node *  noramlNode, Node *  selectedNode,
                                           Node *  disabledNode, int nEventID,
                                           const char *  btnStr, const char *  btnBottomStr) {
	CommonButton  * pRet = NEW CommonButton(nEventID);
	if (pRet && pRet->initWithNode(noramlNode, selectedNode, disabledNode, nEventID, btnStr, btnBottomStr)) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

CommonButton *  CommonButton::createWithNode(Node *  noramlNode, Node *  selectedNode,
                                           Node *  disabledNode, const char *  btnStr, const char *  btnBottomStr) {
	return CommonButton::createWithNode(noramlNode, selectedNode, disabledNode, NULL, btnStr, btnBottomStr);
}

CommonButton * CommonButton::create(const char  * normalImage, const char  * selectedImage, int nEventID) {
	return CommonButton::create(normalImage, selectedImage, NULL, nEventID);
}

CommonButton * CommonButton::create(const char  * normalImage, const char  * selectedImage,
                                    const char  * disabledImage, int nEventID) {
	CommonButton * pRet = NEW CommonButton(nEventID);
	Node * normalSprite = NULL;
	Node * selectedSprite = NULL;
	Node * disabledSprite = NULL;
    
	if (normalImage) {
		normalSprite = CCSprite::create(normalImage);
	}
    
	if (selectedImage) {
		selectedSprite = CCSprite::create(selectedImage);
	}
    
	if(disabledImage) {
		disabledSprite = CCSprite::create(disabledImage);
	}
    
	if (pRet && pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, nEventID)) {
		pRet->autorelease();
		return pRet;
	}
    
	CC_SAFE_DELETE(pRet);
	return NULL;
}

CommonButton *  CommonButton::createWithSpriteFrame(CCSpriteFrame  * normalSpriteFrame,
                                                  CCSpriteFrame  * selectedSpriteFrame, int nEventID) {
	CommonButton  * pRet = NEW CommonButton(nEventID);
    
	if (pRet) {
		CCSprite *  normalSprite = CCSprite::createWithSpriteFrame(normalSpriteFrame);
		CCSprite *  selectedSprite = CCSprite::createWithSpriteFrame(selectedSpriteFrame);
		CCSprite *  disabledSprite = NULL;
        
		if (pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, nEventID)) {
			pRet->autorelease();
			return pRet;
		}
	}
    
	CC_SAFE_DELETE(pRet);
	return NULL;
}

CommonButton *  CommonButton::createWithSpriteFrameName(const char  * normalName, const char  * selectedName, int nEventID) {
	CommonButton  * pRet = NEW CommonButton(nEventID);
    
	if (pRet) {
		CCSprite *  normalSprite = CCSprite::createWithSpriteFrameName(normalName);
		CCSprite *  selectedSprite = CCSprite::createWithSpriteFrameName(selectedName);
		CCSprite *  disabledSprite = NULL;
        
		if (pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, nEventID)) {
			pRet->autorelease();
			return pRet;
		}
	}
    
	CC_SAFE_DELETE(pRet);
	return NULL;
}
CommonButton *  CommonButton::createWithSpriteFrameName(const char  * normalName, const char  * selectedName,
                                                      const char  * disabledName, int nEventID) {
	CommonButton  * pRet = NEW CommonButton(nEventID);
    
	if (pRet) {
		CCSprite *  normalSprite = CCSprite::createWithSpriteFrameName(normalName);
		CCSprite *  selectedSprite = CCSprite::createWithSpriteFrameName(selectedName);
		CCSprite *  disabledSprite = CCSprite::createWithSpriteFrameName(disabledName);;
		if (pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, nEventID)) {
			pRet->autorelease();
			return pRet;
		}
	}
    
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CommonButton::initWithNode(Node *  normalNode, Node *  selectedNode,
                                Node *  disabledNode, int nEventID,
                                const char *  btnStr, const char *  btnBottomStr){
	m_bEnabled = true;
	m_bSelected = false;
	setNormalImage(normalNode);
	setSelectedImage(selectedNode);
	setDisabledImage(disabledNode);

	float scaleEle = GameConfig::getInterface()->scaleEle;
	float scaleEleX = GameConfig::getInterface()->scaleEleX;

	if (g_defaultBtnFontType == 2){
		m_label = CCLabelBMFont::create(btnStr, g_defaultBtnStringTnf.c_str());
		m_pBottomlabel = CCLabelBMFont::create(btnBottomStr, g_defaultBtnStringTnf.c_str());
	}else{
		m_label = CCLabelTTF::create(btnStr, "黑体", 40);
		m_pBottomlabel = CCLabelTTF::create(btnBottomStr, "黑体", 40);
	}
    
	m_label->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
	m_pBottomlabel->setPosition(ccp((getContentSize().width/2) + 75, -20));
	setFontColor(ccWHITE);
	addChild(m_label, 100);
	addChild(m_pBottomlabel, 101);

	return true;
}
void CommonButton::setString(const char *  str){
	if (m_label) {
		CCLabelBMFont *  bmFont = dynamic_cast<CCLabelBMFont * >(m_label);
		if (bmFont) {
			bmFont->setString(str);
		} else {
			CCLabelTTF *  ttf = dynamic_cast<CCLabelTTF * >(m_label);
			if (ttf) {
				ttf->setString(str);
			}
		}
        
		if (0 == strcmp(str, "")) {
			m_label->setVisible(false);
		} else {
			m_label->setVisible(true);
		}
	}
}

void CommonButton::setBottomString(const char *  bottomStr){
	if (m_label){
		CCLabelBMFont *  bmFont = dynamic_cast<CCLabelBMFont * >(m_pBottomlabel);
		if (bmFont){
			bmFont->setString(bottomStr);
		} else {
			CCLabelTTF *  ttf = dynamic_cast<CCLabelTTF * >(m_pBottomlabel);
			if (ttf) {
				ttf->setString(bottomStr);
			}
		}
		if (0 == strcmp(bottomStr, "")){
			m_pBottomlabel->setVisible(false);
		} else {
			m_pBottomlabel->setVisible(true);
		}
	}
}

void CommonButton::setFontColor(const ccColor3B& color) {
	CCLabelBMFont *  bmFont = dynamic_cast<CCLabelBMFont * >(m_label);
	if (bmFont) {
		bmFont->setColor(color);
	} else {
		CCLabelTTF *  ttf = dynamic_cast<CCLabelTTF * >(m_label);
		if (ttf) {
			ttf->setColor(color);
		}
	}
}

void CommonButton::setFontSize(float fontSize) {
	CCLabelBMFont *  bmFont = dynamic_cast<CCLabelBMFont * >(m_label);
	CCLabelBMFont *  bmFont1 = dynamic_cast<CCLabelBMFont * >(m_pBottomlabel);
    
	if (bmFont) {
		float scale = fontSize/24;
		bmFont->setScale(scale);
	} else {
		CCLabelTTF *  ttf = dynamic_cast<CCLabelTTF * >(m_label);
		if (ttf) {
			ttf->setFontSize(fontSize);
		}
	}
    
	if (bmFont1) {
		float scale = fontSize/24;
		bmFont1->setScale(scale);
	} else {
		CCLabelTTF *  ttf = dynamic_cast<CCLabelTTF * >(m_pBottomlabel);
		if (ttf) {
			ttf->setFontSize(fontSize);
		}
	}
}

void CommonButton::setFontName(const char *  fontName) {
	CCLabelTTF *  ttf = dynamic_cast<CCLabelTTF * >(m_label);
	CCLabelTTF *  ttf1 = dynamic_cast<CCLabelTTF * >(m_label);
	if (ttf) {
		ttf->setFontName(fontName);
	}
    
    if (ttf1) {
		ttf1->setFontName(fontName);
	}
}

Node * CommonButton::getNormalImage() {
	return m_pNormalImage;
}

void CommonButton::setNormalImage(Node *  pImage) {
	if (pImage !=  m_pNormalImage) {
		if (pImage) {
			addChild(pImage);
			pImage->setAnchorPoint(ccp(0, 0));
		}

		if (m_pNormalImage) {
			removeChild(m_pNormalImage, true);
		}

		m_pNormalImage = pImage;
		this->setContentSize(m_pNormalImage->getContentSize());
		this->updateImagesVisibility();
	}
}

Node * CommonButton::getSelectedImage() {
	return m_pSelectedImage;
}

void CommonButton::setSelectedImage(Node *  pImage) {
	if (pImage !=  m_pNormalImage) {
		if (pImage) {
			addChild(pImage);
			pImage->setAnchorPoint(ccp(0, 0));
		}

		if (m_pSelectedImage) {
			removeChild(m_pSelectedImage, true);
		}

		m_pSelectedImage = pImage;
		this->updateImagesVisibility();
	}
}

Node * CommonButton::getDisabledImage() {
	return m_pDisabledImage;
}

void CommonButton::setDisabledImage(Node *  pImage) {
	if (pImage !=  m_pNormalImage) {
		if (pImage) {
			addChild(pImage);
			pImage->setAnchorPoint(ccp(0, 0));
		}

		if (m_pDisabledImage) {
			removeChild(m_pDisabledImage, true);
		}

		m_pDisabledImage = pImage;
		this->updateImagesVisibility();
	}
}


void CommonButton::setDefaultBtnFontType(int var) {
	g_defaultBtnFontType = var;
}

void CommonButton::updateImagesVisibility() {
	if (m_bEnabled) {
		if (m_bSelected) {
			if (m_pNormalImage)   m_pNormalImage->setVisible(false);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(true);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		} else {
			if (m_pNormalImage)   m_pNormalImage->setVisible(true);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		}
	} else {
		if (m_pDisabledImage) {
			if (m_pNormalImage)   m_pNormalImage->setVisible(false);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
		} else {
			if (m_pNormalImage)   m_pNormalImage->setVisible(true);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		}
	}
}

void CommonButton::setEnabled(bool enabled) {
	if (m_bEnabled!= enabled) {
		m_bEnabled = enabled;
		updateImagesVisibility();
	}
}

bool CommonButton::isEnabled() {
	return m_bEnabled;
}

bool CommonButton::isSelected() {
	return m_bSelected;
}

void CommonButton::selected(bool var) {
	m_bSelected = var;
	updateImagesVisibility();
}
