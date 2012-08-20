#ifndef __Main_SCENE_H__
#define __Main_SCENE_H__

#include "cocos2d.h"
#include "Domino.h"
#include "../qnx/server.h"

class Main : public cocos2d::CCLayer
{
	Domino pieces[];
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);

	//Going to a new table
	void menuNewTable(CCObject* pSender);

	//Going to join a table
	void menuJoinTable(CCObject* pSender);

	void showPieces();
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(Main);
};

#endif // __Main_SCENE_H__
