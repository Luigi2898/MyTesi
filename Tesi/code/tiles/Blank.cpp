#include "Blank.h"
#include "NasicEventFactory.h"
#include "../../NasicLogicSimulation.hpp"

Blank::Blank()
{
    m_blockCode = "11111";
    m_blockName = "BLANK";
}

Blank::~Blank()
{
}

void Blank::acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class  NasicSimulationMatrixInfo*> > &m){

    if(!obj.visit(this)){
        qDebug() << "sto per uscire";
        return;
    }

}

void Blank::setBlockWidth(int w){
    m_blockWidth = w;
}

int Blank::getBlockWidth(void){
    return m_blockWidth;
}

void Blank::setBlockHeight(int h){
    m_blockHeight = h;
}

int Blank::getBlockHeight(void){
    return m_blockHeight;
}


QString Blank::getBlockCode(void){
    return m_blockCode;
}

QString Blank::getBlockName(void){
    return m_blockName;
}





