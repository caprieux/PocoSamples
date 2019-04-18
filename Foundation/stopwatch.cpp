#include <Poco/StopWatch.h>
#include <unistd.h>
#include "sampleBase.h"

using Poco::Stopwatch;

// reference page :
// https://pocoproject.org/docs/Poco.Stopwatch.html
// https://pocoproject.org/docs/Poco.Clock.html
//
// need libPocoFoundation.so

class StopwatchSample : public SampleBase {
    public:
        StopwatchSample() {};
        ~StopwatchSample() {};

        int sample1BasicUsage(void);
        int sample2Reset(void);
    private:
        Stopwatch mStopwatch;

        StopwatchSample(const StopwatchSample&);
        StopwatchSample& operator=(const StopwatchSample&);
};

int StopwatchSample::sample1BasicUsage(void)
{
    Logger &log = createLogger(__func__);

    poco_information_f1(log, "Stopwatch resolution: %ld (int64)", mStopwatch.resolution());
    for (int i = 2;i > 0;--i) {
        poco_information(log, "Stopwatch start");
        mStopwatch.start();
        sleep(1);
        mStopwatch.stop();
        poco_information(log, "Stopwatch stop");
        poco_information_f1(log, "elapsed: %ld microsecond. (int64)", mStopwatch.elapsed());
        poco_information_f1(log, "elapsed: %d second.", mStopwatch.elapsedSeconds());
    }
    return 0; 
}

int StopwatchSample::sample2Reset(void)
{
    Logger &log = createLogger(__func__);

    poco_information(log, "Stopwatch restart. (reset and start)");
    mStopwatch.restart();
    sleep(1);
    mStopwatch.stop();
    poco_information(log, "Stopwatch stop");
    poco_information_f1(log, "elapsed: %ld microsecond. (int64)", mStopwatch.elapsed());
    poco_information_f1(log, "elapsed: %d second.", mStopwatch.elapsedSeconds());
    mStopwatch.reset();
    poco_information(log, "Stopwatch reset");
    poco_information_f1(log, "elapsed: %d second", mStopwatch.elapsedSeconds());
    return 0;
}

int main()
{
    StopwatchSample ss;
    ss.sample1BasicUsage();
    ss.sample2Reset();
    return 0;
}

