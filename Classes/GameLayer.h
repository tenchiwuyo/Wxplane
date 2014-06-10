//GameLayer.h
#pragma once

#include "cocos2d.h"
#include "PlaneLayer.h"
#include "BulletLayer.h"
#include "MutiBulletLayer.h"
#include "UFOLayer.h"
#include "EnemyLayer.h"

USING_NS_CC;

const int ENEMY1_SCORE=1000;
const int ENEMY2_SCORE=6000;
const int ENEMY3_SCORE=30000;

const int MAX_BIGBOOM_COUNT=100000;
const int TAG_BIGBOOM_MENUITEM=1;
const int TAG_BIGBOOMCOUNT_LABEL=2;

class GameLayer: public CCLayer
{
public:
	GameLayer(void);

	~GameLayer(void);

	CREATE_FUNC(GameLayer);

	virtual bool init();

	void backgroundMove(float ft);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void registerWithTouchDispatcher();

	void update(float dt);

	static Level getCurLevel();

private:
	CCSprite* background1;
	CCSprite* background2;
	PlaneLayer* planeLayer;
	BulletLayer* bulletLayer;
	MutiBulletLayer* mutiBulletsLayer;
	UFOLayer* ufoLayer;
	EnemyLayer* enemyLayer;

	int score;
	int bigBoomCount;
	static Level level;
};