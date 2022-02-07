#include "TcpClient.h"

void TcpClient::OnBytesReceived(const char *buf, int size, const AsyncTcpSocket* socket)
{
    std::string str(buf);
    std::cout << "Recived: " << str << std::endl;
}

void TcpClient::OnSocketConnectionChanged(ConnectionState state, const AsyncTcpSocket* sock)
{
    if (ConnectionState::Connected == state)
    {
        std::lock_guard<std::mutex> guard(mutex);
        isconnected = true;
        std::cout << "Connected" << std::endl;
    }
    else
    {
        std::cout << "Disconnected" << std::endl;
    }
}

void TcpClient::OnError(std::string err)
{
    std::cerr << "Error: " << err << std::endl;
}

void TcpClient::LOG(std::string log) {
    std::cout << "Log: " << log << std::endl;
}

void TcpClient::OnBytesWritten(int bytes)
{
    std::cout << "Bytes Written: " << bytes << std::endl;
}

void TcpClient::Start(std::string ip, uint16_t port)
{
    auto checkConnection = [=](){
        std::lock_guard<std::mutex> guard(mutex);
        return isconnected;
        }; 
    auto socket = std::make_shared<AsyncTcpSocket>(this);

    if (socket)
    {
        socket->ConnectSocketAsync(ip, port);
        waitFor(checkConnection, 10, 1000);

        socket->ReadAsync();

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
                    std::cout << "Wrote: " << message << std::endl;
                }
                else
                {
                    std::cout << "error while writing" << std::endl;
                }
            }
            else
            {
                socket->WriteAsync(message.c_str(), message.length());
            }
        }
         waitFor([&socket](){return socket->IsSocketClosed();}, 100, 5000);
    }

}

