#pragma once

#include "IAsyncSocketCallbacks.h"
#include "AsyncTcpSocket.h"
#include "StopWatch.h"
#include "FileAbstraction.h"
#include <mutex>

enum TESTTYPE {
    KEYBOARD = 0,
    STARTFIRECLOSE = 1,
    STARTLOOPCLOSE = 2,
    STARTDOWNLOADCLOSE = 3,
    STARTFIREDOWNLOADCLOSE = 4
};
static TESTTYPE getTestTypeFromInt(int i){
    switch(i){
        case 0:
        return TESTTYPE::KEYBOARD;
        case 1: 
        return TESTTYPE::STARTFIRECLOSE;
        case 2:
        return TESTTYPE::STARTLOOPCLOSE;
        case 3:
        return TESTTYPE::STARTDOWNLOADCLOSE;
        case 4:
        return TESTTYPE::STARTFIREDOWNLOADCLOSE;
        default:
        return TESTTYPE::KEYBOARD;
    }
};
static std::string getStringfromTesttype(int i){
    switch(i){
        case 0:
        return "TESTTYPE::KEYBOARD";
        case 1: 
        return "TESTTYPE::STARTFIRECLOSE";
        case 2:
        return "TESTTYPE::STARTLOOPCLOSE";
        case 3:
        return "TESTTYPE::STARTDOWNLOADCLOSE";
        case 4:
        return "TESTTYPE::STARTFIREDOWNLOADCLOSE";
        default:
        return "TESTTYPE::KEYBOARD";
    }
};

class TcpClient : public IAsyncSocketCallbacks
{
private:

    std::string threadID = "";

    std::mutex mutex;
    bool isconnected = false;

    std::mutex received;
    bool isreceived = false;

    std::unique_ptr<StopWatch> sw = std::make_unique<StopWatch>();

    TESTTYPE testType = TESTTYPE::KEYBOARD;

    Timepoint start;
    Timepoint stop;

    //download stuff
    uint32_t bytesReceived = 0;
    uint32_t fileSize = 0;
    bool finishedDownload = false;
    bool startDownload = false;
    std::unique_ptr<FileAbstraction> downloadFile;

    //socket
    std::shared_ptr<AsyncTcpSocket> asyncSocket;

public:
    TcpClient() = default;
    virtual ~TcpClient() {};

    virtual void OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket) override;

    virtual void OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock) override;

    virtual void OnError(std::string err) override;

    virtual void OnBytesWritten(int bytes) override;

    void Start(std::string ip, uint16_t port, TESTTYPE testtype, uint16_t loops = 10);

    virtual void LOG(std::string log) override;

};