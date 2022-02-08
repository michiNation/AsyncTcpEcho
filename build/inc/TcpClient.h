#pragma once

#include "IAsyncSocketCallbacks.h"
#include "AsyncTcpSocket.h"
#include "StopWatch.h"
#include <mutex>
#include <fstream>

class TcpClient : public IAsyncSocketCallbacks
{
private:
    std::mutex mutex;
    bool isconnected = false;
    std::unique_ptr<StopWatch> sw = std::make_unique<StopWatch>();

public:
    TcpClient() = default;
    virtual ~TcpClient() {};

    virtual void OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket) override;

    virtual void OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock) override;

    virtual void OnError(std::string err) override;

    virtual void OnBytesWritten(int bytes) override;

    void Start(std::string ip, uint16_t port, std::ifstream& infile, bool fromFile);

    virtual void LOG(std::string log) override;
};