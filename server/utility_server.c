#include "utility_server.h"


#define GET_PORT 1
#define GET_IP 2

char * get_ip_port(int argv,char *argc[],int optiune)
{

    for(int i=1;i<argv;i++)
        if(optiune==GET_PORT&&i<argv&&strcmp(argc[i],"-p")==0)
        {
            return argc[i+1];
        }
        else if(optiune==GET_IP&&i<argv&&strcmp(argc[i],"-ip")==0)
        {
            return argc[i+1];
        }
    return NULL;

}
