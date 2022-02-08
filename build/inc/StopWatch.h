#include <chrono>
#include <fstream>

using Timepoint = std::chrono::time_point<std::chrono::steady_clock>;


class StopWatch {
private:
    std::string filepath;
    std::ofstream stream;
    std::string protocol;
    Timepoint start;
    Timepoint stop;

public:
    void CreateFile(std::string filename, std::string testrun, std::string protocol){
        this->protocol = protocol;
        stream.open(filename);
        stream << "Start of Testrun: " << testrun << " at UTC" << std::endl;
    }

    void CloseFile(){
        stream << "File Closed" << std::endl;
        stream.close();
    }

    static auto getCurrentTimeMs(){
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    static Timepoint getCurrentTime() {
        return std::chrono::steady_clock::now();
    }

    void Start(){
        this->start = this->getCurrentTime();
    }
    void Stop(){
        this->stop = this->getCurrentTime();
    }

    static std::chrono::microseconds::rep getTimeDif(Timepoint start, Timepoint end){
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    void CreateLogEntry(bool isSend, std::string message, Timepoint start, Timepoint end){
        stream  << getCurrentTimeMs() << ";" << this->protocol << ";" << (isSend ? "send":"receive") << ";" << (isSend ? 0 : getTimeDif(start, end)) << ";" << message << std::endl;
    }

        void CreateLogEntry(bool isSend, std::string message){
        stream  << getCurrentTimeMs() << ";" << this->protocol << ";" << (isSend ? "send":"receive") << ";" << (isSend ? 0 : getTimeDif(this->start, this->stop)) << ";" << message << std::endl;
    }
};
