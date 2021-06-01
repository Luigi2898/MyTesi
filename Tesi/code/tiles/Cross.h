#ifndef CROSS_H
#define CROSS_H
#include <Tiles.h>

class Cross : public Tiles
{
public:

    Cross(){
        m_parent = 0;
    }
    virtual ~Cross();

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
    Cross* m_parent;

};

#endif // CROSS_H
