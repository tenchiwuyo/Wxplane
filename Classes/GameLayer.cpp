//GameLayer.cpp
#include "GameLayer.h"

GameLayer::GameLayer(void)
{
	background1=NULL;
	background2=NULL;
	planeLayer=NULL;
}

GameLayer::~GameLayer(void)
{
}

bool GameLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		//png����ȫ��cache��
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shoot_background.plist");

		//����background1, background1��background2��CCSprite*�ͳ�Ա����
		background1=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background1->setAnchorPoint(ccp(0,0));
		background1->setPosition(ccp(0,0));
		this->addChild(background1);

		//����background2
		background2=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background2->setAnchorPoint(ccp(0,0));
		background2->setPosition(ccp(0,background2->getContentSize().height-2));
		this->addChild(background2);

		this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

		this->planeLayer=PlaneLayer::create();
		this->addChild(planeLayer);

		bRet=true;

	} while (0);
	return bRet;
}

//��������
void GameLayer::backgroundMove(float dt)
{
	background1->setPositionY(background1->getPositionY()-2);
	background2->setPositionY(background1->getPositionY()+background1->getContentSize().height-2);
	if (background2->getPositionY()==0)
	{
		background1->setPositionY(0);
	}
}