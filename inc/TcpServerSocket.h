#pragma once

#include "AsyncTcpSocket.h"
#include "IAsyncSocketCallbacks.h"
#include <thread>
#include "IServerCallbacks.h"
#include <vector>
class TcpServerSocket
{
private:
    int sock = 0;
    sockaddr_in address;
    bool isClosed = false;
    IServerCallbacks* callback = nullptr;
    std::vector<std::shared_ptr<AsyncTcpSocket>> tcpSockets; 

public:

    void onError(std::string s){
        std::cout << s << std::endl;
    }

    virtual ~TcpServerSocket(){
        for (auto &&i : tcpSockets)
        {
            if(i && !i->IsSocketClosed()){
                i->Close();
            }
        }
    }

    TcpServerSocket(IServerCallbacks* callback)
    {
        this->callback = callback;

        if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            onError("Socket creating error.");
        }

        int opt = 1;
        setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
        setsockopt(this->sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
    }

    // Binding the server.
    void Bind(const char *address, uint16_t port)
    {
        if (inet_pton(AF_INET, address, &this->address.sin_addr) <= 0)
        {
            onError("Invalid address. Address type not supported.");
            return;
        }

        this->address.sin_family = AF_INET;
        this->address.sin_port = htons(port);

        if (bind(this->sock, (const sockaddr *)&this->address, sizeof(this->address)) < 0)
        {
            onError("Cannot bind the socket.");
            return;
        }
    }

    // Start listening the server.
    void Listen()
    {
        if (listen(this->sock, 20) < 0)
        {
            onError("Error: Server can't listen the socket.");
            return;
        }

        std::thread t(Accept, this);
        t.detach();
    }

    // Overriding Close to add shutdown():
    void Close()
    {
        shutdown(this->sock, SHUT_RDWR);

        if (isClosed)
            return;

        isClosed = true;
        close(this->sock);
    }

private:
    static void Accept(TcpServerSocket *server)
    {
        sockaddr_in newSocketInfo;
        socklen_t newSocketInfoLength = sizeof(newSocketInfo);

        int newSock;
        while (!server->isClosed)
        {
            while ((newSock = accept(server->sock, (sockaddr *)&newSocketInfo, &newSocketInfoLength)) < 0)
            {
                if (errno == EBADF || errno == EINVAL)
                    return;

                server->onError("Error while accepting a new connection.");
                return;
            }

            if (!server->isClosed && newSock >= 0)
            {
                auto asyncSocket = std::make_shared<AsyncTcpSocket>(newSock, nullptr);
                
                asyncSocket->SetClientAdress(newSocketInfo);
                
                server->tcpSockets.push_back(asyncSocket);

                if(server->callback){
                    server->callback->OnNewConnection(asyncSocket);
                }
                asyncSocket->ReadAsync(); 
            }
        }
    }
};