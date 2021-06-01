#ifndef SIMULATIONCONTROLLER_HPP
#define SIMULATIONCONTROLLER_HPP

#include <QObject>
#include <QSemaphore>
#include <math.h>
#include <QMutex>
#include <QDebug>
#include "../Actions.hpp"
#include "../NASIC/NASICSimulation/NasicLogicSimulation.hpp"
#include "ThreadController.hpp"

#define MAX_THREAD 32
#define MARGIN 4

extern QElapsedTimer timer;
extern QWaitCondition bufferNotEmpty;
extern QWaitCondition bufferNotFull;
extern int readySamples;
extern QMutex Mutex;
extern QMap<double,vector<double> > simulationBuffer;

class VWFHeader;

void simCtrlInit(double delta);

class SimulationController : public QObject
{
    Q_OBJECT
public:
    explicit SimulationController(QObject *parent = 0);
    ~SimulationController();
    static SimulationController * instance(void);
    static void deleteInstance(void);
    void setHeader(VWFHeader*);
    VWFHeader* getHeader();
    void setSamplesNumber(double samples);
    double getSamplesNumber(void);
    void setDelta(double delta);
    double getDelta(void);
    void startSimulation(const QDir wDir, double delta);

    
private:

    static SimulationController* sm_SimulationController;

    double m_SamplesNumber;

    VWFHeader *m_Header;

    double m_Delta;

    Algorithm *m_NasicLogicSimulation;

    ThreadData m_dataBlocks[2];

    NasicElectronicSimulation *m_NasicElectronicSimulation;

signals:

    
public slots:
    void threadRelease();
    
};

#endif // SIMULATIONCONTROLLER_HPP
