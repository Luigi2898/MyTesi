#ifndef THREADCONTROLLER_HPP
#define THREADCONTROLLER_HPP

#include <QObject>
#include <QLabel>
#include <QThread>
#include <QProgressBar>
#include <QDebug>

//using namespace std;

typedef struct _ThreadData
{
    QThread* thread;
    QLabel* label;
    QProgressBar* progressBar;
    bool busyFlag;
}ThreadData;

class ThreadController : public QObject
{
    Q_OBJECT

public:
    explicit ThreadController(QObject *parent = 0);
    ~ThreadController();
    static ThreadController* instance();
    static void deleteInstance();
    void setThreadInfo();
    bool getThreadData(ThreadData* threadBlock);
    bool releaseThreadData(ThreadData* threadBlock);

private:
    static ThreadController* sm_ThreadController;
    QList<ThreadData> m_threadList;
    int m_nThread;

signals:

public slots:

};

#endif // THREADCONTROLLER_HPP
