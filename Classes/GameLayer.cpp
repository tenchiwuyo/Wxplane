//GameLayer.cpp
#include "GameLayer.h"

Level GameLayer::level=EASY;

GameLayer::GameLayer(void)
{
	background1=NULL;
	background2=NULL;
	planeLayer=NULL;
	bulletLayer=NULL;
	mutiBulletsLayer=NULL;
	
	enemyLayer=NULL;

	score=0;
	bigBoomCount=0;
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
		//png加入全局cache中
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shoot_background.plist");

		//加载background1, background1和background2是CCSprite*型成员变量
		background1=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background1->setAnchorPoint(ccp(0,0));
		background1->setPosition(ccp(0,0));
		this->addChild(background1);

		//加载background2
		background2=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background2->setAnchorPoint(ccp(0,0));
		background2->setPosition(ccp(0,background2->getContentSize().height-2));
		this->addChild(background2);

		this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

		//加入planeLayer
		this->planeLayer=PlaneLayer::create();
		this->addChild(planeLayer);

		//加入bulletLayer
		this->bulletLayer=BulletLayer::create();
		this->addChild(bulletLayer);
		this->bulletLayer->StartShoot();

		//加入enemyLayer
		this->enemyLayer=EnemyLayer::create();
		this->addChild(enemyLayer);

		this->setTouchEnabled(true);

		bRet=true;

	} while (0);
	return bRet;
}

//背景滚动
void GameLayer::backgroundMove(float dt)
{
	background1->setPositionY(background1->getPositionY()-2);
	background2->setPositionY(background1->getPositionY()+background1->getContentSize().height-2);
	if (background2->getPositionY()==0)
	{
		background1->setPositionY(0);
	}
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (this->planeLayer->isAlive)
	{
		CCPoint beginPoint=pTouch->getLocationInView();
		beginPoint=CCDirector::sharedDirector()->convertToGL(beginPoint);

		//juggle the area of drag
		CCRect planeRect=this->planeLayer->getChildByTag(AIRPLANE)->boundingBox();
		planeRect.origin.x-=15;
		planeRect.origin.y-=15;
		planeRect.size.width+=30;
		planeRect.size.height+=30;
		if (planeRect.containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch)))
		{
			CCPoint endPoint=pTouch->getPreviousLocationInView();
			endPoint=CCDirector::sharedDirector()->convertToGL(endPoint);

			CCPoint offSet=ccpSub(beginPoint, endPoint);
			CCPoint toPoint=ccpAdd(this->planeLayer->getChildByTag(AIRPLANE)->getPosition(), offSet);
			this->planeLayer->MoveTo(toPoint);
		}
	}
}

void GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameLayer::registerWithTouchDispatcher()
{
	CCDirector *pDirector=CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameLayer::update(float dt)
{
	if (level==EASY && score>=1000000)
	{
		level=MIDDLE;
	}
	else if (level==MIDDLE && score>=2000000)
	{
		level=HARD;
	}

	CCArray* bulletsToDelete=CCArray::create();
	bulletsToDelete->retain();
	CCObject* bt, *et, *ut;

	//enemy1 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet, bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy1sToDelete=CCArray::create();
		enemy1sToDelete->retain();
		int a=this->enemyLayer->m_pAllEnemy1->capacity();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
		{
			Enemy* enemy1=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife()==1)
				{
					enemy1->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy1sToDelete->addObject(enemy1);
					score+=ENEMY1_SCORE;
					//this->controlLayer.updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete,et)
		{
			Enemy* enemy1=(Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy2 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy2sToDelete=CCArray::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife()>1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy2sToDelete->addObject(enemy2);
					score+=ENEMY2_SCORE;
					//this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy3 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy3sToDelete=CCArray::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				//如果life>1,移除bullet
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				//如果life==1,移除enemy3
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy3sToDelete->addObject(enemy3);
					score+=ENEMY3_SCORE;
					//this->controlLayer->updateScore(score);
				}
				//此时处在animate阶段,不做处理
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();
	bulletsToDelete->release();

	CCArray* mutiBulletsToDelete=CCArray::create();
	mutiBulletsToDelete->retain();
	CCObject* mbt;

	////enemy1 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy1sToDelete=CCArray::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
		{
			Enemy* enemy1=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife()==1)
				{
					enemy1->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy1sToDelete->addObject(enemy1);
					score+=ENEMY1_SCORE;
					//this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete,et)
		{
			Enemy* enemy1=(Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	//enemy2 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy2sToDelete=CCArray::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife()>1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy2sToDelete->addObject(enemy2);
					score+=ENEMY2_SCORE;
					//this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	//enemy3 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;

		CCArray* enemy3sToDelete=CCArray::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				//如果life>1,移除bullet
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				//如果life==1,移除enemy3
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy3sToDelete->addObject(enemy3);
					score+=ENEMY3_SCORE;
					//this->controlLayer->updateScore(score);
				}
				//此时处在animate阶段,不做处理
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete,mbt)
	{
		CCSprite* mutiBullets=(CCSprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();
	mutiBulletsToDelete->release();

	CCRect airplaneRect=this->planeLayer->getChildByTag(AIRPLANE)->boundingBox();
	airplaneRect.origin.x+=30;
	airplaneRect.size.width-=60;
	//enemy1 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et)
	{
		Enemy* enemy1=(Enemy*)et;
		if (enemy1->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				//卸载所有任务计划
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	//enemy2 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
	{
		Enemy* enemy2=(Enemy*)et;
		if (enemy2->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy2->getBoundingBox()))
			{
				//卸载所有任务计划
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	//enemy3 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
	{
		Enemy* enemy3=(Enemy*)et;
		if (enemy3->getLife()>0)
		{
			if (airplaneRect.intersectsRect(enemy3->getBoundingBox()))
			{
				//卸载所有任务计划
				this->unscheduleAllSelectors();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->planeLayer->Blowup(score);
				return;
			}
		}
	}

	//mutiBullets & airplane CheckCollision
	CCARRAY_FOREACH(this->ufoLayer->m_pAllMutiBullets,ut)
	{
		CCSprite* mutiBullets=(CCSprite*)ut;
		if (this->planeLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(mutiBullets->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("get_double_laser.mp3");
			this->ufoLayer->RemoveMutiBullets(mutiBullets);
			this->bulletLayer->StopShoot();
			this->mutiBulletsLayer->StartShoot();
			this->bulletLayer->StartShoot(6.2f);
		}
	}

	//bigBoom & airplane CheckCollision
	CCARRAY_FOREACH(this->ufoLayer->m_pAllBigBoom,ut)
	{
		CCSprite* bigBoom=(CCSprite*)ut;
		if (this->planeLayer->getChildByTag(AIRPLANE)->boundingBox().intersectsRect(bigBoom->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("get_bomb.mp3");
			this->ufoLayer->RemoveBigBoom(bigBoom);
			bigBoomCount++;
			//updateBigBoomItem(bigBoomCount);
		}
	}
}

Level GameLayer::getCurLevel()
{
	return level;
}