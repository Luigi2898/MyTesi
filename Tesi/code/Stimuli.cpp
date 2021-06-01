#include "Stimuli.hpp"
#include <QDebug>

Stimuli * Stimuli::sm_Stimuli = 0;

Stimuli::Stimuli(QObject *parent) :
  QObject(parent)
{
}

Stimuli::~Stimuli()
{
  VWFParser::deleteInstance();
}

Stimuli * Stimuli::instance(void){
  if(sm_Stimuli==0)
    sm_Stimuli= new Stimuli();
  return sm_Stimuli;
}

void Stimuli::deleteInstance(void){
  delete sm_Stimuli;
  sm_Stimuli=0;
}

void Stimuli::setDirectory(QDir d){
  m_StimuliSubdir=d;
}


/// \brief Removes the stimuli directory and its content.\n
/// Before parsing and creating time samples, the old samples must be deleted from the disk.
bool Stimuli::removeStimuliDirectory(QDir* aDir)
{
  bool has_err = false;
  int count=0;
  if (aDir->exists())
    {
      QDir next;
      QFileInfoList entries = aDir->entryInfoList(QDir::NoDotAndDotDot |
						  QDir::Dirs | QDir::Files);
      count = entries.size();
      for (int idx = 0; ((idx < count) && (!has_err)); idx++)
	{
	  QFileInfo entryInfo = entries[idx];
	  QString path = entryInfo.absoluteFilePath();
	  if (entryInfo.isDir())
	    {
	      next.setPath(path);
	      has_err = this->removeStimuliDirectory(&next);
	    }
	  else
	    {
	      QFile file(path);
	      if (!file.remove())
		has_err = true;
	    }
	}
      if (!aDir->rmdir(aDir->absolutePath())){
	has_err = true;
      }
    }
  if(count > 0)
    return(has_err);
  else
    return (false);
}

/// \brief Loads the timing and file system information from XML archive.
void Stimuli::restoreSignalInfo(void){
  string signalNames;
  // why this path
  QString myString=AppController::instance()->getStimuli().absoluteFilePath(m_InputsInfoFilename);
  //QString myString=m_StimuliSubdir.absoluteFilePath(m_InputsInfoFilename);
  std::cout << myString.toStdString() <<endl;
  std::ifstream ifs(myString.toStdString().c_str());
  boost::archive::xml_iarchive ia(ifs);
  ia >> boost::serialization::make_nvp("Names",signalNames)
     >> boost::serialization::make_nvp("deltaT",m_DeltaT)
     >> boost::serialization::make_nvp("Duration",m_Duration)
     >> boost::serialization::make_nvp("NumRows",m_NumRows);

  myString=QString::fromStdString(signalNames);
  m_Inputs = myString.split(" ");
}

/// \brief Instantiates the parser and starts parsing.
bool Stimuli::startParse(const QString filename){
  VWFParser *p= VWFParser::instance();
  bool r = p->startParse(filename);
  return r;
}

void Stimuli::getInputsInfoFilename(QString& s){
  VWFParser *p= VWFParser::instance();
  p->getInputsInfoFilename(s);
}

void Stimuli::setInputsInfoFilename(QString s){
  m_InputsInfoFilename=s;
  VWFParser *p= VWFParser::instance();
  p->setInputsInfoFilename(s);
}

/// \brief Calculates the subdir index, given the file index,
void Stimuli::getDirectoryNumber(double& dirIndex, double& fileIndex){
  // dirIndex=fileIndex / maxFilesPerDir;
}



/// \brief Deserializes the archive given the file index and the directory index.
void Stimuli::getStimuliFileContent(double fileIndex, double dirIndex, vector<double> &time, vector<vector<double> > &ampli){
  m_StimuliSubdir.cd("subdir"+QString::number((int)dirIndex,10));
  QString absFilename=m_StimuliSubdir.absoluteFilePath("waveforms"+QString::number((long int)fileIndex,10)+".iwf");
  restoreMap(time,ampli,absFilename.toStdString().c_str());
}


void Stimuli::restoreMap(vector<double> &time, vector<vector<double> > &ampli, const char* filename){
  std::ifstream ifs(filename, ios_base::binary);
  boost::archive::binary_iarchive ia(ifs);
  ia >> time >> ampli;
}

//should be setSignalNames
void Stimuli::getSignalNames(QStringList &names){
  names=m_Inputs;
}

void Stimuli::setSamplesParameters(const QDir wDir, double delta){
  VWFParser *p= VWFParser::instance();
  p->setSamplesParameters(wDir,delta);
}

