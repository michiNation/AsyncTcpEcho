#include "TcpClient.h"

void TcpClient::OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket)
{
    if(size <= 0){return;}
    std::string message = "";
    if(size > 15){
        std::string str(buf[0],buf[15]);
        message = str;
    }else{
        std::string str(buf);
        message = str;
    }
    switch(this->testType)
        {
        case TESTTYPE::KEYBOARD:
        {
            break;
        }
        case TESTTYPE::STARTFIRECLOSE:
        {
            std::lock_guard<std::mutex> guard(mutex);
            isreceived = true;
            break;
        }
        case TESTTYPE::STARTLOOPCLOSE:
        {
            std::lock_guard<std::mutex> guard(mutex);
            isreceived = true;
            break;
        }
        case TESTTYPE::STARTDOWNLOADCLOSE:
        {
            break;
        }
        case TESTTYPE::STARTFIREDOWNLOADCLOSE:
        {    break;
        }
        default:
            break;
        }
    sw->Stop();
    sw->CreateLogEntry(false, message);
    LOG("Recived: " + message);
}

void TcpClient::OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock)
{
    if (ConnectionState::Connected == state)
    {
        std::lock_guard<std::mutex> guard(mutex);
        isconnected = true;
        LOG("Connected");
    }
    else
    {
        LOG("Disconnected");
        sw->CloseFile();
    }
}

void TcpClient::OnBytesWritten(int bytes)
{
    LOG("Bytes Written: " + bytes);
}

void TcpClient::Start(std::string ip, uint16_t port, TESTTYPE testtype)
{
    this->testType = testtype;
    sw->CreateFile("TcpTest",getStringfromTesttype(static_cast<int>(testtype)),"TCP+STunnel");

    auto checkConnection = [=](){
        std::lock_guard<std::mutex> guard(mutex);
        return isconnected;
        }; 
    auto socket = std::make_shared<AsyncTcpSocket>(this);

    if (socket)
    {
        LOG("Timepoint Start: " +  std::to_string(sw->getCurrentTimeMs()));
        sw->Start();
        start = sw->getCurrentTime();
        sleep(1);
        socket->ConnectSocketAsync(ip, port);
        waitFor(checkConnection, 10, 2000);
        socket->ReadAsync();

        switch(testtype)
        {
        case TESTTYPE::KEYBOARD:
        {
                std::string message = "";
                while (std::getline(std::cin, message))
                {
                    if (message.compare("stop") == 0)
                    {
                        socket->TimeToClose();
                        socket->Close();
                        break;
                    }
                    if (message.empty())
                    {
                        continue;
                    }
                    if (!true) //switch to use async or sync write
                    {
                        int i = socket->Write(message);
                        if (i > 0)
                        {
                            LOG("Wrote: " + message);
                        }
                        else
                        {
                            LOG("error while writing");
                        }
                    }
                    else
                    {
                        socket->WriteAsync(message.c_str(), message.length());
                    }
                }
            break;
        }
        case TESTTYPE::STARTFIRECLOSE:
        {
            std::string message = "HelloFromClient";
            sw->CreateLogEntry(true, message);
            socket->WriteAsync(message.c_str(), message.length());
            LOG("Timepoint write: " + std::to_string(sw->getCurrentTimeMs()));
            waitFor([=](){
                std::lock_guard<std::mutex> guard(mutex);
                return isreceived;}, 10, 5000);
            socket->TimeToClose();
            socket->Close();
            LOG("Timepoint closed: " + std::to_string(sw->getCurrentTimeMs()));
            break;
        }
        case TESTTYPE::STARTLOOPCLOSE:
        {
            //todo check if file input is read
            FileAbstraction fa(true);
            fa.LodeFile("../MaxPackageFile");
            size_t filesize = fa.GetFileSize();
            std::vector<uint8_t> v = fa.ReadBytes(filesize);
            LOG("1");
            LOG("Filesize: " + std::to_string(filesize));
            LOG("2");
            LOG("Vectorsize: " + std::to_string(v.size()));
            LOG("3");
            std::string str(v.begin(), v.begin()+15);
            LOG("FirstFewBytes: " + str );
            sw->CreateLogEntry(true, "MaxPackageFileMessage");
            LOG("4");
            int i = 0;
            while(i < 10){
                LOG("5");
                socket->WriteAsync((const char *) v.data(),v.size());
                    waitFor([=](){
                std::lock_guard<std::mutex> guard(mutex);
                return isreceived;}, 10, 5000);
                LOG("6");
                {
                    std::lock_guard<std::mutex> guard(mutex);
                    isreceived = false;
                }
                i++;
                LOG("7");
            }
            socket->TimeToClose();
            socket->Close();
            fa.CloseFile();
            break;
        }
        case TESTTYPE::STARTDOWNLOADCLOSE:
        {
            break;
        }
        case TESTTYPE::STARTFIREDOWNLOADCLOSE:
        {    break;
        }
        default:
            break;
        }
         waitFor([&socket](){return socket->IsSocketClosed();}, 10, 5000);
    }
}

void TcpClient::OnError(std::string err)
{
    std::cerr << "Error: " << err << std::endl;
}

void TcpClient::LOG(std::string log) {
    std::cout << "Log: " << log << std::endl;
}
