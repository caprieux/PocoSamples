#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include <iostream>


using Poco::Logger;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;
using std::cout;
using std::endl;


class LoggerSample {
public:
    LoggerSample() : mCc(new ConsoleChannel()) {};
    ~LoggerSample();

    int sample1BasicUsage(void);
    int sample2FormatUsage(void);
    int sample3FileLogUsage(void);
    int sample4LogFunctions(void);
private:
    // All classes that inherit RefCountedObject don't have public destructor, we need to call release() to decrease
    // reference count and then trigger "delete this".
    // So it is better to maintain by a AutoPtr for convenience (don't need to call release() explicitly).
    AutoPtr<ConsoleChannel> mCc;

    LoggerSample(const LoggerSample& l);
    LoggerSample& operator=(const LoggerSample& l);
};
LoggerSample::~LoggerSample()
{
    // All log instances are maintained by the static variable Logger::LoggerMap. (Logger::Logger is protected)
    // Need to call shutdown to release all Logger objects and remove the reference count of related Channel.
    Logger::shutdown();
}

int LoggerSample::sample1BasicUsage(void)
{
    Logger &log = Logger::get("sample1");
    log.setChannel(mCc);
    // default level is info
    // more level : https://pocoproject.org/docs/Poco.Message.html
    log.warning("sample1BasicUse : test warning");

    Logger::shutdown();
    return 0;
}

int LoggerSample::sample2FormatUsage(void)
{
    //      %s : message source = Logger name
    // MUST %t : message text = the content of log
    // more format symbols : https://pocoproject.org/docs/Poco.PatternFormatter.html
    AutoPtr<FormattingChannel> pFc(new FormattingChannel(new PatternFormatter("%s : %t")));
    pFc->setChannel(mCc);

    Logger &log = Logger::get("sample2FormatUsage");
    log.setChannel(pFc);

    log.warning("test warning");

    Logger::shutdown();
    //To release anonymous PatternFormatter object
    pFc->setFormatter(NULL);
    return 0;
}

int LoggerSample::sample3FileLogUsage(void)
{
    AutoPtr<FileChannel> pFC(new FileChannel("sample.log"));
	Logger& fileLogger = Logger::create("FileLogger", pFC, Message::PRIO_INFORMATION);

    fileLogger.warning("test file warning");

    Logger::shutdown();
    return 0;
}

int LoggerSample::sample4LogFunctions(void)
{
    AutoPtr<PatternFormatter> pPF(new PatternFormatter("%s :%U:%u: %t"));
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, mCc));
    Logger &log = Logger::get("sample4LogFunctions");
    log.setChannel(pFC);

    // the format don't support %s
    // convenience macros always have File and Line, but formatter need to set %U and %u
    poco_warning(log, "poco_warning test");
    poco_warning_f1(log, "poco_warning_f1 test %d", 1);
    poco_warning_f2(log, "poco_warning_f1 test %d %c", 1, '2');
    poco_warning_f3(log, "poco_warning_f1 test %d %c %f", 1, '2', 3.0);
    // max with 4 arguments
    poco_warning_f4(log, "poco_warning_f1 test %d %c %f %b", 1, '2', 3.0, 4);

    // the format is the same with poco_warning, don't support %s
    log.warning("log warning test");
    // except this function, don't have File and Line in others
    log.warning("log warning test", __FILE__, __LINE__);
    log.warning("log warning test with 1 arg %d", 1);
    // max with 10 arguments
    log.warning("log warning test with 10 arg %d %d %d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

    Logger::shutdown();
    return 0;
}

int main(int argc, char** argv)
{
    LoggerSample ls;
    ls.sample1BasicUsage();
    ls.sample2FormatUsage();
    ls.sample3FileLogUsage();
    ls.sample4LogFunctions();

	return 0;
}

