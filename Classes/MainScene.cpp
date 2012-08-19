#include "MainScene.h"

USING_NS_CC;

CCScene* Main::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	Main *layer = Main::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Main::init()
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
										menu_selector(Main::menuCloseCallback) );


	CCLabelBMFont *label1 = CCLabelBMFont::labelWithString( "Crear mesa nueva", "fonts/bitmapFontTest3.fnt" );
	CCLabelBMFont *label2 = CCLabelBMFont::labelWithString( "Unirse a mesa", "fonts/bitmapFontTest3.fnt" );
	CCMenuItem *createTable = CCMenuItemLabel::itemWithLabel(label1, this, menu_selector(Main::menuNewTable) );
	CCMenuItem *joinTable = CCMenuItemLabel::itemWithLabel(label2, this, menu_selector(Main::menuJoinTable) );


	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	CCMenu *menu = CCMenu::menuWithItems(createTable,joinTable, NULL);
	menu->setPosition( ccp(size.width / 2, size.height - 80) );
	menu->alignItemsVertically();
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);
	this->addChild(menu,1);



	// add "Main" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("DominoBack.jpg");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
	
	return true;
}

void Main::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
/* Create a new table
 *
 */
void Main::menuNewTable(CCObject* pSender)
{
		CCScene* scene = CCScene::node();


	    Main* pLayer = Main::node();
	    scene->addChild( pLayer, 0 );

		CCLabelBMFont *label1 = CCLabelBMFont::labelWithString( "Esperando por los jugadores", "fonts/bitmapFontTest3.fnt" );
		this->addChild(label1,0);
		CCDirector::sharedDirector()->pushScene( CCTransitionSlideInT::transitionWithDuration(1, scene) );
	    scene->release();
	    pLayer->release();
	    //Changing to a "Waiting for players screen".

	    //runServer();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

/* Join a table
 *
 */
void Main::menuJoinTable(CCObject* pSender)
{
			CCScene* scene = CCScene::node();


			Main* pLayer =  Main::node();
		    scene->addChild( pLayer, 0 );

			CCLabelBMFont *label1 = CCLabelBMFont::labelWithString( "Insertar el numero del servidor", "fonts/bitmapFontTest3.fnt" );
			this->addChild(label1,0);
			CCDirector::sharedDirector()->pushScene( CCTransitionSlideInT::transitionWithDuration(1, scene) );
		    scene->release();
		    pLayer->release();
	//runClient();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
