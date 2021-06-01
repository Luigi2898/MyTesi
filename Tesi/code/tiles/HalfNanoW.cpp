#include "HalfNanoW.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

HalfNanoW::HalfNanoW()
{
    m_blockCode = "011";
    m_blockName = "HALFNANOW";
}

HalfNanoW::~HalfNanoW()
{
}

void HalfNanoW::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

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
        PowerOn *eventRight = new PowerOn();

        obj.getPosition(&evPos);
        evPos.second++;
        eventRight->setSimDirection(SIMW);
        eventRight->setMatrixIndex(obj.getMatrixIndex());
        eventRight->setPosition(evPos);
        eventRight->setSimValue(obj.getSimValue());
        globalQueue.push(eventRight);

        if(tmp_position.first+1 < m[obj.getMatrixIndex()].size1()){
            // Update row index
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
    else if(obj.getSimDirection() == SIMS){
        // Update wires values, horizontal and vertical
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());

        pair<int, int> evPos;
        PowerOn *eventRight = new PowerOn();

        obj.getPosition(&evPos);
        evPos.second++;
        eventRight->setSimDirection(SIMW);
        eventRight->setMatrixIndex(obj.getMatrixIndex());
        eventRight->setPosition(evPos);
        eventRight->setSimValue(obj.getSimValue());
        globalQueue.push(eventRight);

        if(tmp_position.first-1 >= 0){
            // Update row index
            tmp_position.first--;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }else{
        if(obj.getAR() == false){
            // Send back the event that come from west
            obj.setSimDirection(SIMW);
            obj.setSimValue((m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getValueV()));
            tmp_position.second++;
            obj.setPosition(tmp_position);
            obj.setAR(true);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }
    }
}

void HalfNanoW::setBlockWidth(int w){
    m_blockWidth = w;
}

int HalfNanoW::getBlockWidth(void){
    return m_blockWidth;
}

void HalfNanoW::setBlockHeight(int h){
    m_blockHeight = h;
}

int HalfNanoW::getBlockHeight(void){
    return m_blockHeight;
}


QString HalfNanoW::getBlockCode(void){
    return m_blockCode;
}

QString HalfNanoW::getBlockName(void){
    return m_blockName;
}





