#pragma once

#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>


//Types:
    enum ConnectionState {
        Invalid = 0,
        Connected = 1,
        Disconnected = 2,
    };

    constexpr int MAXTOSTRING = 33;
    constexpr bool LOG_ON = true;
    constexpr bool ERRORLOG_ON = true;

//helpers
/**
 * Used to wait for *waitForTrue* Lamda.
 * Every *sleepms* the Lamda will be called again.
 * Once the *timeoutms* is reached it will terminate and log an error. 
 */
static void waitFor(std::function<bool(void)> waitForTrue, int sleepms, int timeoutms){
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    //std::cout << "start" << " check: " << waitForTrue() << std::endl;
    while(!waitForTrue()){
        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepms));
        if((now -start) > std::chrono::milliseconds(timeoutms)){
            std::cerr << "Timeout" << std::endl;
            break;
        }
       // std::cout << "next round" << std::endl;
    }
}
static void Sleep(int sleepms){
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepms));
}

static std::string ipToString(sockaddr_in addr)
{
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

    return std::string(ip);
}  

class AsyncTcpSocket;
//Callbacks
class IAsyncSocketCallbacks{
public:
    virtual void OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* sock) = 0;

    virtual void OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock) = 0;

    virtual void OnError(std::string err) = 0;

    virtual void OnBytesWritten(int bytes) = 0;

    virtual void LOG(std::string log) = 0;
};