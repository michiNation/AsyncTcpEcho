#include "iostream"
#include "AsyncTcpSocket.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "FileAbstraction.h"
#include <fstream>

int main(int argc, char *argv[])
{
    FileAbstraction outFa(false);
    outFa.LodeFile("hello.txt");
    std::string str = "HelloWorld";
    std::vector<uint8_t> v(str.begin(), str.end());
    outFa.WriteBytes(v);
    std::cout << "Size of hello.txt: " << outFa.GetFileSize() << std::endl;
    outFa.CloseFile();

    FileAbstraction inFa(true);
    inFa.LodeFile("hello.txt");
    std::cout << "Size of hello.txt: " << inFa.GetFileSize() << std::endl;
    auto v1 = inFa.ReadBytes(5);
    std::string str2(v1.begin(), v1.end());
    std::cout << "Read 4 bytes from File hello.txt. Input is: " << str2 << std::endl;
    inFa.CloseFile();
    return 0;
   /*     auto check = [](){return true;};
    std::cout << "Start" << std::endl;
    waitFor(check, 10, 100);
    std::cout << "Stop" << std::endl;
    return 1; */
    std::ifstream infile("../file");
    TcpClient client;
    TcpServer server;
    std::string mode = argv[3];
    if(argc > 2){
        if(mode.compare("client") == 0){
            std::string filemode = argv[4];
            client.Start(argv[1],atoi(argv[2]), infile, filemode.compare("true") == 0? true: false);
        }
        else{
            server.Start(argv[1],atoi(argv[2]));    
        }
        
    }else{
        std::cout << "Usage: ./binary 127.0.0.1 5050 <client|server> <true|false>" << std::endl;
    }

    return EXIT_SUCCESS; 
}