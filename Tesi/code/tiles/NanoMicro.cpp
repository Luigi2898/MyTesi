#include "NanoMicro.h"

NanoMicro::NanoMicro()
{
    m_blockCode = "101";
    m_blockName = "NANOMICRO";
}

NanoMicro::~NanoMicro()
{
}

void NanoMicro::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getSimDirection()==SIMN){
        if(obj.getAR()){
            m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue());
        }
        if(tmp_position.first+1 < m[obj.getMatrixIndex()].size1()){
            //Destination position
            tmp_position.first++;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    else if(obj.getSimDirection()==SIMS){
        if(obj.getAR()){
            m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueV(obj.getSimValue());
        }
        if(tmp_position.first-1 >= 0){
            //Destination position
            tmp_position.first--;
            obj.setPosition(tmp_position);
            (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
        }else{
            if(obj.findOutputOrInterConnect(obj)){
                obj.getPosition(&tmp_position);
                (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->getSubTile())->acceptNasicEvent(obj, m);
            }
        }
    }
    if(obj.getSimDirection()==SIMW){
        if(obj.getAR()){
            m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue());
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
        if(obj.getAR()){
            m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue());
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

void NanoMicro::setBlockWidth(int w){
    m_blockWidth = w;
}

int NanoMicro::getBlockWidth(void){
    return m_blockWidth;
}

void NanoMicro::setBlockHeight(int h){
    m_blockHeight = h;
}

int NanoMicro::getBlockHeight(void){
    return m_blockHeight;
}


QString NanoMicro::getBlockCode(void){
    return m_blockCode;
}

QString NanoMicro::getBlockName(void){
    return m_blockName;
}

