#ifndef TILES_H
#define TILES_H
#include <QString>
#include "../NasicQueueEvent/EventNasic.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../../NasicVisitors/NasicSimulationMatrixInfo.h"
#include "QDebug"


using namespace std;

class EventNasic;
class NasicPrimaryInputH;
class NasicPrimaryInputV;
class NasicHorizontalEvent;
class NasicVerticalEvent;
class PowerOn;

class Tiles
{
public:
    Tiles(){
        m_parent = 0;
    }
    virtual ~Tiles();

    virtual void setBlockWidth(int w) = 0;
    virtual int getBlockWidth(void){return -1;}
    virtual void setBlockHeight(int h) = 0;
    virtual int getBlockHeight(void){return -1;}
    //virtual void setBlockCode(int c) = 0;
    virtual QString getBlockCode(void){return NULL;}
    virtual QString getBlockName(void){return NULL;}
    virtual void acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &)=0;

    virtual bool handleEvent(NasicPrimaryInputH *e) {return true;}
    virtual bool handleEvent(NasicPrimaryInputV *e) {return true;}
    virtual bool handleEvent(NasicHorizontalEvent *e) {return true;}
    virtual bool handleEvent(NasicVerticalEvent *e) {return true;}
    virtual bool handleEvent(PowerOn *e) {return true;}

        virtual bool generateEvent(NasicPrimaryInputH *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {return true;}
        virtual bool generateEvent(NasicPrimaryInputV *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {return true;}
        virtual bool generateEvent(NasicHorizontalEvent *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {return true;}
        virtual bool generateEvent(NasicVerticalEvent *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {return true;}
        virtual bool generateEvent(PowerOn *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {return true;}

protected:
    Tiles* m_parent;

    int m_blockWidth;
    int m_blockHeight;

    QString m_blockName;
    QString m_blockCode;

};

#endif // TILES_H
