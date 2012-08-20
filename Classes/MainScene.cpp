#include "MainScene.h"
#include <stdlib.h>
#include <sstream>
#include <string>
#include "../qnx/server.h"
#include <pthread.h>
USING_NS_CC;

CCSize size;

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

	size = CCDirector::sharedDirector()->getWinSize();
	CCLabelBMFont *label1 = CCLabelBMFont::labelWithString( "Crear mesa nueva", "fonts/bitmapFontTest3.fnt" );
	CCLabelBMFont *label2 = CCLabelBMFont::labelWithString( "Unirse a mesa", "fonts/bitmapFontTest3.fnt" );
	CCMenuItem *createTable = CCMenuItemLabel::itemWithLabel(label1, this, menu_selector(Main::menuNewTable) );
	CCMenuItem *joinTable = CCMenuItemLabel::itemWithLabel(label2, this, menu_selector(Main::menuJoinTable) );


	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	CCMenu *menu = CCMenu::menuWithItems(createTable,joinTable, NULL);
	menu->setPosition( ccp(size.width / 2, size.height - 100) );
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

void *createServer(void* t){

	int r = runServer();

}
/* Create a new table
 *
 */
void Main::menuNewTable(CCObject* pSender)
{
		CCScene* scene = CCScene::node();
		CCLayer* pLayer = CCLayer::node();

	    //Creating waiting and loading scene
	    CCLabelBMFont *label1 = CCLabelBMFont::labelWithString("Loading...","fonts/bitmapFontTest3.fnt");
		pLayer->addChild(label1,1);
		CCSprite* waitingBack =  CCSprite::spriteWithFile("waitingBack.jpg");
		pLayer->addChild(waitingBack,0);
		label1->setPosition( ccp(size.width / 2, size.height - 150) );
		scene->addChild(pLayer, 0);
		CCDirector::sharedDirector()->replaceScene( scene );
		// Create a thread to run server
		pthread_t thread;
		long t;
		int rc= pthread_create(&thread, NULL, createServer, (void *)t);
	    if (rc){
	       printf("ERROR; return code from pthread_create() is %d\n", rc);
	       exit(-1);
	    }
	    printf("Im dad");
	    while (serverIp == NULL){
	    	printf("Server Ip is null");
	    }
	    //Sustitution of labels
	    pLayer->removeChild(label1,true);

	    //Show the serverIp address
//	    CCLabelBMFont *label2 = CCLabelBMFont::labelWithString(serverIp, "fonts/bitmapFontTest3.fnt" );
//	    CCLabelBMFont *label3 = CCLabelBMFont::labelWithString("Este es tu identificador. Compartelo con 3 amigos para que se unan a tu mesa!", "fonts/bitmapFontTest3.fnt" );
//
//	    //
//	    CCLabelBMFont *label4 = CCLabelBMFont::labelWithString( "Esperando por los otros jugadores", "fonts/bitmapFontTest3.fnt" );
//	    pLayer->addChild(label2,0);
//	    CCDirector::sharedDirector()->pushScene( scene );

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

			CCLayer* pLayer = CCLayer::node();
			CCSprite* sprite = CCSprite::spriteWithFile("dominoTable.jpg");
			addChild(sprite);
		    scene->addChild( pLayer, 0 );

		    CCLabelTTF* label = CCLabelTTF::labelWithString("Inserta el identificador", "Arial", 28);
			this->addChild(label,0);
			CCDirector::sharedDirector()->pushScene( CCTransitionSlideInT::transitionWithDuration(1, scene) );
		    scene->release();
		    pLayer->release();
	//runClient();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void Main::showPieces(){
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	int size = 40;
	for (int i=0; i<7; i++){
		std::stringstream ss;
		ss << (pieces[i].x) << (pieces[i].y);
		std::string f = ss.str();
		const char* file = f.c_str();
		CCSprite* sprite = CCSprite::spriteWithFile(file);
		addChild(sprite);
		sprite->setPosition( CCPointMake(s.width-size, s.height/2) );
		size = size -10;

	}
}





