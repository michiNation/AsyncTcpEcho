#include "StopWatch.h"


    std::string StopWatch::GetUtcString(){
        auto t = std::time(nullptr);
        //auto tm = *std::localtime(&t);
        auto tm = *std::gmtime(&t);
        auto str = std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        auto ms = getCurrentTimeMicroSec() % 1000000; //to get micro seconds
        std::ostringstream oss;
        oss << str << ":" << std::to_string(ms);
        return oss.str();
    }

    void StopWatch::CreateFile(std::string filename, std::string testrun, std::string protocol){

        //File 1 for Events
        {
            std::lock_guard<std::mutex> guard(lock);
            this->protocol = protocol;
            stream.open(filename);
            stream << "Start of Testrun: " << testrun << " Start at: " << GetUtcString() << std::endl;
            stream << "EpochMs,Event,message" << std::endl;
        }

        {
            //File 2 for RTT Events
            std::lock_guard<std::mutex> guard(lock_2);
            steam_2.open(filename + "RTT");
            steam_2 << "Start of Testrun: " << testrun << " Start at: " << GetUtcString() << std::endl;
            steam_2 << "EpochMs,Event,RTT,Usecase" << std::endl;
        }

    }

    void StopWatch::CloseFile(){
        {
            std::lock_guard<std::mutex> guard(lock);
            stream << "File Closed at: " << GetUtcString() << std::endl;
            stream.close();
        }
        {
            std::lock_guard<std::mutex> guard(lock_2);
            stream << "File Closed at: " << GetUtcString() << std::endl;
            steam_2.close();
        }
    }

    std::chrono::milliseconds::rep StopWatch::getCurrentTimeMicroSec(){
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    std::chrono::milliseconds::rep StopWatch::getCurrentTimeMs(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    Timepoint StopWatch::getCurrentTime() {
        return std::chrono::steady_clock::now();
    }

    void StopWatch::Start(){
        std::lock_guard<std::mutex> guard(lock);
        this->start = this->getCurrentTime();
    }
    void StopWatch::Stop(){
        std::lock_guard<std::mutex> guard(lock);
        this->stop = this->getCurrentTime();
    }

    void StopWatch::Connect(){
        std::lock_guard<std::mutex> guard(lock);
        this->connect = this->getCurrentTime();
    }
    void StopWatch::Disconnect(){
        std::lock_guard<std::mutex> guard(lock);
        this->disconnect = this->getCurrentTime();
    }

    std::chrono::milliseconds::rep StopWatch::getTimeDifMicroSec(Timepoint start, Timepoint stop){
        return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    }
    std::chrono::milliseconds::rep StopWatch::getTimeDifMilliSec(Timepoint start, Timepoint stop){
        return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    }

  /*   void CreateLogEntry(std::string eventstr, std::string message, Timepoint start, Timepoint stop){
        std::lock_guard<std::mutex> guard(lock);
        stream << getCurrentTimeMs() << "," << eventstr << "," << getTimeDifMilliSec(start, stop) << "," << message << std::endl;
    } */

    void StopWatch::CreateLogEntry(std::string eventstr, std::string message){
        std::lock_guard<std::mutex> guard(lock_2);
        steam_2  << getCurrentTimeMs() << "," << eventstr << "," << getTimeDifMilliSec(this->start, this->stop) << "," << message << std::endl;
    }

    void StopWatch::CreateConnectionTrackingEntry(std::string eventstr, std::string message){
        std::lock_guard<std::mutex> guard(lock);
        stream  << getCurrentTimeMs() << "," << eventstr << "," << getTimeDifMilliSec(this->connect, this->disconnect) << "," << message << std::endl;
    }

    void StopWatch::ConnectedEvent(std::string message){
        std::lock_guard<std::mutex> guard(lock);
        stream << getCurrentTimeMs() << ",Connected," << message << std::endl;
    }

    void StopWatch::DisconnectedEvent(std::string message){
        std::lock_guard<std::mutex> guard(lock);
        stream << getCurrentTimeMs() << ",Disconnected," << message << std::endl;
    }

    void StopWatch::SendEvent(std::string message){
        std::lock_guard<std::mutex> guard(lock);
        stream << getCurrentTimeMs() << ",Send," << message << std::endl;
    }

    void StopWatch::ReceivedEvent(std::string message){
        std::lock_guard<std::mutex> guard(lock);
        stream << getCurrentTimeMs() << ",Received," << message << std::endl;
    }
