#include "HalfNanoS.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

HalfNanoS::HalfNanoS()
{
    m_blockCode = "001";
    m_blockName = "HALFNANOS";
}

HalfNanoS::~HalfNanoS()
{
}

void HalfNanoS::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getSimDirection() == SIME){
        // Update wires values, horizontal and vertical
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());

        pair<int, int> evPos;
        PowerOn *eventNorth = new PowerOn();

        obj.getPosition(&evPos);
        evPos.first--;
        eventNorth->setSimDirection(SIMS);
        eventNorth->setMatrixIndex(obj.getMatrixIndex());
        eventNorth->setPosition(evPos);
        eventNorth->setSimValue(obj.getSimValue());
        globalQueue.push(eventNorth);

        if(tmp_position.second-1 >= 0){
            tmp_position.second--;
            obj.setPosition(tmp_position);
            // Update col index
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    else if(obj.getSimDirection() == SIMW ){
        // Update wires values, horizontal and vertical
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());

        pair<int, int> evPos;
        PowerOn *eventNorth = new PowerOn();

        obj.getPosition(&evPos);
        evPos.first--;
        eventNorth->setSimDirection(SIMS);
        eventNorth->setMatrixIndex(obj.getMatrixIndex());
        eventNorth->setPosition(evPos);
        eventNorth->setSimValue(obj.getSimValue());
        globalQueue.push(eventNorth);

        if(tmp_position.second+1 < m[obj.getMatrixIndex()].size2()){
            tmp_position.second++;
            obj.setPosition(tmp_position);
            // Update col index
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }else{
        if(obj.getAR() == false){
            // Send back the event that come from north
            obj.setSimDirection(SIMS);
            obj.setSimValue((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getValueV()));
            tmp_position.first--;
            obj.setPosition(tmp_position);
            obj.setAR(true);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }
    }
}

void HalfNanoS::setBlockWidth(int w){
    m_blockWidth = w;
}

int HalfNanoS::getBlockWidth(void){
    return m_blockWidth;
}

void HalfNanoS::setBlockHeight(int h){
    m_blockHeight = h;
}

int HalfNanoS::getBlockHeight(void){
    return m_blockHeight;
}


QString HalfNanoS::getBlockCode(void){
    return m_blockCode;
}

QString HalfNanoS::getBlockName(void){
    return m_blockName;
}





