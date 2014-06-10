
#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class MutiBulletLayer : public CCLayer
{
public:

	MutiBulletLayer(void);

	~MutiBulletLayer(void);

	CREATE_FUNC(MutiBulletLayer);

	virtual bool init();

	void AddMutiBullets(float dt);

	void mutiBulletsMoveFinished(CCNode* pSender);

	void RemoveMutiBullets(CCSprite* mutiBullets);

	void StartShoot();

	void StopShoot();

public:

	CCArray* m_pAllMutiBullets;

	CCSpriteBatchNode* mutiBulletsBatchNode;

};