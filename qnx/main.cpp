#include "cocos2d.h"
#include "AppDelegate.h"
#include "server.h"
#include "client.h"


USING_NS_CC;

int main(int argc, char **argv)
{
   int runEl = 0;
   if(runEl == 0)
   {
      fprintf(stderr, "Running server\n");
      runServer();
   }else{
      fprintf(stderr, "Running client\n");
      runClient();
   }

	AppDelegate* app;
	int ret = 0;
	app = new AppDelegate();
	ret = cocos2d::CCApplication::sharedApplication().run();
	delete app;

	return ret;
}
