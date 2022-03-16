#include "TcpClient.h"

void TcpClient::OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket)
{
    if(size <= 0){return;}
    std::string message = "";
    if(size > MAXTOSTRING){
        std::string str(&buf[0],&buf[15]);
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
            sw->Stop();
            sw->CreateLogEntry("RTT", "StartFireClose");
            std::lock_guard<std::mutex> guard(mutex);
            isreceived = true;
            break;
        }
        case TESTTYPE::STARTLOOPCLOSE:
        {
            sw->Stop();
            sw->CreateLogEntry("RTT", "StartLoopClose");
            std::lock_guard<std::mutex> guard(mutex);
            isreceived = true;
            break;
        }
        case TESTTYPE::STARTDOWNLOADCLOSE:
        {
            if(!startDownload){
                std::string str(&buf[0],&buf[9]);
                fileSize = std::stoi(str);
                LOG("Start Download - Filesize: " + std::to_string(fileSize) + " Received Bytes: " + std::to_string(size));
                downloadFile = std::make_unique<FileAbstraction>(false);
                //downloadFile->LodeFile("../VideoFileDownload.MOV");
                downloadFile->LodeFile("../BigFile1GBdw.zip");
                startDownload = true;
            }else{
                LOG("Next Chunk of: " + std::to_string(size) + " Bytessum: " + std::to_string(bytesReceived));
                
                bytesReceived += size;
                downloadFile->WriteBytes(buf, size);
                LOG("Written to file");
            }

            if(bytesReceived >= fileSize){
                std::lock_guard<std::mutex> guard(mutex);
                finishedDownload = true;
                LOG("Finished Download. Bytessum: " + std::to_string(bytesReceived));
            }
            sw->ReceivedEvent("message");
            sw->Stop();
            sw->CreateLogEntry("RTT", "Downloaded");
            return;
            //break;
        }
        case TESTTYPE::STARTFIREDOWNLOADCLOSE:
        {    break;
        }
        default:
            break;
        }
    sw->ReceivedEvent(message);
    LOG("Recived: " + message);
}

void TcpClient::OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock)
{
    if (ConnectionState::Connected == state)
    {
        std::lock_guard<std::mutex> guard(mutex);
        isconnected = true;
        LOG("Connected");
        sw->ConnectedEvent();
    }
    else
    {
        LOG("Disconnected");
        sw->DisconnectedEvent();
        sw->CloseFile();
    }
}

void TcpClient::OnBytesWritten(int bytes)
{
    LOG("Bytes Written: " + bytes);
}

void TcpClient::Start(std::string ip, uint16_t port, TESTTYPE testtype, uint16_t loops)
{
    this->testType = testtype;
    sw->CreateFile(("TcpTest_" + sw->GetUtcString()) ,getStringfromTesttype(static_cast<int>(testtype)),"TCP+STunnel");

    auto checkConnection = [=](){
        std::lock_guard<std::mutex> guard(mutex);
        return isconnected;
        }; 
    auto socket = std::make_shared<AsyncTcpSocket>(this);
    asyncSocket = socket;

    if (socket)
    {
        LOG("Timepoint Start: " +  std::to_string(sw->getCurrentTimeMs()));
        LOG("UseCase: " + std::to_string(testtype) + " Loops:" + std::to_string(loops));
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
            sw->SendEvent(message);
            sw->Start();
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
            fa.LodeFile("../Files/MaxPackageFile");
            size_t filesize = fa.GetFileSize();
            std::vector<uint8_t> v = fa.ReadBytes(filesize);
            std::string str(v.begin(), v.begin()+15);
            int i = 0;
            LOG("Loops: " + loops);
            while(i < loops){
                sw->SendEvent(str);
                sw->Start();
                socket->WriteAsync((const char *) &v[0],v.size());
                    waitFor([=](){
                std::lock_guard<std::mutex> guard(mutex);
                return isreceived;}, 10, 5000);
                {
                    std::lock_guard<std::mutex> guard(mutex);
                    isreceived = false;
                }
                i++;
            }
            socket->TimeToClose();
            socket->Close();
            fa.CloseFile();
            break;
        }
        case TESTTYPE::STARTDOWNLOADCLOSE:
        {
            sw->Start();
            socket->Write("Download");
            waitFor([=](){
                std::lock_guard<std::mutex> guard(mutex);
                return finishedDownload;}, 100, (1000 * 60 * 30)); //timeout 30 min

            socket->TimeToClose();
            socket->Close();
            downloadFile->CloseFile();
            break;
        }
        case TESTTYPE::STARTFIREDOWNLOADCLOSE:
        {    
            //would actually mean have to instances off the client and start usecase 2 and 3 together !
            break;
        }
        default:
            break;
        }
         waitFor([&socket](){return socket->IsSocketClosed();}, 10, 5000);
    }
}

void TcpClient::OnError(std::string err)
{
    if(ERRORLOG_ON){
        std::cerr << "Error: " << err << std::endl;
    }
}

void TcpClient::LOG(std::string log) {
    if(LOG_ON){
        std::cout << "Log: " << log << std::endl;
    }
}
