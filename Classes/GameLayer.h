//GameLayer.h
#include "cocos2d.h"

USING_NS_CC;

class GameLayer: public CCLayer
{
public:
	GameLayer(void);

	~GameLayer(void);

	CREATE_FUNC(GameLayer);

	virtual bool init();

	void backgroundMove(float ft);

private:
	CCSprite* background1;
	CCSprite* background2;

};