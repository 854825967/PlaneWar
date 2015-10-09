#include "AppDelegate.h"
#include "Header.h"
#include "GameConfig.h"
#include "CHttpEngine.h"
#include "LoginScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

class RandHandler {
public:
    void SetSeed(u32 seed) {
        m_lSeed = seed;
    }
    
    s32 Rand() {
        return( ((m_lSeed = m_lSeed * 214013L
                  + 2531011L) >> 16) & 0x7fff);
    }
    
private:
    u64 m_lSeed;
};

AppDelegate::AppDelegate() {
//    RandHandler randHandler;
//    randHandler.SetSeed(100);
//    
//    while (true) {
//        printf("rand:%d\n", randHandler.Rand() % 1000);
//        
//        CSleep(2000);
//    }
}

AppDelegate::~AppDelegate() {

}

void AppDelegate::initialize() {
    auto director = Director::getInstance();
    Size winSize = director->getWinSize();
    g_pGameConfig = GameConfig::getInterface();

    g_pGameConfig->screenCenterPoint = Vec2(winSize.width/2.0f, winSize.height/2.0f);
    g_pGameConfig->scalex = winSize.width / BASE_SCREEN_WIDTH;
    g_pGameConfig->scaley = winSize.height / BASE_SCREEN_HEIGHT;
	//控件的缩放比例
	g_pGameConfig->scaleEleX = winSize.width/MAX_SCREEN_WIDTH;
	g_pGameConfig->scaleEleY = winSize.height/MAX_SCREEN_HEIGHT;
    if (g_pGameConfig->scaleEleX <=  g_pGameConfig->scaleEleY){
        g_pGameConfig->scaleEleMin = g_pGameConfig->scaleEleX;
        g_pGameConfig->scaleEleMax = g_pGameConfig->scaleEleY;
	}else{
        g_pGameConfig->scaleEleMin = g_pGameConfig->scaleEleY;
        g_pGameConfig->scaleEleMax = g_pGameConfig->scaleEleX;
	}

    g_pGameConfig->virtualSize = Vec2(BASE_SCREEN_WIDTH * g_pGameConfig->scalex, BASE_SCREEN_HEIGHT * g_pGameConfig->scaley);
    g_pGameConfig->screenSize = winSize;
    g_pHttpEngine->Start(1);
    
    
    g_log.OpenLogFile(0, "log", "debug");
    g_log.Start();
    g_tcpMgr.Start();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#ifdef WIN32
        glview = GLView::createWithRect("PlaneWars", Rect(0, 0, 640, 960));
#else
        glview = GLView::create("PlaneWars");
#endif //WIN32
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    initialize();

    // create a scene. it's an autorelease object
    auto scene = Login::createScene();
	//auto scene = MainMenu::createScene();
    //auto scene = DepotScene::createScene();
    /* auto scene = BattleGround::createScene(); */
	//auto scene = Login::createScene();

	
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call, it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
