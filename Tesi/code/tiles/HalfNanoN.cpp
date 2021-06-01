#include "HalfNanoN.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

HalfNanoN::HalfNanoN()
{
    m_blockCode = "000";
    m_blockName = "HALFNANON";
}

HalfNanoN::~HalfNanoN()
{
}

void HalfNanoN::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

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
        PowerOn *eventSouth = new PowerOn();

        obj.getPosition(&evPos);
        evPos.first--;
        eventSouth->setSimDirection(SIMN);
        eventSouth->setMatrixIndex(obj.getMatrixIndex());
        eventSouth->setPosition(evPos);
        eventSouth->setSimValue(obj.getSimValue());
        globalQueue.push(eventSouth);

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
    else if(obj.getSimDirection() == SIMW){
        // Update wires values, horizontal and vertical
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());

        pair<int, int> evPos;
        PowerOn *eventSouth = new PowerOn();

        obj.getPosition(&evPos);
        evPos.first--;
        eventSouth->setSimDirection(SIMN);
        eventSouth->setMatrixIndex(obj.getMatrixIndex());
        eventSouth->setPosition(evPos);
        eventSouth->setSimValue(obj.getSimValue());
        globalQueue.push(eventSouth);

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
        // Send back the event that come from west
        if(obj.getAR() == false){
            obj.setSimDirection(SIMN);
            obj.setSimValue((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getValueV()));
            tmp_position.first++;
            obj.setPosition(tmp_position);
            obj.setAR(true);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }
    }
}

void HalfNanoN::setBlockWidth(int w){
    m_blockWidth = w;
}

int HalfNanoN::getBlockWidth(void){
    return m_blockWidth;
}

void HalfNanoN::setBlockHeight(int h){
    m_blockHeight = h;
}

int HalfNanoN::getBlockHeight(void){
    return m_blockHeight;
}


QString HalfNanoN::getBlockCode(void){
    return m_blockCode;
}

QString HalfNanoN::getBlockName(void){
    return m_blockName;
}




