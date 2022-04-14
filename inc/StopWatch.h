#pragma once

#include <chrono>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iomanip>

using Timepoint = std::chrono::time_point<std::chrono::steady_clock>;


class StopWatch {
private:
    std::mutex lock;
    std::mutex lock_2;
    std::string filepath;
    std::ofstream stream;
    std::string protocol;
    Timepoint start = Timepoint();
    Timepoint stop = Timepoint();

    Timepoint connect = Timepoint();
    Timepoint disconnect = Timepoint();


    //File 2
    std::ofstream steam_2;

public:

    static std::string GetUtcString();

    void CreateFile(std::string filename, std::string testrun, std::string protocol);

    void CloseFile();

    static std::chrono::milliseconds::rep getCurrentTimeMicroSec();

    static std::chrono::milliseconds::rep getCurrentTimeMs();

    static Timepoint getCurrentTime() ;

    void Start();

    void Stop();

    void Connect();

    void Disconnect();

    static std::chrono::milliseconds::rep getTimeDifMicroSec(Timepoint start, Timepoint stop);

    static std::chrono::milliseconds::rep getTimeDifMilliSec(Timepoint start, Timepoint stop);

  /*   void CreateLogEntry(std::string eventstr, std::string message, Timepoint start, Timepoint stop){
        std::lock_guard<std::mutex> guard(lock);
        stream << getCurrentTimeMs() << "," << eventstr << "," << getTimeDifMilliSec(start, stop) << "," << message << std::endl;
    } */

    void CreateLogEntry(std::string eventstr, std::string message);

    void CreateConnectionTrackingEntry(std::string eventstr, std::string message);

    void ConnectedEvent(std::string message = "");

    void DisconnectedEvent(std::string message = "");

    void SendEvent(std::string message = "");

    void ReceivedEvent(std::string message = "null");
};
