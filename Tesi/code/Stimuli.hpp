#ifndef STIMULI_HPP
#define STIMULI_HPP

#include <QObject>
#include "./Controllers/AppController.hpp"
#include <QStringList>
#include <QThread>

#include "./Parser/VWFParser.hpp"

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


/// \brief Gives an interface between the simulation engine and the waveforms samples on file system.

class Stimuli : public QObject
{
    Q_OBJECT
public:
    explicit Stimuli(QObject *parent = 0);
    ~Stimuli();
        static Stimuli * instance(void);
        static void deleteInstance(void);
        bool startParse(const QString filename);
        void getInputsInfoFilename(QString& s);
        void setDirectory(QDir d);
        void setInputsInfoFilename(QString s);
        void restoreSignalInfo(void);
        void restoreMap(vector<double> &time, vector<vector<double> > &ampli, const char* filename);
        bool removeStimuliDirectory(QDir* aDir);
        void getSignalNames(QStringList &names);
        void getStimuliFileContent(double fileIndex, double dirIndex, vector<double> &time, vector<vector<double> > &ampli);
        void getDirectoryNumber(double& dirIndex, double& fileIndex);
        void setSamplesParameters(const QDir wDir, double delta);

    private:

        ///Singleton instance of the class.
        static Stimuli *sm_Stimuli;

        ///Directory where the samples are located
        QDir m_StimuliSubdir;

        ///Filename of the XML archive containing timing and file system information
        QString m_InputsInfoFilename;

        ///Duration of the simulation
        double m_Duration;

        ///Simulation time step
        double m_DeltaT;

        ///Overall number of time samples
        unsigned int m_NumRows;

        ///Contains the names of all the signals separated by space
        QStringList m_Inputs;

};

#endif // STIMULI_HPP
