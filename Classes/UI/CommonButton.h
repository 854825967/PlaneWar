#ifndef COMMON_BUTTON_H
#define COMMON_BUTTON_H

#include "Header.h"
#include "Interface/ITouchHandler.h"
#include "CTouchEngine.h"
#include "TStream.h"
USING_NS_CC;
using namespace tlib;

static std::string g_defaultBtnStringTnf = "";//按钮上文字对应的tnf
static int g_defaultBtnFontType = 1;//1:CCLabelTTF, 2:CCLabelBMFont

class CommonButton : public ITouchHandler, public Node{
public:
	~CommonButton(void);

	static CommonButton * create(const s32 nEventID);
	virtual void setOpacity(GLubyte opacity);
	virtual void setScale(float scale);
	virtual void changePic(const char * fileName);
	virtual s32 GetTouchIndex();
	virtual bool IsTouch(const cocos2d::Point & point);
	virtual void TouchEvent(const cocos2d::Point & point, const CTouchEventType nEventType);

	static CommonButton *  createWithNode(Node *  noramlNode, Node *  selectedNode,
                                        Node *  disabledNode, const char *  btnStr = "",
                                        const char *  btnBottomStr = "");
    
	static CommonButton *  createWithNode(Node *  noramlNode, Node *  selectedNode,
                                        Node *  disabledNode, int nEventID,
                                        const char *  btnStr = "", const char *  btnBottomStr = "");

	static CommonButton *  create(const char  * normalImage, const char  * selectedImage,
                                int nEventID);
    
	static CommonButton *  create(const char  * normalImage, const char  * selectedImage,
                                const char  * disabledImage, int nEventID);

	static CommonButton *  createWithSpriteFrame(cocos2d::CCSpriteFrame  * normalSpriteFrame,
                                               cocos2d::CCSpriteFrame  * selectedSpriteFrame, int nEventID);
    
	static CommonButton *  createWithSpriteFrameName(const char  * normalName, const char  * selectedName, int nEventID);
    
	static CommonButton *  createWithSpriteFrameName(const char  * normalName, const char  * selectedName,
                                                   const char  * disabledName, int nEventID);

	bool initWithNode(Node *  normalNode, Node *  selectedNode, Node *  disabledNode,
                      int nEventID, const char *  btnStr = "", const char *  btnBottomStr = "");
    
	void setString(const char *  str);//设置按钮上的字
	void setBottomString(const char *  bottomStr);//设置按钮底部文字标签
	void setFontColor(const cocos2d::ccColor3B& color);//设置字体颜色
	void setFontSize(float fontSize);//设置字体
	void setFontName(const char  * fontName);//设置字体大小

	static void setDefaultBtnStringTnf(std::string var);//设置字体文件（当选用CCLabelBMFont时有用）
	static void setDefaultBtnFontType(int var);//设置字体类型

	//响应
	virtual void selected(bool var);
	virtual bool isEnabled();
	virtual void setEnabled(bool value);
	virtual bool isSelected();
    
	//getter & setter
    void setNormalImage(Node *  pImage);
    void setSelectedImage(Node *  pImage);
    void setDisabledImage(Node *  pImage);
    
	Node *  getNormalImage();
	Node *  getSelectedImage();
	Node *  getDisabledImage();


private:
    CommonButton(const s32 nEventID);
private:
	cocos2d::Sprite * m_pSprite;
	cocos2d::Label * m_pLabel;
	s32 m_nEventID;
	float m_fScale;

	Node *  m_pNormalImage;
	Node *  m_pSelectedImage;
	Node *  m_pDisabledImage;
	Node *  m_label;
	Node *  m_pBottomlabel;
    
protected:
	virtual void updateImagesVisibility();
protected:
	bool m_bSelected;
	bool m_bEnabled;
};

#endif //BUTTON_H
