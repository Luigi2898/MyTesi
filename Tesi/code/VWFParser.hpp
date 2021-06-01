#ifndef VWFPARSER_HPP
#define VWFPARSER_HPP

#include <math.h>
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <fstream>
#include <vector>
#include <QMap>
#include <QVector>
#include <QObject>
#include <QThread>


//Boost serialization
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

// boost archive related inclusions

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

// boost serialization related inclusions
#include <boost/serialization/utility.hpp>

//XML serialization archive
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/nvp.hpp>


class VWFHeader;
class VWFSignal;
class VWFLevel;
class VWFTransitionList;

using namespace std;

///VWFParser class has the goal to parse the vector waveform file, to create the waveforms samples and store them.
class VWFParser : public QObject
{
    Q_OBJECT
public:
    explicit VWFParser(QObject *parent = 0);
    ~VWFParser();
    static VWFParser * instance(void);
    static void deleteInstance(void);
    bool startParse(const QString filename);
    void setHeader(VWFHeader*);
    void setInputsInfoFilename(QString s);
    void pushSignal(VWFSignal*);
    void pushTransitionList(VWFTransitionList*);
    //void createWaveformFiles(const QDir &wDir, double delta, int maxRow, int maxFilesInDir);
    void saveMap(const vector<double> &time, const vector<vector<double> > &ampli, const char* filename);
    void saveSignalInfo(const string& signalNames, const double& duration, const double& delta,
                            const unsigned int &numRows, const char* filename);
    void getInputsInfoFilename(QString& s);
    void deleteMembers(void);
    void setSamplesParameters(const QDir wDir,double delta);
    void setSignalInfo();

private:

    ///Singleton instance of the class.
    static VWFParser* sm_Parser;

    ///Contains the header part of the VWF extracted by the parsing process.
    VWFHeader *m_Header;

    ///Contains the signals declarations of the VWF extracted by the parsing process.
    vector<VWFSignal*> m_Signals;

    ///Contains the signals transition lists of the VWF extracted by the parsing process.
    vector<VWFTransitionList*> m_TransitionList;

    ///String containing the name of the VWF to be parsed.
    QString m_InputsInfoFilename;

    QDir m_wDir;
    double m_delta;

signals:

    void finished();

public slots:

    void createSamples();

};

#endif // VWFPARSER_HPP
