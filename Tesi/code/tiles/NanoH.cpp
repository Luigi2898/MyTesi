#include "NanoH.h"

NanoH::NanoH()
{
    m_blockCode = "1011";
    m_blockName = "NANOH";
}

NanoH::~NanoH()
{
}

void NanoH::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

    pair<int, int> tmp_position;
    obj.getPosition(&tmp_position);

    if(obj.getAR()){
        (m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setValueH(obj.getSimValue()));
        m[obj.getMatrixIndex()](tmp_position.first, tmp_position.second)->setSubTileState(obj.getSimValue());
    }

    if(obj.getSimDirection() == SIMW){
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

void NanoH::setBlockWidth(int w){
    m_blockWidth = w;
}

int NanoH::getBlockWidth(void){
    return m_blockWidth;
}

void NanoH::setBlockHeight(int h){
    m_blockHeight = h;
}

int NanoH::getBlockHeight(void){
    return m_blockHeight;
}


QString NanoH::getBlockCode(void){
    return m_blockCode;
}

QString NanoH::getBlockName(void){
    return m_blockName;
}






