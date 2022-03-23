#include "TcpServer.h"
#include "TcpServerSocket.h"
#include "FileAbstraction.h"

void TcpServer::OnNewConnection(std::shared_ptr<AsyncTcpSocket> socket)
{
    LOG("New Connection: " + socket->ClientAddress());
    socket->SetCallback(this);
}

void TcpServer::OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket)
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
    auto found = message.find("Download");
    if(found != std::string::npos){
        StartDownload(socket);
    }
    else{
        LOG("Recived from " + socket->ClientAddress() + " size: " + std::to_string(size) + " : " + message);
        socket->WriteAsync(buf,size);
    }

}

void TcpServer::StartDownload(const AsyncTcpSocket* socket){
    //prep stuff
    uint16_t CHUNCKSIZE = 65000;
    LOG("Download requested");
    FileAbstraction fa(true);
    //fa.LodeFile("../Files/video.MOV");
    fa.LodeFile("../Files/BigFile1GB.zip");
    auto size = fa.GetFileSize();
    std::vector<uint8_t> v(CHUNCKSIZE);
    
    //send file size
    auto sent = socket->Write(std::to_string(size));
    LOG("Sent size: " + std::to_string(size) + " sent bytes: " + std::to_string(sent));
    //loop and read CHUNCKSIZE bytes from the file and send them to the client
    while(size > 0){
        //todo check why download is not working after a few packages
        if(size >= CHUNCKSIZE){
            size = size - CHUNCKSIZE;
        }
        else{
            CHUNCKSIZE = size;
            size = 0;
        }
        v = fa.ReadBytes(CHUNCKSIZE);
        auto sent = socket->Write((const char *)&v[0], v.size());
        LOG("Size: " + std::to_string(size) + " sent: " + std::to_string(sent));
    }
}

void TcpServer::OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock)
{
    if (ConnectionState::Connected == state)
    {
        LOG("Connected " + sock->ClientAddress());
    }
    else
    {
        LOG("Disconnected " + sock->ClientAddress());
    }
}


void TcpServer::OnBytesWritten(int bytes)
{
    LOG("OnBytesWritten: " + std::to_string(bytes));
}

void TcpServer::Start(std::string ip, uint16_t port){

    auto server = std::make_shared<TcpServerSocket>(this);

    server->Bind(ip.c_str(), port);

    server->Listen();

    std::string message = "";
    while (std::getline(std::cin, message))
    {
        if (message.compare("stop") == 0){
            break;
        }
    }

    server->Close();
}

void TcpServer::OnError(std::string err)
{
    if(ERRORLOG_ON){
        std::cerr << "Error: " << err << std::endl;
    }
}

void TcpServer::LOG(std::string log) {
    if(LOG_ON){
        std::cout << "Log: " << log << std::endl;
    }
}