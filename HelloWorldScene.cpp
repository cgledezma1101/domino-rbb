#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"exit.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );


	CCLabelBMFont *label1 = CCLabelBMFont::labelWithString( "Crear mesa nueva", "fonts/bitmapFontTest3.fnt" );
	CCLabelBMFont *label2 = CCLabelBMFont::labelWithString( "Unirse a mesa", "fonts/bitmapFontTest3.fnt" );
	CCMenuItem *crearMesa = CCMenuItemLabel::itemWithLabel(label1, this, menu_selector(HelloWorld::menuCloseCallback) );
	CCMenuItem *unirseMesa = CCMenuItemLabel::itemWithLabel(label2, this, menu_selector(HelloWorld::menuCloseCallback) );


	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	CCMenu *menu = CCMenu::menuWithItems(crearMesa,unirseMesa, NULL);
	menu->setPosition( ccp(size.width / 2, size.height - 80) );
	menu->alignItemsVertically();
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);
	this->addChild(menu,1);



	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("DominoBack.jpg");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
	
	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
