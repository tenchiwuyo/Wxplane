#include "WelcomeScene.h"

WelcomeScene::WelcomeScene(void)
{
	_welcomeLayer=NULL;
}

WelcomeScene::~WelcomeScene(void)
{
}

bool WelcomeScene::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCScene::init());
		_welcomeLayer=WelcomeLayer::create();
		CC_BREAK_IF(!_welcomeLayer);
		this->addChild(_welcomeLayer);
		PreloadMusic();
		bRet=true;
	} while (0);
}

void WelcomeScene::PreloadMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/game_music.mp3");
}