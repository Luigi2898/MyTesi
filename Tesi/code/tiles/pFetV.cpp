#include "pFetV.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

pFetV::pFetV()
{
    m_blockCode = "00";
    m_blockName = "PFETV";
}

pFetV::~pFetV()
{
}

bool pFetV::handleEvent(NasicVerticalEvent *e)
{
    pair<int, int> tmpStart, tmpCurrent;
    int counter;
    e->getStartingPosition(&tmpStart);
    e->getPosition(&tmpCurrent);
    counter = e->getCounter();
    if(tmpStart.first == tmpCurrent.first && tmpStart.second == tmpCurrent.second){
        counter++;
        e->setCounter(counter);
        if( counter == 2 ){
            qDebug() << "Terminazione dell'evento secondario";
            return false;
        }
    }
    qDebug() << "L?evento secondario deve proseguire";
    return true;
}

void pFetV::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getSimDirection() == SIMN){
        if(tmp_position.first+1 < m[obj.getMatrixIndex()].size1()){
            //Destination position
            if((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState())==0){
                tmp_position.first++;
                obj.setPosition(tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    else if(obj.getSimDirection()==SIMS){
        if(tmp_position.first-1 >= 0){
            //Destination position
            if((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState())==0){
                obj.getPosition(&tmp_position);
                tmp_position.first--;
                obj.setPosition(tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    else if(obj.getSimDirection() == SIMW){
        // If true update FET state
        if(obj.getAR()){
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        // Se arrivo da EST e accendo il transistor creo gli eventi orizzontali
        if (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState() == 0){
            pair<int, int> evPos;
            NasicVerticalEvent *newPos = new NasicVerticalEvent();
            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);


            if(tmp_position.first-1 >= 0){
                NasicVerticalEvent *eventNorth = new NasicVerticalEvent();
                obj.getPosition(&evPos);
                evPos.first--;
                eventNorth->setSimDirection(SIMS);
                eventNorth->setMatrixIndex(obj.getMatrixIndex());
                eventNorth->setPosition(evPos);
                // Impostazione della posizione di terminazione
                eventNorth->setStartingPosition(tmp_position);
                globalQueue.push(eventNorth);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicVerticalEvent *eventNorth = new NasicVerticalEvent();
                    eventNorth->setSimDirection(SIMS);
                    eventNorth->setMatrixIndex((*newPos).getMatrixIndex());
                    eventNorth->setPosition(evPos);
                    eventNorth->setStartingPosition(tmp_position);
                    globalQueue.push(eventNorth);

                }
            }

            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);

            if(tmp_position.first+1 < m[obj.getMatrixIndex()].size1()){
                NasicVerticalEvent *eventSouth = new NasicVerticalEvent();
                obj.getPosition(&evPos);
                evPos.first++;
                eventSouth->setSimDirection(SIMN);
                eventSouth->setMatrixIndex(obj.getMatrixIndex());
                eventSouth->setPosition(evPos);
                // Impostazione della posizione di terminazione
                eventSouth->setStartingPosition(tmp_position);
                globalQueue.push(eventSouth);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicVerticalEvent *eventSouth = new NasicVerticalEvent();
                    eventSouth->setSimDirection(SIMN);
                    eventSouth->setMatrixIndex((*newPos).getMatrixIndex());
                    eventSouth->setPosition(evPos);
                    eventSouth->setStartingPosition(tmp_position);
                    globalQueue.push(eventSouth);

                }
            }
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
    else if(obj.getSimDirection()==SIME){
        // If true update FET state
        if(obj.getAR()){
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        // Se arrivo da EST e accendo il transistor creo gli eventi orizzontali
        if (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState() == 0){
            pair<int, int> evPos;
            NasicVerticalEvent *newPos = new NasicVerticalEvent();
            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);


            if(tmp_position.first-1 >= 0){
                NasicVerticalEvent *eventNorth = new NasicVerticalEvent();
                eventNorth->setSimDirection(SIMS);
                obj.getPosition(&evPos);
                evPos.first--;
                eventNorth->setMatrixIndex(obj.getMatrixIndex());
                eventNorth->setPosition(evPos);
                eventNorth->setStartingPosition(tmp_position);
                globalQueue.push(eventNorth);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicVerticalEvent *eventNorth = new NasicVerticalEvent();
                    eventNorth->setSimDirection(SIMS);
                    eventNorth->setMatrixIndex((*newPos).getMatrixIndex());
                    eventNorth->setPosition(evPos);
                    eventNorth->setStartingPosition(tmp_position);
                    globalQueue.push(eventNorth);

                }
            }

            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);

            if(tmp_position.first+1 < m[obj.getMatrixIndex()].size1()){
                NasicVerticalEvent *eventSouth = new NasicVerticalEvent();
                eventSouth->setSimDirection(SIMN);
                obj.getPosition(&evPos);
                evPos.first++;
                eventSouth->setMatrixIndex(obj.getMatrixIndex());
                eventSouth->setPosition(evPos);
                eventSouth->setStartingPosition(tmp_position);
                globalQueue.push(eventSouth);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicVerticalEvent *eventSouth = new NasicVerticalEvent();
                    eventSouth->setSimDirection(SIMN);
                    eventSouth->setMatrixIndex((*newPos).getMatrixIndex());
                    eventSouth->setPosition(evPos);
                    eventSouth->setStartingPosition(tmp_position);
                    globalQueue.push(eventSouth);

                }
            }

        }
        if(tmp_position.second-1 >= 0){
            //Destination position
            obj.getPosition(&tmp_position);
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

void pFetV::setBlockWidth(int w){
    m_blockWidth = w;
}

int pFetV::getBlockWidth(void){
    return m_blockWidth;
}

void pFetV::setBlockHeight(int h){
    m_blockHeight = h;
}

int pFetV::getBlockHeight(void){
    return m_blockHeight;
}


QString pFetV::getBlockCode(void){
    return m_blockCode;
}

QString pFetV::getBlockName(void){
    return m_blockName;
}




