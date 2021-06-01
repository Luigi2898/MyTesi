#include "nFetH.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

nFetH::nFetH()
{
    m_blockCode = "0";
    m_blockName = "NFETH";
}

bool nFetH::handleEvent(NasicHorizontalEvent *e)
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

nFetH::~nFetH()
{
}

void nFetH::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){


    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getSimDirection() == SIMW){
        if(tmp_position.second+1 < m[obj.getMatrixIndex()].size2()){
            if((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState())==1){
                //Destination position
                tmp_position.second++;
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
    else if(obj.getSimDirection()==SIME){

        if(tmp_position.second-1 >= 0){
            if((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState())==1){
                //Destination position
                obj.getPosition(&tmp_position);
                tmp_position.second--;
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
    else if(obj.getSimDirection() == SIMN){
        // If true update FET state
        if(obj.getAR()){
            qDebug() << "aggiorno lo stato";
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        // Se arrivo da NORD e accendo il transistor creo gli eventi orizzontali
        if (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState() == 1){
            pair<int, int> evPos;
            NasicHorizontalEvent *newPos = new NasicHorizontalEvent();
            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);


            if(tmp_position.second+1 < m[obj.getMatrixIndex()].size2()){
                NasicHorizontalEvent *eventRight = new NasicHorizontalEvent();
                obj.getPosition(&evPos);
                evPos.second++;
                eventRight->setSimDirection(SIMW);
                eventRight->setMatrixIndex(obj.getMatrixIndex());
                eventRight->setPosition(evPos);
                eventRight->setStartingPosition(tmp_position);
                globalQueue.push(eventRight);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicHorizontalEvent *eventRight = new NasicHorizontalEvent();
                    eventRight->setSimDirection(SIMW);
                    eventRight->setMatrixIndex((*newPos).getMatrixIndex());
                    eventRight->setPosition(evPos);
                    eventRight->setStartingPosition(tmp_position);
                    globalQueue.push(eventRight);

                }
            }

            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);

            if(tmp_position.second-1 >= 0){
                NasicHorizontalEvent *eventLeft = new NasicHorizontalEvent();
                obj.getPosition(&evPos);
                evPos.second--;
                eventLeft->setSimDirection(SIME);
                eventLeft->setMatrixIndex(obj.getMatrixIndex());
                eventLeft->setPosition(evPos);
                eventLeft->setStartingPosition(tmp_position);
                globalQueue.push(eventLeft);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicHorizontalEvent *eventLeft = new NasicHorizontalEvent();
                    eventLeft->setSimDirection(SIME);
                    eventLeft->setMatrixIndex((*newPos).getMatrixIndex());
                    eventLeft->setPosition(evPos);
                    eventLeft->setStartingPosition(tmp_position);
                    globalQueue.push(eventLeft);

                }
            }
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
        // If true update FET state
        if(obj.getAR()){
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue()));
        }
        // Se arrivo da SUD e accendo il transistor creo gli eventi orizzontali
        if (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTileState() == 1){
            pair<int, int> evPos;
            NasicHorizontalEvent *newPos = new NasicHorizontalEvent();
            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);


            if(tmp_position.second+1 < m[obj.getMatrixIndex()].size2()){
                NasicHorizontalEvent *eventRight = new NasicHorizontalEvent();
                obj.getPosition(&evPos);
                evPos.second++;
                eventRight->setSimDirection(SIMW);
                eventRight->setMatrixIndex(obj.getMatrixIndex());
                eventRight->setPosition(evPos);
                // Impostazione della posizione di terminazione
                eventRight->setStartingPosition(tmp_position);
                globalQueue.push(eventRight);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicHorizontalEvent *eventRight = new NasicHorizontalEvent();
                    eventRight->setSimDirection(SIMW);
                    eventRight->setMatrixIndex((*newPos).getMatrixIndex());
                    eventRight->setPosition(evPos);
                    eventRight->setStartingPosition(tmp_position);
                    globalQueue.push(eventRight);

                }
            }

            newPos->setMatrixIndex(obj.getMatrixIndex());
            newPos->setSimDirection(obj.getSimDirection());
            obj.getPosition(&tmp_position);
            newPos->setPosition(tmp_position);

            if(tmp_position.second-1 >= 0){
                NasicHorizontalEvent *eventLeft = new NasicHorizontalEvent();
                obj.getPosition(&evPos);
                evPos.second--;
                eventLeft->setSimDirection(SIME);
                eventLeft->setMatrixIndex(obj.getMatrixIndex());
                eventLeft->setPosition(evPos);
                // Impostazione della posizione di terminazione
                eventLeft->setStartingPosition(tmp_position);
                globalQueue.push(eventLeft);
            }else{
                // find interconnection or output
                if(obj.findOutputOrInterConnect(*newPos)){
                    (*newPos).getPosition(&evPos);
                    NasicHorizontalEvent *eventLeft = new NasicHorizontalEvent();
                    eventLeft->setSimDirection(SIME);
                    eventLeft->setMatrixIndex((*newPos).getMatrixIndex());
                    eventLeft->setPosition(evPos);
                    eventLeft->setStartingPosition(tmp_position);
                    globalQueue.push(eventLeft);

                }
            }
        }
        if(tmp_position.first-1 >= 0){
            //Destination position
            obj.getPosition(&tmp_position);
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

    /*if(!obj.checkPosition(this)){
        qDebug()<<"-------> L'evento e' tornato al punto di partenza";
        return;
    }*/


}

void nFetH::setBlockWidth(int w){
    m_blockWidth = w;
}

int nFetH::getBlockWidth(void){
    return m_blockWidth;
}

void nFetH::setBlockHeight(int h){
    m_blockHeight = h;
}

int nFetH::getBlockHeight(void){
    return m_blockHeight;
}


QString nFetH::getBlockCode(void){
    return m_blockCode;
}

QString nFetH::getBlockName(void){
    return m_blockName;
}



