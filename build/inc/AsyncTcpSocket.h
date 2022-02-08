#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <functional>
#include <thread>
#include <atomic>
#include <future>
#include "IAsyncSocketCallbacks.h"

    enum LOG{
        INFO = 0,
        DEBUG = 1,
        ERROR = 2,
    };

class AsyncTcpSocket
{

private:
    int sock = 0;
    std::atomic<bool> isClosed{false};
    std::atomic<bool> isConnected{false};
    std::atomic<bool> isTimeToClose{false};
    sockaddr_in address;
    const uint16_t BUFFER_SIZE = 0xFFFF;
    IAsyncSocketCallbacks* callback = nullptr;

public:

//Server Only
    AsyncTcpSocket(int sock, IAsyncSocketCallbacks* cb){
        this->callback = cb;
        this->sock = sock;
    }
    void SetClientAdress(sockaddr_in newSocketInfo){
        this-> address = newSocketInfo;
    }
    std::string ClientAddress() const
    {
        std::string s = ipToString(this->address) + ":" + std::to_string(ntohs(this->address.sin_port));
        return s;
    }
    int GetSocket() const 
    { 
        return this->sock; 
    }
    void SetCallback(IAsyncSocketCallbacks* callback){
        this->callback = callback;
    }

// Client and Server
    AsyncTcpSocket(IAsyncSocketCallbacks* cb)
    {
        this->callback = cb;
        if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            callback->OnError("Socket creating error.");
        }else{
            callback->LOG("Socket created");
        }
    };
    ~AsyncTcpSocket()
    {
        if(callback){
            //callback->LOG("Destroy AsyncSocket");
        }
        Close();
    };

    // Send TCP Packages
    int Write(const char *bytes, size_t byteslength) const
    {
        if (this->isClosed)
        {
            callback->OnError("Socket is closed");
            return -1;
        }

        int sent = 0;
        if ((sent = send(this->sock, bytes, byteslength, 0)) < 0)
        {
            callback->OnError("error while sending");
            return -1;
        }
        return sent;
    }
    // to write a string
    int Write(std::string message) const { return this->Write(message.c_str(), message.length()); }

    void WriteAsync(const char *bytes, size_t byteslength) const {
        auto future = std::async(&AsyncTcpSocket::_WriteAsync,this, bytes,byteslength);
    }


    void ConnectSocketAsync(std::string host, uint16_t port)
    {
        callback->LOG("Connect async to IP: " + host + ":" + std::to_string(port));
        auto future = std::async(&AsyncTcpSocket::ConnectSocket, this, host, port);
        int simple = future.get();

       /*  std::thread t(&AsyncTcpSocket::ConnectSocket, this, str, port);
        t.detach();  */
    }

    // Connect to a ip and port
    int ConnectSocket(std::string host, uint16_t port)
    {
        callback->LOG("Connect to IP: " + host + ":" + std::to_string(port));
        struct addrinfo hints, *res, *it;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        // Get address info from DNS
        int status;
        if ((status = getaddrinfo(host.c_str(), NULL, &hints, &res)) != 0)
        {
            if(callback){
                callback->OnError("Invalid address." + std::string(gai_strerror(status)));
            }
            return -1;
        }

        for (it = res; it != NULL; it = it->ai_next)
        {
            if (it->ai_family == AF_INET)
            { // IPv4
                memcpy((void *)(&address), (void *)it->ai_addr, sizeof(sockaddr_in));
                break; // for now, just get first ip (ipv4).
            }
        }

        freeaddrinfo(res);

        address.sin_family = AF_INET;
        address.sin_port = htons(port);

        setTimeout(5);
        // Try to connect.
        if (connect(sock, (const sockaddr *)&address, sizeof(sockaddr_in)) < 0)
        {
            callback->OnError("Connection failed to the host.");
            setTimeout(0);
            return -1;
        }
        else{
            if(callback){
                callback->OnSocketConnectionChanged(ConnectionState::Connected, this);
            }
        }

        setTimeout(0);
        return 1;
    }

    void ReadAsync()
    {
        std::thread listenThread(AsyncTcpSocket::StartListen, this);
        listenThread.detach();
    }

    void TimeToClose(){
        isTimeToClose = true;
    }
    bool IsTimeToClose() const {
        return isTimeToClose;
    }

    bool IsSocketClosed() const {
        return isClosed;
    }

    void Close()
    {
        if (isClosed)
        {
            return;
        }
        isClosed = true;
        if(0 != shutdown(sock, SHUT_RDWR)){
            callback->OnError("Shutdown does not work!");
            close(this->sock);
        }
        if (callback){
            callback->OnSocketConnectionChanged(ConnectionState::Disconnected, this);
        }
    }

private:

    void _WriteAsync(const char *bytes, size_t byteslength) const {
        auto i = this->Write(bytes,byteslength);
        if(callback){
            callback->OnBytesWritten(i);
        }
    }

    static void StartListen(AsyncTcpSocket *socket)
    {
        char tempBuffer[socket->BUFFER_SIZE];
        int messageLength;

        while (((messageLength = recv(socket->sock, tempBuffer, socket->BUFFER_SIZE, 0)) > 0) && !socket->IsTimeToClose())
        {
            tempBuffer[messageLength] = '\0';

            if (socket->callback){
                socket->callback->OnBytesReceived(tempBuffer,messageLength, socket);
            }
        }
        socket->Close();
    }

    void setTimeout(int seconds)
    {
        struct timeval tv;
        tv.tv_sec = seconds;
        tv.tv_usec = 0;

        setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
        setsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));
    }
};
