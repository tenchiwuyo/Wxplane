
#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class UFOLayer : public CCLayer
{
public:

	UFOLayer(void);

	~UFOLayer(void);

	CREATE_FUNC(UFOLayer);

	virtual bool init();

	void AddMutiBullets(float dt);

	void mutiBulletsMoveFinished(CCNode* pSender);

	void AddBigBoom(float dt);

	void bigBoomMoveFinished(CCNode* pSender);

	void RemoveMutiBullets(CCSprite* mutiBullets);

	void RemoveBigBoom(CCSprite* bigBoom);

public:

	CCArray* m_pAllMutiBullets;

	CCArray* m_pAllBigBoom;

};