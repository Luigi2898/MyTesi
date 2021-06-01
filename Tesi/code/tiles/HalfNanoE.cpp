#include "HalfNanoE.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

HalfNanoE::HalfNanoE()
{
    m_blockCode = "010";
    m_blockName = "HALFNANOE";
}

HalfNanoE::~HalfNanoE()
{
}

void HalfNanoE::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getSimDirection() == SIMN){
        // Update wires values, horizontal and vertical
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());

        pair<int, int> evPos;
        PowerOn *eventLeft = new PowerOn();

        obj.getPosition(&evPos);
        evPos.second--;
        eventLeft->setSimDirection(SIME);
        eventLeft->setMatrixIndex(obj.getMatrixIndex());
        eventLeft->setPosition(evPos);
        eventLeft->setSimValue(obj.getSimValue());
        globalQueue.push(eventLeft);

        if(tmp_position.first+1 < m[obj.getMatrixIndex()].size1()){
            // Update row index
            tmp_position.first++;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }
        else{
            // find interconnection or output
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }else if(obj.getSimDirection() == SIMS){
        // Update wires values, horizontal and vertical
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());

        pair<int, int> evPos;
        PowerOn *eventLeft = new PowerOn();

        obj.getPosition(&evPos);
        evPos.second--;
        eventLeft->setSimDirection(SIME);
        eventLeft->setMatrixIndex(obj.getMatrixIndex());
        eventLeft->setPosition(evPos);
        eventLeft->setSimValue(obj.getSimValue());
        globalQueue.push(eventLeft);

        if(tmp_position.first-1 >= 0){
            // Update row index
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
    }else{
        qDebug()<<"Trovata una T";
        // Send back the event that come from west
        if(obj.getAR() == false){
            obj.setSimDirection(SIME);
            obj.setSimValue((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getValueV()));
            tmp_position.second--;
            obj.setPosition(tmp_position);
            obj.setAR(true);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }
    }
}

void HalfNanoE::setBlockWidth(int w){
    m_blockWidth = w;
}

int HalfNanoE::getBlockWidth(void){
    return m_blockWidth;
}

void HalfNanoE::setBlockHeight(int h){
    m_blockHeight = h;
}

int HalfNanoE::getBlockHeight(void){
    return m_blockHeight;
}


QString HalfNanoE::getBlockCode(void){
    return m_blockCode;
}

QString HalfNanoE::getBlockName(void){
    return m_blockName;
}





