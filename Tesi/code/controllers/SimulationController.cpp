#include "SimulationController.hpp"
#include "../InputsAndClocks/Grammar/VWFGrammar.hpp"
#include "../InputsAndClocks/Parser/VWFParser.hpp"


SimulationController * SimulationController::sm_SimulationController = 0;

QElapsedTimer timer;
QWaitCondition bufferNotEmpty;
QWaitCondition bufferNotFull;
int readySamples = 0;
QMutex Mutex;
QMap<double,vector<double> > simulationBuffer;

void simCtrlInit(double delta){

  ThreadController::instance()->setThreadInfo();
  double tmp = round(SimulationController::instance()->getHeader()->getDataDuration() 
		     * SimulationController::instance()->getHeader()->getTimeUnit() /delta);
  SimulationController::instance()->setDelta(delta);
  double numRows=1+tmp;
  SimulationController::instance()->setSamplesNumber(numRows);
  qDebug() << "numSamples controller:" << SimulationController::instance()->getSamplesNumber();
}

SimulationController::SimulationController(QObject *parent) :
  QObject(parent)
{

}

SimulationController::~SimulationController()
{
  SimulationController::deleteInstance();
}

SimulationController * SimulationController::instance(void){
  if(sm_SimulationController==0){
    sm_SimulationController = new SimulationController();
  }
  return sm_SimulationController;
}

void SimulationController::deleteInstance(void){
  delete sm_SimulationController;
  sm_SimulationController = 0;
}

void SimulationController::setHeader(VWFHeader* h){
  m_Header = h;
}

VWFHeader* SimulationController::getHeader(){
  return m_Header;
}

void SimulationController::setSamplesNumber(double samples){

  m_SamplesNumber = samples;
}

double SimulationController::getSamplesNumber(void){

  return m_SamplesNumber;
}

void SimulationController::setDelta(double delta){

  m_Delta = delta;
}

double SimulationController::getDelta(void){

  return m_Delta;
}

void SimulationController::startSimulation(const QDir wDir, double delta){

  Stimuli *stimuli= Stimuli::instance();
  stimuli->setSamplesParameters(wDir,delta);

  VWFParser *p = VWFParser::instance();

  for(int i=0; i<2; i++){
    bool myTest = ThreadController::instance()->getThreadData(&m_dataBlocks[i]);
    if(myTest==true){
      qDebug() << "Thread trovato" << i << "ID" << m_dataBlocks[i].thread;
    }else{
      qDebug() << "Thread non trovato";
    }
  }

  QThread *generateSamplesThread = m_dataBlocks[0].thread;
  p->moveToThread(generateSamplesThread);
  m_NasicLogicSimulation = new NasicLogicSimulation;
  QThread *logicSimulationThread = m_dataBlocks[1].thread;
  m_NasicLogicSimulation->moveToThread(logicSimulationThread);

  connect(p, SIGNAL(finished()), p, SLOT(deleteLater()));
  connect(generateSamplesThread, SIGNAL(started()), p, SLOT(createSamples()));
  connect(m_NasicLogicSimulation, SIGNAL(finished()), logicSimulationThread, SLOT(terminate()));
  connect(logicSimulationThread, SIGNAL(started()), m_NasicLogicSimulation, SLOT(Simulate()));
  connect(m_NasicLogicSimulation, SIGNAL(finished()), this, SLOT(threadRelease()));

  generateSamplesThread->start();

  logicSimulationThread->start();

}

void SimulationController::threadRelease(){
  bool thread0 = ThreadController::instance()->releaseThreadData(&m_dataBlocks[0]);
  bool thread1 = ThreadController::instance()->releaseThreadData(&m_dataBlocks[1]);
}

