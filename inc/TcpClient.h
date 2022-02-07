#pragma once

#include "IAsyncSocketCallbacks.h"
#include "AsyncTcpSocket.h"
#include <mutex>

class TcpClient : public IAsyncSocketCallbacks
{
private:
    std::mutex mutex;
    bool isconnected = false;

public:
    TcpClient() = default;
    virtual ~TcpClient() {};

    virtual void OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket) override;

    virtual void OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock) override;

    virtual void OnError(std::string err) override;

    virtual void OnBytesWritten(int bytes) override;

    void Start(std::string ip, uint16_t port);

    virtual void LOG(std::string log) override;
};