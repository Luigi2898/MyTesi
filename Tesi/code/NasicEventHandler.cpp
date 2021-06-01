#include "NasicEventHandler.h"
#include "EventNasic.h"
#include "NasicPrimaryInputV.h"
#include "NasicPrimaryInputH.h"
#include "NasicHorizontalEvent.h"
#include "NasicVerticalEvent.h"
#include "PowerOn.h"
#include "NasicLogicSimulation.hpp"

NasicEventHandler::NasicEventHandler(QObject *parent) :
    QThread(parent)
{

}

void NasicEventHandler::setParameter(EventNasic &e, vector<boost::numeric::ublas::matrix<class  NasicSimulationMatrixInfo*> > &m)
{
    
    m_Event = &e;
    m_Matrix = &m;
    pair<int,int> Position;
    m_Event->getPosition(&Position);
    
}

void NasicEventHandler::run()
{
    
    m_Event->handleThreadedEvent(m_Matrix[0]);

}
