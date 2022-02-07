#include "iostream"
#include "AsyncTcpSocket.h"
#include "TcpClient.h"
#include "TcpServer.h"

int main(int argc, char *argv[])
{

/*     auto check = [](){return true;};
    std::cout << "Start" << std::endl;
    waitFor(check, 10, 100);
    std::cout << "Stop" << std::endl;
    return 1; */

    TcpClient client;
    TcpServer server;
    std::string mode = argv[3];
    if(argc > 2){
        if(mode.compare("client") == 0){
            client.Start(argv[1],atoi(argv[2]));
        }
        else{
            server.Start(argv[1],atoi(argv[2]));    
        }
        
    }else{
        std::cout << "Usage: ./binary 127.0.0.1 5050 <client|server>" << std::endl;
    }

    return EXIT_SUCCESS; 
}