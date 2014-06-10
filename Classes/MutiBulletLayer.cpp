
#include "MutiBulletLayer.h"
#include "PlaneLayer.h"

MutiBulletLayer::MutiBulletLayer(void)
{
	mutiBulletsBatchNode=NULL;

	m_pAllMutiBullets=CCArray::create();
	m_pAllMutiBullets->retain();
}

MutiBulletLayer::~MutiBulletLayer(void)
{
	m_pAllMutiBullets->release();
	m_pAllMutiBullets=NULL;
}

bool MutiBulletLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		CCTexture2D *texture=CCTextureCache::sharedTextureCache()->textureForKey("shoot.png");
		mutiBulletsBatchNode=CCSpriteBatchNode::createWithTexture(texture);
		this->addChild(mutiBulletsBatchNode);

		bRet=true;
	} while (0);
	return bRet;
}

void MutiBulletLayer::StartShoot()
{
	this->schedule(schedule_selector(MutiBulletLayer::AddMutiBullets), 0.2f, 30, 0.0f);
}

void MutiBulletLayer::StopShoot()
{
	this->unschedule(schedule_selector(MutiBulletLayer::AddMutiBullets));
}

void MutiBulletLayer::AddMutiBullets(float dt)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bullet.mp3");

	CCSprite* bulletLeft=CCSprite::createWithSpriteFrameName("bullet2.png");
	CCSprite* bulletRight=CCSprite::createWithSpriteFrameName("bullet2.png");
	mutiBulletsBatchNode->addChild(bulletLeft);
	mutiBulletsBatchNode->addChild(bulletRight);
	this->m_pAllMutiBullets->addObject(bulletLeft);
	this->m_pAllMutiBullets->addObject(bulletRight);

	CCPoint planePosition=PlaneLayer::sharedPlane->getChildByTag(AIRPLANE)->getPosition();
	CCPoint bulletLeftPosition=ccp(planePosition.x-33, planePosition.y+35);
	CCPoint bulletRightPosition=ccp(planePosition.x+33, planePosition.y+35);
	bulletLeft->setPosition(bulletLeftPosition);
	bulletRight->setPosition(bulletRightPosition);

	float length=CCDirector::sharedDirector()->getWinSize().height+bulletLeft->getContentSize().height/2-bulletLeftPosition.y;
	float velocity=420/1;
	float realMoveDuration=length/velocity;

	CCFiniteTimeAction* actionLeftMove=CCMoveTo::create(realMoveDuration, ccp(bulletLeftPosition.x, CCDirector::sharedDirector()->getWinSize().height+bulletLeft->getContentSize().height/2));
	CCFiniteTimeAction* actionLeftDone=CCCallFuncN::create(this, callfuncN_selector(MutiBulletLayer::mutiBulletsMoveFinished));
	CCSequence* sequenceLeft=CCSequence::create(actionLeftMove, actionLeftDone, NULL);

	CCFiniteTimeAction* actionRightMove=CCMoveTo::create(realMoveDuration,ccp(bulletRightPosition.x,CCDirector::sharedDirector()->getWinSize().height+bulletRight->getContentSize().height/2));
	CCFiniteTimeAction* actionRightDone=CCCallFuncN::create(this,callfuncN_selector(MutiBulletLayer::mutiBulletsMoveFinished));
	CCSequence* sequenceRight=CCSequence::create(actionRightMove,actionRightDone,NULL);

	bulletLeft->runAction(sequenceLeft);
	bulletRight->runAction(sequenceRight);
}

void MutiBulletLayer::mutiBulletsMoveFinished(CCNode* pSender)
{
	CCSprite* mutiBullets=(CCSprite*)pSender;
	m_pAllMutiBullets->removeObject(mutiBullets);
	this->mutiBulletsBatchNode->removeChild(mutiBullets,true);
	CCLog("MutiBUlletsCount=%d",m_pAllMutiBullets->count());
}

void MutiBulletLayer::RemoveMutiBullets(CCSprite* mutiBullets)
{
	if (mutiBullets!=NULL)
	{
		this->m_pAllMutiBullets->removeObject(mutiBullets);
		this->mutiBulletsBatchNode->removeChild(mutiBullets,true);
	}
}