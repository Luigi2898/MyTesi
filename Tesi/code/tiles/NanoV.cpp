#include "NanoV.h"

NanoV::NanoV()
{
    m_blockCode = "1001";
    m_blockName = "NANOV";
}

NanoV::~NanoV()
{
}

void NanoV::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getAR()){
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());
    }

    if(obj.getSimDirection() == SIMN){
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
}

void NanoV::setBlockWidth(int w){
    m_blockWidth = w;
}

int NanoV::getBlockWidth(void){
    return m_blockWidth;
}

void NanoV::setBlockHeight(int h){
    m_blockHeight = h;
}

int NanoV::getBlockHeight(void){
    return m_blockHeight;
}


QString NanoV::getBlockCode(void){
    return m_blockCode;
}

QString NanoV::getBlockName(void){
    return m_blockName;
}






