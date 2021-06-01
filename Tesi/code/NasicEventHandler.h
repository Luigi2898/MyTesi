#ifndef NASICEVENTHANDLER_H
#define NASICEVENTHANDLER_H

#include <QObject>
#include <QThread>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "NasicVisitor.h"
#include "../../../Controllers/SimulationController.hpp"

class NasicPrimaryInputV;
class NasicPrimaryInputH;
class NasicVerticalEvent;
class NasicHorizontalEvent;
class PowerOn;
class EventNasic;
class NasicSimulationMatrixInfo;

using namespace std;

class NasicEventHandler : public QThread
{
    Q_OBJECT
public:
    NasicEventHandler(QObject *parent = 0);

    void setParameter(EventNasic &e, vector<boost::numeric::ublas::matrix<class  NasicSimulationMatrixInfo*> > &m);

    void run();
    
signals:
    
public slots:

private:

    EventNasic* m_Event;

    vector <NewMatrix_type>* m_Matrix;


    
};

#endif // NASICEVENTHANDLER_H
