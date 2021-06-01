#ifndef MICROMICRO_H
#define MICROMICRO_H

#include <Cross.h>

class MicroMicro : public Cross
{
public:
    MicroMicro();
    ~MicroMicro();

     void setBlockWidth(int w);
     int getBlockWidth(void);
     void setBlockHeight(int h);
     int getBlockHeight(void);
     void setBlockCode(int c);
     QString getBlockCode(void);
     QString getBlockName(void);
     void acceptNasicEvent(EventNasic &obj, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m);

     bool handleEvent(NasicPrimaryInputH *e) {;}
     bool handleEvent(NasicPrimaryInputV *e) {;}
     bool handleEvent(NasicHorizontalEvent *e) {;}
     bool handleEvent(NasicVerticalEvent *e){;}
     bool handleEvent(PowerOn *e) {;}

     bool generateEvent(NasicPrimaryInputH *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {;}
     bool generateEvent(NasicPrimaryInputV *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {;}
     bool generateEvent(NasicHorizontalEvent *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {;}
     bool generateEvent(NasicVerticalEvent *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {;}
     bool generateEvent(PowerOn *e, vector<boost::numeric::ublas::matrix<class NasicSimulationMatrixInfo*> > &m) {;}
};

#endif // MICROMICRO_H
