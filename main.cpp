#include "server.h"
#include "client.h"

using namespace std;

int main()
{
   int runEl = 1;
   if(runEl == 0)
   {
      perror("Running server\n");
      runServer();
   }else{
      perror("Running client\n");
      runClient();
   }
}


