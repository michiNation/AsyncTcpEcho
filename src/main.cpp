#include "iostream"
#include "AsyncTcpSocket.h"
#include "TcpClient.h"
#include "TcpServer.h"

int main(int argc, char *argv[])
{

    TcpClient client;
    TcpServer server;
    std::string mode = argv[3];
    if(argc > 2){
        if(mode.compare("client") == 0){
            
            client.Start(argv[1],atoi(argv[2]), getTestTypeFromInt(atoi(argv[4])));
        }
        else{
            server.Start(argv[1],atoi(argv[2]));    
        }
        
    }else{
        std::cout << "Usage: ./binary 127.0.0.1 5050 server" << std::endl;
        std::cout << "Usage: ./binary 127.0.0.1 5050 client <modeNr>" << std::endl;
        std::cout << "Modes for Client: KEYBOARD = 0;STARTFIRECLOSE = 1;STARTLOOPCLOSE = 2;STARTDOWNLOADCLOSE = 3;STARTFIREDOWNLOADCLOSE = 4" << std::endl;
    }

    return EXIT_SUCCESS; 
}