#ifndef APPCONTROLLER_HPP
#define APPCONTROLLER_HPP

#include <QString>
#include <QDir>
#include <QSettings>


/// \brief Gives an interface between the file-system and the rest of the application.\n
/// Is a sort of manager able to store the paths of workdirectory and of other folders necessary
/// to the simulator. It defines the methods set and get for each path we need to store. \n
/// It is defined has a singleton class.
class AppController
{
public:

    AppController(QString wDir);
    ~AppController();

    static AppController *instance(void);
    static void deleteInstance(void);

    QString getWorkspacePath();
    QDir getWorkspace();
    void setWorkspace(QString workDir);

    QString getStimuliPath();
    QDir getStimuli();
    void setStimuli(QString StimuliDir);

    QString getVWFPath();
    QDir getVWF();
    void setVWF(QString VWFDir);

    void setTechTarget(int target);
    int  getTechTarget();

    void setTechNode(QString node);

    bool appInit();

    void writeSettings();
    bool readSettings();


private:

    ///Singleton instance of the class.
    static AppController *sm_AppController;

    ///String where the path related to the workdir is stored
    QString m_WorkspacePath;

    ///Directory where the path related to the workdir is stored
    QDir m_Workspace;

    ///String where the path related to the stimuli directory is stored
    QString m_StimuliPath;

    ///Directory where the path related to the stimuli directory is stored
    QDir m_Stimuli;

    ///String where the path related to the vector waveform file is stored
    QString m_VWFPath;

    ///Directory where the path related to the vector waveform file is stored
    QDir m_VWF;

    ///Stores the technology choosen by the user
    int m_TechTarget;

    ///Stores the technology node choosen by the user
    QString m_TechNode;

    ///Flag used to decide if the wizard must be executed or not
    bool wizardFlag;
};

#endif // APPCONTROLLER_HPP
