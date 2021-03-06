#pragma once

#include "TcpServerSocket.h"
#include "FileAbstraction.h"
#include "StopWatch.h"
class TcpServer : public IServerCallbacks, public IAsyncSocketCallbacks
{

public:

    TcpServer() {};

    virtual ~TcpServer();

    void StartDownload(const AsyncTcpSocket* socket);

    virtual void OnNewConnection(std::shared_ptr<AsyncTcpSocket> socket) override;

    virtual void OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket)  override;

    virtual void OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock)  override;

    virtual void OnError(std::string err)  override;

    virtual void OnBytesWritten(int bytes)  override;

    virtual void LOG(std::string log)  override;

    void Start(std::string ip, uint16_t port);

private:
    std::unique_ptr<FileAbstraction> fa = std::make_unique<FileAbstraction>(true);
    std::unique_ptr<StopWatch> sa2 = std::make_unique<StopWatch>();
};
