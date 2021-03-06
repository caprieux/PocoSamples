#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/Stopwatch.h>
#include <string>

using Poco::Logger;
using Poco::AutoPtr;
using Poco::ConsoleChannel;
using Poco::Stopwatch;
using std::string;

class SampleBase {
    public:
        SampleBase () : mCC(new ConsoleChannel) {};
        ~SampleBase () { Logger::shutdown(); };

        Logger& createLogger(string name);
    protected:
        Stopwatch   mTimer;
    private:
        AutoPtr<ConsoleChannel> mCC;

        SampleBase(const SampleBase&);
        SampleBase& operator=(const SampleBase&);
};

Logger& SampleBase::createLogger(string name)
{
    Logger &log = Logger::get(name);
    log.setChannel(mCC);
    return log;
}
