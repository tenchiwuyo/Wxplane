
#include "PlaneLayer.h"
#include "BulletLayer.h"


BulletLayer::BulletLayer(void)
{
	bulletBatchNode=NULL;

	m_pAllBullet=CCArray::create();
	m_pAllBullet->retain();
}

BulletLayer::~BulletLayer(void)
{
	m_pAllBullet->release();
	m_pAllBullet=NULL;
}

bool BulletLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		CCTexture2D *textture = CCTextureCache::sharedTextureCache()->textureForKey("shoot.png");
		bulletBatchNode = CCSpriteBatchNode::createWithTexture(textture);
		this->addChild(bulletBatchNode);

		bRet = true;
	} while (0);
	return bRet;
}

void BulletLayer::StartShoot(float delay)
{
	this->schedule(schedule_selector(BulletLayer::AddBullet), 0.10f, kCCRepeatForever, delay);
}

void BulletLayer::StopShoot()
{
	this->unschedule(schedule_selector(BulletLayer::AddBullet));
}

void BulletLayer::AddBullet(float dt)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/bullet.mp3");
	CCSprite* bullet = CCSprite::createWithSpriteFrameName("bullet1.png");
	bulletBatchNode->addChild(bullet);

	this->m_pAllBullet->addObject(bullet);

	CCPoint planePosition=PlaneLayer::sharedPlane->getChildByTag(AIRPLANE)->getPosition();
	CCPoint bulletPosition=ccp(planePosition.x, planePosition.y+PlaneLayer::sharedPlane->getChildByTag(AIRPLANE)->getContentSize().height/2);
	bullet->setPosition(bulletPosition);

	float length=CCDirector::sharedDirector()->getWinSize().height+bullet->getContentSize().height/2-bulletPosition.y;
	float velocity=320/1;
	float realMoveDuration=length/velocity;

	CCFiniteTimeAction* actionMove=CCMoveTo::create(realMoveDuration, ccp(bulletPosition.x, CCDirector::sharedDirector()->getWinSize().height+bullet->getContentSize().height/2));
	CCFiniteTimeAction* actionDone=CCCallFuncN::create(this, callfuncN_selector(BulletLayer::bulletMoveFinished));

	CCSequence* sequence=CCSequence::create(actionMove, actionDone, NULL);
	bullet->runAction(sequence);
}

void BulletLayer::bulletMoveFinished(CCNode* pSender)
{
	CCSprite* bullet=(CCSprite*)pSender;
	this->bulletBatchNode->removeChild(bullet,true);
	this->m_pAllBullet->removeObject(bullet);
}

void BulletLayer::RemoveBullet(CCSprite* bullet)
{
	if (bullet!=NULL)
	{
		this->bulletBatchNode->removeChild(bullet,true);
		this->m_pAllBullet->removeObject(bullet);
	}
}