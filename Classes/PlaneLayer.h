#include "cocos2d.h"

USING_NS_CC;

const int AIRPLANE=747;

class PlaneLayer : public CCLayer
{
public:

	PlaneLayer(void);

	~PlaneLayer(void);

	static PlaneLayer* create(); //ʵ��create����

	virtual bool init();

	void MoveTo(CCPoint location);

	void Blowup(int passScore);

	void RemovePlane();

public:
	static PlaneLayer* sharedPlane; //�ṩsharedPlaneȫ��ָ��

	bool isAlive;

	int score;

};
