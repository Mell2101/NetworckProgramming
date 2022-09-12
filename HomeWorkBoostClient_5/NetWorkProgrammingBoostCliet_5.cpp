#include "create.h"

int main()
{
    CreateClient* tcpClient = new CreateClient;
    tcpClient->createClient();
   
    return 0;
}

