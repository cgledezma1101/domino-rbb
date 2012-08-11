#include "server.h"
#include "client.h"

using namespace std;

int main()
{
   int runEl = 1;
   if(runEl == 0)
   {
      fprintf(stderr, "Running server\n");
      runServer();
   }else{
      fprintf(stderr, "Running client\n");
      runClient();
   }
}


