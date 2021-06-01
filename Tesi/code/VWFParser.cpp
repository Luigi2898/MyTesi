#include "VWFParser.hpp"
#include "../Grammar/VWFGrammar.hpp"
#include "../../Controllers/SimulationController.hpp"
#include <QDebug>
#include "../../Controllers/AppController.hpp"

VWFParser * VWFParser::sm_Parser = 0;

VWFParser::VWFParser(QObject *parent) :
  QObject(parent)
{
}

VWFParser::~VWFParser()
{
}

VWFParser * VWFParser::instance(void){
  if(sm_Parser==0){
    sm_Parser=new VWFParser();
  }
  return sm_Parser;
}

void VWFParser::deleteInstance(void){
  delete sm_Parser;
}

///\brief Parses the VWF and save the information in an appropriate data structure.\n

bool VWFParser::startParse(const QString filename){
  namespace ascii = boost::spirit::ascii;
  double rise,fall,startTime;
  vector<char> TempFileStream;
  //  string absFilename = dir.absoluteFilePath(filename).toStdString();
  string absFilename = filename.toStdString();
  cout <<absFilename<<endl;
  ifstream in(absFilename.c_str());
  if (!in){
    cout <<"Unable to read file " <<filename.toStdString() <<endl;
    return false;
  }


  in.unsetf(ios::skipws);		// white space skipping is disabled
  std::copy( boost::spirit::basic_istream_iterator<char>(in),
	     boost::spirit::basic_istream_iterator<char>(),
	     std::back_inserter(TempFileStream));
  //   std::cout << "size is " << TempFileStream.size() << "\n";
  in.close();
  vector<char>::const_iterator first = TempFileStream.begin();
  vector<char>::const_iterator last = TempFileStream.end();

  //grammar instantiation.
  VWFGrammar::VWF<vector<char>::const_iterator> g;
  bool result = boost::spirit::qi::phrase_parse(first, last, g, ascii::space);

  // std::cout << result << "\n";
  std::cout << "I am here!" << "\n";
  if (result && (first==last)){
    cout <<"Parsing succeeded\n\n";
    //m_Header->print();
    //cout <<endl;
    rise=m_Header->getRiseTime();
    fall=m_Header->getFallTime();
    //the rise and fall times are written in the objects containing the signals declaration.
    for (unsigned int i=0; i<m_Signals.size();i++){
      m_Signals[i]->updateRiseAndFallTimes(rise,fall);
      //m_Signals[i]->print();
      //cout <<endl;
    }
    startTime=m_Header->getSimulationTime();
    for(unsigned int i=0; i<m_TransitionList.size();i++){
      m_TransitionList[i]->setStartEndTimes(startTime);
      //m_TransitionList[i]->print();
      //cout <<endl;
    }
  }
  else{
    cout <<"Parsing failed\n";
  }

  return result;
}

void VWFParser::setHeader(VWFHeader* h){
  m_Header = h;
}

void VWFParser::pushSignal(VWFSignal* s){
  m_Signals.push_back(s);
}

void VWFParser::pushTransitionList(VWFTransitionList* t){
  m_TransitionList.push_back(t);
}

void VWFParser::setInputsInfoFilename(QString s){
  m_InputsInfoFilename=s;
}

void VWFParser::setSamplesParameters(const QDir wDir,double delta){

  m_wDir = AppController::instance()->getStimuli();
  m_delta = delta;
}

void VWFParser::createSamples(){

  QMap<double,vector<double> > input;
  double time;
  vector<vector<double> > lowerBound, upperBound;
  double currentLowerBound, currentUpperBound;
  vector <double> tmpBounds;
  vector<Transition> incomingTransition;
  vector<VWFLevel*> levels;
  vector<double> amplitude;
  vector<double> inputSignals;

  QString signalnames;
  //QString m_signalnames;
  QString filename;
  QString absFilename;
  QDir currentDir;
  bool condA, condB, condC, timeCond1, timeCond2;
  double tmp=round(m_Header->getDataDuration() * m_Header->getTimeUnit() /m_delta);
  double numRows=1+tmp;

  //numFile is the current file index
  unsigned int numFile;

  //numDir is the current subdirectory index
  unsigned int numDir;

  unsigned int input_num = 0;

  //loop that creates all the samples.
  for(unsigned int i=0; i<m_Signals.size();i++){
    //initializing transitions
    incomingTransition.push_back(NOTRANSITION);
    amplitude.push_back(0);

    if(i< m_TransitionList.size()){
      //initializing upper and lower bounds
      m_TransitionList[i]->getLevels(levels);
      for(unsigned int l=0;l<levels.size();l++)
	tmpBounds.push_back(0);
      upperBound.push_back(tmpBounds);
      lowerBound.push_back(tmpBounds);
      tmpBounds.clear();
    }
    if((m_Signals[i]->getDirection() =="INPUT") && (m_Signals[i]->getSignalType() =="SINGLE_BIT")){
      input_num++;
    }
  }

  timer.start();

  time=0;
  currentLowerBound = currentUpperBound = 0;
  numFile=0;
  numDir=0;
  int j = 0;
  double endSimulation = m_Header->getDataDuration() * m_Header->getTimeUnit();
  while(time < endSimulation){

    //because the delta is very small, even lower than the double error digits.
    if(time > (m_Header->getDataDuration() * m_Header->getTimeUnit()) )
      time = m_Header->getDataDuration() * m_Header->getTimeUnit();
    //populating the input vector
    for (unsigned int i=0;i<m_Signals.size();i++){

      if(i<input_num)
	{
	  //for each signal i get the logic levels and identify the level correspondent to the time step.
	  m_TransitionList[i]->getLevels(levels);
	  for(unsigned int l=0;l<levels.size();l++){
	    //(LEVEL SELECTION) if the time instant belongs to the level time interval
	    timeCond1=(time<=(levels[l]->getEndTime() * m_Header->getTimeUnit()+m_delta/2));
	    timeCond2=(time >= (levels[l]->getStartTime() * m_Header->getTimeUnit()));

	    if( timeCond1 && timeCond2){

	      if( ((time <= (levels[l]->getStartTime() + m_Signals[i]->getRiseTime()/2)*m_Header->getTimeUnit())
		   && (incomingTransition[i]==LOWtoHIGH))
		  || ((time <= (levels[l]->getStartTime() + m_Signals[i]->getFallTime()/2)*m_Header->getTimeUnit())
		      && (incomingTransition[i]==HIGHtoLOW))){
		currentLowerBound=lowerBound[i][l-1];
		currentUpperBound=upperBound[i][l-1];
	      }
	      else{
		currentLowerBound=lowerBound[i][l];
		currentUpperBound=upperBound[i][l];
	      }
	      //if time instant belongs to a level transition
	      condA=(time >= currentLowerBound) && (time <= currentUpperBound);

	      //if the time instant belongs to the currrent level and next transition is 0 to 1
	      condB=((time < ((levels[levels.size()-1]->getEndTime() - m_Signals[i]->getRiseTime()/2)
			      * m_Header->getTimeUnit())) && (incomingTransition[i]==LOWtoHIGH));
	      //if the time instant belongs to the currrent level and next transition is 1 to 0
	      condC=((time < ((levels[levels.size()-1]->getEndTime() - m_Signals[i]->getFallTime()/2)
			      * m_Header->getTimeUnit())) && (incomingTransition[i])==HIGHtoLOW);


	      if( condA  && (condB || condC) ) {

		if(incomingTransition[i]==LOWtoHIGH){
		  //cout <<"transient 0->1\n";
		  amplitude[i]= (double)LOW + ((double)(HIGH-LOW)) * (time-currentLowerBound)/(currentUpperBound-currentLowerBound);
		}
		else{
		  if(incomingTransition[i]==HIGHtoLOW){
		    //cout <<"transiento 1->0\n";
		    amplitude[i]= (double)HIGH + ((double)(LOW-HIGH)) * (time-currentLowerBound)/(currentUpperBound-currentLowerBound);
		  }
		}
	      }
	      else{
		amplitude[i]=(double) levels[l]->getLevel();

		//calculation of the next time bounds when the signal is in steady state.
		if(amplitude[i]==1){
		  //cout <<"steady 1\n";
		  incomingTransition[i]=HIGHtoLOW;
		  lowerBound[i][l]=(levels[l]->getEndTime() - m_Signals[i]->getFallTime()/2) * m_Header->getTimeUnit();
		  upperBound[i][l]=(levels[l]->getEndTime() + m_Signals[i]->getFallTime()/2) * m_Header->getTimeUnit();
		}
		else{
		  if(amplitude[i]==0){
		    //cout <<"steady 0\n";
		    incomingTransition[i]=LOWtoHIGH;
		    lowerBound[i][l]=(levels[l]->getEndTime() - m_Signals[i]->getRiseTime()/2) * m_Header->getTimeUnit();
		    upperBound[i][l]=(levels[l]->getEndTime() + m_Signals[i]->getRiseTime()/2) * m_Header->getTimeUnit();
		  }
		}
	      }

	      inputSignals.push_back(amplitude[i]);
	      break;
	    }
	  }
	}
    }
    Mutex.lock();
    if(readySamples == MAX_THREAD)
      bufferNotFull.wait(&Mutex);        
    simulationBuffer.insert(time,inputSignals);
    ++readySamples;

    bufferNotEmpty.wakeAll();

    inputSignals.clear();

    time+=m_delta;
    j++;
  }

  qDebug() << "Parser index DONE" << j-1;
  qDebug() << "Parser finished";
  qDebug() << "Time:" << time;
  for(unsigned int i=0;i<lowerBound.size();i++){
    lowerBound[i].clear();
    upperBound[i].clear();
  }
  lowerBound.clear();
  upperBound.clear();
  amplitude.clear();
  deleteMembers();
  emit finished();
}

void VWFParser::deleteMembers(void){
  VWFFactory::deleteInstance();
  delete m_Header;
  m_Header = 0;
  for (unsigned int i=0; i<m_Signals.size();i++){
    delete m_Signals[i];
    m_Signals[i] = 0;
  }
  m_Signals.clear();
  for(unsigned int i=0; i<m_TransitionList.size();i++){
    delete m_TransitionList[i];
    m_TransitionList[i]=0;
  }
  m_TransitionList.clear();
}

void VWFParser::getInputsInfoFilename(QString& s){
  s=m_InputsInfoFilename;
}

void VWFParser::saveMap(const vector<double> &time, const vector<vector<double> > &ampli, const char* filename){
  std::ofstream ofs(filename);
  boost::archive::binary_oarchive oa(ofs, ios_base::binary);
  oa << time << ampli;
}

void VWFParser::saveSignalInfo(const string& signalNames, const double& duration, const double& delta,
                               const unsigned int &numRows, const char* filename){
  //    std::cout << "I am here!" << endl;
  std::ofstream ofs(filename);
  boost::archive::xml_oarchive oa(ofs);
  oa << boost::serialization::make_nvp("Names",signalNames)
     << boost::serialization::make_nvp("deltaT",delta)
     << boost::serialization::make_nvp("Duration",duration)
     << boost::serialization::make_nvp("NumRows",numRows);
  //     << boost::serialization::make_nvp("MaxRows",maxRow)
  //     << boost::serialization::make_nvp("MaxFilesInDir",maxFilesInDir);
}

void VWFParser::setSignalInfo(){

  QString signalnames;
  double tmp=round(m_Header->getDataDuration() * m_Header->getTimeUnit() /m_delta);
  double numRows=1+tmp;

  //loop that creates all the samples.
  for(unsigned int i=0; i<m_Signals.size();i++){

    if((m_Signals[i]->getDirection() =="INPUT") && (m_Signals[i]->getSignalType() =="SINGLE_BIT")){
      signalnames = signalnames + QString::fromStdString(m_Signals[i]->getName());

      signalnames=signalnames +" ";
    }
  }

  signalnames.replace(signalnames.lastIndexOf(" "), 1, "");
  // saving the time and file system information in a XML archive
  saveSignalInfo(signalnames.toStdString(),
		 m_Header->getDataDuration() * m_Header->getTimeUnit(),m_delta,
		 numRows, m_InputsInfoFilename.toStdString().c_str());
}
