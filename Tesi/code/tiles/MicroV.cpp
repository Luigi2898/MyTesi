#include "MicroV.h"

MicroV::MicroV()
{
    m_blockCode = "0111";
    m_blockName = "MICROV";
}

MicroV::~MicroV()
{
}

void MicroV::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getAR()){
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue());
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
        }
    }
}

void MicroV::setBlockWidth(int w){
    m_blockWidth = w;
}

int MicroV::getBlockWidth(void){
    return m_blockWidth;
}

void MicroV::setBlockHeight(int h){
    m_blockHeight = h;
}

int MicroV::getBlockHeight(void){
    return m_blockHeight;
}


QString MicroV::getBlockCode(void){
    return m_blockCode;
}

QString MicroV::getBlockName(void){
    return m_blockName;
}





