#pragma once
#include "AsyncTcpSocket.h"
class IServerCallbacks{
public:
    virtual void OnNewConnection(std::shared_ptr<AsyncTcpSocket> socket) = 0;

};