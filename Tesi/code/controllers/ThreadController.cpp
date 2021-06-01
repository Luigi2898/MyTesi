#include "ThreadController.hpp"

ThreadController *ThreadController::sm_ThreadController=0;

ThreadController::ThreadController(QObject *parent) :
    QObject(parent)
{
}

ThreadController::~ThreadController()
{
  ThreadController::deleteInstance();
}

ThreadController * ThreadController::instance(void){
  if(sm_ThreadController==0){
    sm_ThreadController = new ThreadController();
  }
  return sm_ThreadController;
}

void ThreadController::deleteInstance(void){
  delete sm_ThreadController;
  sm_ThreadController = 0;
}

void ThreadController::setThreadInfo(void){

    m_nThread = QThread::idealThreadCount();

    for(int i=0; i<m_nThread;i++){
        ThreadData threadData;
        m_threadList.append(threadData);
        m_threadList[i].thread = new QThread;
        m_threadList[i].label = new QLabel;
        m_threadList[i].progressBar = new QProgressBar;
        m_threadList[i].busyFlag = 0;
        qDebug() << "Thread" << i << "pointer:" << m_threadList[i].thread;
    }

    qDebug()<<"Topolinano creates"<<m_nThread<<"thread(s)";
}

bool ThreadController::getThreadData(ThreadData* threadBlock){
    bool find = false;

    for(int i=0; (i<m_nThread && find==false); i++){
        if(m_threadList[i].busyFlag == 0){
            find = true;
            m_threadList[i].busyFlag = 1;
            *threadBlock = m_threadList[i];
        }
    }

    return find;

}

bool ThreadController::releaseThreadData(ThreadData* threadBlock){
    qDebug() << "freeing:" << (*threadBlock).thread;
    (*threadBlock).busyFlag = 0;
    return true;
}
