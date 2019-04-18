#include "sampleBase.h"

#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>

using Poco::Thread;
using Poco::Runnable;
using Poco::Mutex;
using Poco::ScopedLock;

#include <unistd.h>
#ifdef __APPLE__
//#include <sys/syscall.h>
//#define gettid()  syscall(SYS_thread_selfid)  //syscall was first deprecated in macOS 10.12
#include <pthread.h>
inline uint64_t gettid() {
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    return tid;
}
#else
#include <sys/types.h>
#endif

class ThreadSample : public SampleBase {
    public:
        ThreadSample () {};
        ~ThreadSample () {};

        int sample1BasicUsage();
        int sample2MutexUsage();
    private:
        class Worker : public Runnable {
            public:
                enum Type {
                    SAMPLE1,
                    SAMPLE2
                };
                Worker(Type type, Logger &log) :
                    mType(type),
                    mStop(false),
                    mLogger(&log) {};
                ~Worker() {};
                virtual void run();
                void setLogger(Logger &log) { mLogger = &log; };
                void setMutex(Mutex &mutex) { mMutex = &mutex; };
            private:
                Type mType;
                bool mStop;
                Logger *mLogger;
                Mutex *mMutex;
        };
};

void ThreadSample::Worker::run()
{
    switch (mType) {
        case SAMPLE1:
            if (mLogger) {
                int tid = (int)gettid();
                poco_warning_f1(*mLogger, "run in thread : %d", tid);
            }
            break;
        case SAMPLE2:
            {
                Mutex::ScopedLock m(*mMutex); //equal to ScopedLock<Mutex> m(*mMutex);
                sleep(1);
                poco_warning(*mLogger, "sleep in sample2 done");
            }
            break;
        default:
            poco_warning_f1(*mLogger, "unknown type : ", (int)mType);
            break;
    }
    return;
}

int ThreadSample::sample1BasicUsage()
{
    Logger& log = createLogger(__func__);

    Worker worker(Worker::SAMPLE1, log);
    poco_warning_f1(log, "main thread : %d", (int)gettid());

    Thread thread;
    thread.start(worker);
    thread.join();
    return 0;
}

int ThreadSample::sample2MutexUsage()
{
    Logger& log = createLogger(__func__);
    Mutex mutex;

    Worker worker(Worker::SAMPLE2, log);
    worker.setMutex(mutex);

    Thread thread;

    mTimer.start();
    sleep(1);   // guarentee thread start to run at first

    thread.start(worker);

    mutex.lock();
    sleep(1);
    mutex.unlock();

    thread.join();

    mTimer.stop();
    log.warning("elasped time: %ld microseconds", mTimer.elapsed());
    return 0;
}

int main(int argc, char** argv)
{
    ThreadSample ts;

    ts.sample1BasicUsage();
    ts.sample2MutexUsage();
    return 0;
}

