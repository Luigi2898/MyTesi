#include "Connect.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

Connect::Connect()
{
    m_blockCode = "1111";
    m_blockName = "CONNECT";
}

Connect::~Connect()
{
}

bool Connect::generateEvent(NasicPrimaryInputH *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m)
{
    qDebug() << "connect primaryinputH event";
    // Posizione da cui deve partire l'evento
    pair<int, int> evPos, tmp, tmp_position;
    e->getPosition(&tmp);

    NasicPrimaryInputV *newPos = new NasicPrimaryInputV();
    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    newPos->setSimValue(e->getSimValue());
    newPos->setPosition(tmp);


    if(tmp.first+1 < m[e->getMatrixIndex()].size1()){
        NasicPrimaryInputV *eventNorth = new NasicPrimaryInputV();
        eventNorth->setSimDirection(SIMN);
        e->getPosition(&evPos);
        evPos.first++;
        eventNorth->setSimValue(e->getSimValue());
        eventNorth->setMatrixIndex(e->getMatrixIndex());
        eventNorth->setPosition(evPos);
        globalQueue.push(eventNorth);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicPrimaryInputV *eventNorth = new NasicPrimaryInputV();
            eventNorth->setSimDirection(SIMN);
            eventNorth->setSimValue(newPos->getSimValue());
            eventNorth->setMatrixIndex(newPos->getMatrixIndex());
            eventNorth->setPosition(tmp_position);
            globalQueue.push(eventNorth);
        }
    }

    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    e->getPosition(&tmp);
    newPos->setPosition(tmp_position);

    if(tmp.first-1 >= 0){
        NasicPrimaryInputV *eventSouth = new NasicPrimaryInputV();
        eventSouth->setSimDirection(SIMS);
        e->getPosition(&evPos);
        evPos.first--;
        eventSouth->setSimValue(e->getSimValue());
        eventSouth->setMatrixIndex(e->getMatrixIndex());
        eventSouth->setPosition(evPos);
        globalQueue.push(eventSouth);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicPrimaryInputV *eventSouth = new NasicPrimaryInputV();
            eventSouth->setSimDirection(SIMS);
            eventSouth->setSimValue(newPos->getSimValue());
            eventSouth->setMatrixIndex(newPos->getMatrixIndex());
            eventSouth->setPosition(tmp_position);
            globalQueue.push(eventSouth);
        }
    }


    return true;
}

bool Connect::generateEvent(NasicPrimaryInputV *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m)
{
    qDebug() << "connect primaryinputV event";
    // Posizione da cui deve partire l'evento
    pair<int, int> evPos, tmp, tmp_position;
    e->getPosition(&tmp);

    NasicPrimaryInputH *newPos = new NasicPrimaryInputH();
    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    newPos->setSimValue(e->getSimValue());
    newPos->setPosition(tmp);


    if(tmp.second+1 < m[e->getMatrixIndex()].size2()){
        NasicPrimaryInputH *eventRight = new NasicPrimaryInputH();
        eventRight->setSimDirection(SIMW);
        e->getPosition(&evPos);
        evPos.second++;
        eventRight->setSimValue(e->getSimValue());
        eventRight->setMatrixIndex(e->getMatrixIndex());
        eventRight->setPosition(evPos);
        globalQueue.push(eventRight);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicPrimaryInputH *eventRight = new NasicPrimaryInputH();
            eventRight->setSimDirection(SIMW);
            eventRight->setSimValue(newPos->getSimValue());
            eventRight->setMatrixIndex(newPos->getMatrixIndex());
            eventRight->setPosition(tmp_position);
            globalQueue.push(eventRight);
        }
    }

    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    e->getPosition(&tmp);
    newPos->setPosition(tmp_position);

    if(tmp.second-1 >= 0){
        NasicPrimaryInputH *eventLeft = new NasicPrimaryInputH();
        eventLeft->setSimDirection(SIME);
        e->getPosition(&evPos);
        evPos.second--;
        eventLeft->setSimValue(e->getSimValue());
        eventLeft->setMatrixIndex(e->getMatrixIndex());
        eventLeft->setPosition(evPos);
        globalQueue.push(eventLeft);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicPrimaryInputH *eventLeft = new NasicPrimaryInputH();
            eventLeft->setSimDirection(SIME);
            eventLeft->setSimValue(newPos->getSimValue());
            eventLeft->setMatrixIndex(newPos->getMatrixIndex());
            eventLeft->setPosition(tmp_position);
            globalQueue.push(eventLeft);
        }
    }
    return true;
}

bool Connect::generateEvent(NasicHorizontalEvent *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m)
{
    qDebug() << "connect horizontal event";
    // Posizione da cui deve partire l'evento
    pair<int, int> evPos, tmp, tmp_position;
    e->getPosition(&tmp);

    NasicVerticalEvent *newPos = new NasicVerticalEvent();
    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    newPos->setSimValue(e->getSimValue());
    newPos->setPosition(tmp);


    if(tmp.first+1 < m[e->getMatrixIndex()].size1()){
        NasicVerticalEvent *eventNorth = new NasicVerticalEvent();
        eventNorth->setSimDirection(SIMN);
        eventNorth->setAR(true);
        e->getPosition(&evPos);
        evPos.first++;
        eventNorth->setSimValue(e->getSimValue());
        eventNorth->setMatrixIndex(e->getMatrixIndex());
        eventNorth->setPosition(evPos);
        globalQueue.push(eventNorth);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicVerticalEvent *eventNorth = new NasicVerticalEvent();
            eventNorth->setSimDirection(SIMN);
            eventNorth->setAR(true);
            eventNorth->setSimValue(newPos->getSimValue());
            eventNorth->setMatrixIndex(newPos->getMatrixIndex());
            eventNorth->setPosition(tmp_position);
            globalQueue.push(eventNorth);
        }
    }

    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    e->getPosition(&tmp);
    newPos->setPosition(tmp_position);

    if(tmp.first-1 >= 0){
        NasicVerticalEvent *eventSouth = new NasicVerticalEvent();
        eventSouth->setSimDirection(SIMS);
        eventSouth->setAR(true);
        e->getPosition(&evPos);
        evPos.first--;
        eventSouth->setSimValue(e->getSimValue());
        eventSouth->setMatrixIndex(e->getMatrixIndex());
        eventSouth->setPosition(evPos);
        globalQueue.push(eventSouth);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicVerticalEvent *eventSouth = new NasicVerticalEvent();
            eventSouth->setSimDirection(SIMS);
            eventSouth->setAR(true);
            eventSouth->setSimValue(newPos->getSimValue());
            eventSouth->setMatrixIndex(newPos->getMatrixIndex());
            eventSouth->setPosition(tmp_position);
            globalQueue.push(eventSouth);
        }
    }
    return true;
}

bool Connect::generateEvent(NasicVerticalEvent *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m)
{
    qDebug() << "connect vertical event";
    // Posizione da cui deve partire l'evento
    pair<int, int> evPos, tmp, tmp_position;
    e->getPosition(&tmp);

    NasicHorizontalEvent *newPos = new NasicHorizontalEvent();
    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    newPos->setSimValue(e->getSimValue());
    newPos->setPosition(tmp);

    if(tmp.second+1 < m[e->getMatrixIndex()].size2()){
        NasicHorizontalEvent *eventRight = new NasicHorizontalEvent();
        eventRight->setSimDirection(SIMW);
        eventRight->setAR(true);
        e->getPosition(&evPos);
        evPos.second++;
        eventRight->setSimValue(e->getSimValue());
        eventRight->setMatrixIndex(e->getMatrixIndex());
        eventRight->setPosition(evPos);
        globalQueue.push(eventRight);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicHorizontalEvent *eventRight = new NasicHorizontalEvent();
            eventRight->setSimDirection(SIMW);
            eventRight->setAR(true);
            eventRight->setSimValue(newPos->getSimValue());
            eventRight->setMatrixIndex(newPos->getMatrixIndex());
            eventRight->setPosition(tmp_position);
            globalQueue.push(eventRight);
        }
    }

    newPos->setMatrixIndex(e->getMatrixIndex());
    newPos->setSimDirection(e->getSimDirection());
    e->getPosition(&tmp);
    newPos->setPosition(tmp_position);

    if(tmp.second-1 >= 0){
        NasicHorizontalEvent *eventLeft = new NasicHorizontalEvent();
        eventLeft->setSimDirection(SIME);
        eventLeft->setAR(true);
        e->getPosition(&evPos);
        evPos.second--;
        eventLeft->setSimValue(e->getSimValue());
        eventLeft->setMatrixIndex(e->getMatrixIndex());
        eventLeft->setPosition(evPos);
        globalQueue.push(eventLeft);
    }else{
        // find interconnection or output
        if(e->findOutputOrInterConnect(*newPos)){
            newPos->getPosition(&tmp_position);
            NasicHorizontalEvent *eventLeft = new NasicHorizontalEvent();
            eventLeft->setSimDirection(SIME);
            eventLeft->setAR(true);
            eventLeft->setSimValue(newPos->getSimValue());
            eventLeft->setMatrixIndex(newPos->getMatrixIndex());
            eventLeft->setPosition(tmp_position);
            globalQueue.push(eventLeft);
        }
    }
    return true;
}

void Connect::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class  NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    if(obj.getAR() == true){
        qDebug()<<"CONNECT: genero eventi";
        obj.manageConnect(this, m);
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getSimDirection()==SIMN){
        if(obj.getAR() == true){
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        if(tmp_position.first+1 < m[obj.getMatrixIndex()].size1()){
            //Destination position
            tmp_position.first++;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    else if(obj.getSimDirection()==SIMS){
        if(obj.getAR() == true){
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        if(tmp_position.first-1 >= 0){
            //Destination position
            tmp_position.first--;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    if(obj.getSimDirection()==SIMW){
        if(obj.getAR() == true){
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        if(tmp_position.second+1 < m[obj.getMatrixIndex()].size2()){
            //Destination position
            tmp_position.second++;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    if(obj.getSimDirection()==SIME){
        if(obj.getAR() == true){
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        if(tmp_position.second-1 >= 0){
            //Destination position
            tmp_position.second--;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }



}

void Connect::setBlockWidth(int w){
    m_blockWidth = w;
}

int Connect::getBlockWidth(void){
    return m_blockWidth;
}

void Connect::setBlockHeight(int h){
    m_blockHeight = h;
}

int Connect::getBlockHeight(void){
    return m_blockHeight;
}


QString Connect::getBlockCode(void){
    return m_blockCode;
}

QString Connect::getBlockName(void){
    return m_blockName;
}




