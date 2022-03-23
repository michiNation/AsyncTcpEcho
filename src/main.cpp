#include "iostream"
#include "AsyncTcpSocket.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include <thread>

int main(int argc, char *argv[])
{
    TcpClient client;
    TcpServer server;
    std::string mode = argv[3];
    if(argc > 2){
        if(mode.compare("client") == 0){
            if(argc == 6){
                std::cout  << "Start with 6 arguments";
                client.Start(argv[1],atoi(argv[2]), getTestTypeFromInt(atoi(argv[4])), atoi(argv[5]));
            }
            else if (argc == 5)
            {
                std::cout  << "Start with 5 arguments";
                client.Start(argv[1],atoi(argv[2]), getTestTypeFromInt(atoi(argv[4])));
            }
            else if(argc == 7){
                std::cout  << "Start with 7 arguments";
       
                std::vector<std::thread> threads;
                uint16_t instances = static_cast<uint16_t>(atoi(argv[6]));
                for(int i = 0; i < instances; i++){
                    auto c = std::make_shared<TcpClient>();
                    auto f = [c, argv](){c->Start(argv[1],atoi(argv[2]), getTestTypeFromInt(atoi(argv[4])), atoi(argv[5]));};
                    threads.push_back(std::thread{f});    
                }
                for (std::thread & th : threads)
                {
                    // If thread Object is Joinable then Join that thread.
                    if (th.joinable())
                        th.join();
                }
            }
            std::cout  << "something went wrong";
        }
        else{
            std::cout  << "Start server";
            server.Start(argv[1],atoi(argv[2]));    
        }
    }else{
        std::cout << "Usage: ./binary 127.0.0.1 5050 server" << std::endl;
        std::cout << "Usage: ./binary 127.0.0.1 5050 client <modeNr> <loops> <instances>" << std::endl;
        std::cout << "./binary 127.0.0.1 5050 client 2 10 5 will create uc 2 with 10 loops in 5 instances" << std::endl;
        std::cout << "Modes for Client: KEYBOARD = 0;STARTFIRECLOSE = 1;STARTLOOPCLOSE = 2;STARTDOWNLOADCLOSE = 3;STARTFIREDOWNLOADCLOSE = 4" << std::endl;
    }

    return EXIT_SUCCESS; 
}