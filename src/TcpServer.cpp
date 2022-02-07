#include "TcpServer.h"
#include "TcpServerSocket.h"


void TcpServer::OnNewConnection(std::shared_ptr<AsyncTcpSocket> socket)
{
    std::cout << "New Connection: " << socket->ClientAddress() << std::endl;
    socket->SetCallback(this);
}

void TcpServer::OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket)
{
    std::string str(buf);
    std::cout << "Recived from " << socket->ClientAddress() << ": " << str << std::endl;
    socket->WriteAsync(buf,size);
}

void TcpServer::OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock)
{
    if (ConnectionState::Connected == state)
    {
        std::cout << "Connected " << sock->ClientAddress() << std::endl;
    }
    else
    {
        std::cout << "Disconnected " << sock->ClientAddress() << std::endl;
    }
}

void TcpServer::OnError(std::string err)
{
    std::cerr << "Error: " << err << std::endl;
}

void TcpServer::OnBytesWritten(int bytes)
{
    std::cout << "OnBytesWritten: " << std::to_string(bytes) << std::endl;
}

void TcpServer::LOG(std::string log) 
{
    std::cout << "LOG: " << log << std::endl;
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