#include <chrono>
#include <fstream>
#include <mutex>
using Timepoint = std::chrono::time_point<std::chrono::steady_clock>;


class StopWatch {
private:
    std::mutex lock;
    std::string filepath;
    std::ofstream stream;
    std::string protocol;
    Timepoint start = Timepoint();
    Timepoint stop = Timepoint();

public:
    void CreateFile(std::string filename, std::string testrun, std::string protocol){
        std::lock_guard<std::mutex> guard(lock);
        this->protocol = protocol;
        stream.open(filename);
        stream << "Start of Testrun: " << testrun << " Start at: " << getCurrentTimeMs() << std::endl;
        stream << "time,protocol,issend,timediff,message" << std::endl;
    }

    void CloseFile(){
        std::lock_guard<std::mutex> guard(lock);
        stream << "File Closed at: " << getCurrentTimeMs() << std::endl;
        stream.close();
    }

    static std::chrono::milliseconds::rep getCurrentTimeMs(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    static Timepoint getCurrentTime() {
        return std::chrono::steady_clock::now();
    }

    void Start(){
        std::lock_guard<std::mutex> guard(lock);
        this->start = this->getCurrentTime();
    }
    void Stop(){
        std::lock_guard<std::mutex> guard(lock);
        this->stop = this->getCurrentTime();
    }

    static std::chrono::milliseconds::rep getTimeDif(Timepoint start, Timepoint stop){
        return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    }

    void CreateLogEntry(bool isSend, std::string message, Timepoint start, Timepoint stop){
        std::lock_guard<std::mutex> guard(lock);
        stream  << getCurrentTimeMs() << "," << this->protocol << "," << (isSend ? "send":"receive") << "," << (isSend ? 0 : getTimeDif(start, stop)) << "," << message << std::endl;
    }

    void CreateLogEntry(bool isSend, std::string message){
        std::lock_guard<std::mutex> guard(lock);
        stream  << getCurrentTimeMs() << "," << this->protocol << "," << (isSend ? "send":"receive") << "," << (isSend ? 0 : getTimeDif(this->start, this->stop)) << "," << message << std::endl;
    }
};
