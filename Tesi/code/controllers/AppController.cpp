#include "AppController.hpp"
#include "wizard.h"
#include "./Actions.hpp"
//TIMES ARE IN MICROSECONDS
#define DELTAT 100e-6
#define maxRowsPerFile 1000
#define maxFilePerDir 1000
#define MAX_INDEX 10

AppController *AppController::sm_AppController = 0;

AppController::AppController(QString wDir = QDir::homePath())
{
    m_WorkspacePath = wDir;
    wizardFlag = 0;
}

AppController::~AppController()
{
  AppController::deleteInstance();
}

void AppController::deleteInstance(void){
  delete sm_AppController;
  sm_AppController=0;
}

AppController * AppController::instance()
{
    if (!sm_AppController)
      sm_AppController = new AppController;
    return sm_AppController;
}

QString AppController::getWorkspacePath()
{
    return m_WorkspacePath;
}

QDir AppController::getWorkspace()
{
    return m_Workspace;
}

void AppController::setWorkspace(QString workDir)
{
    m_WorkspacePath = workDir;
    m_Workspace.setPath(m_WorkspacePath);
}

QString AppController::getStimuliPath()
{
    return m_StimuliPath;
}

QDir AppController::getStimuli()
{
    return m_Stimuli;
}

void AppController::setStimuli(QString StimuliDir)
{
    m_StimuliPath = StimuliDir;
    m_Stimuli.setPath(m_StimuliPath);
}

QString AppController::getVWFPath()
{
    return m_VWFPath;
}

QDir AppController::getVWF()
{
    return m_VWF;
}

void AppController::setVWF(QString VWFDir)
{
    m_VWFPath = VWFDir;
    m_VWF.setPath(m_VWFPath);
}

void AppController::setTechTarget(int target)
{
    m_TechTarget = target;
}

int AppController::getTechTarget(){

    return m_TechTarget;
}

void AppController::setTechNode(QString node){

    m_TechNode = node;
}

bool AppController::appInit()
{

    int gridWidth = 5;
    int gridHeight = 10;
    int numPhaseBlocksPerRow = 2;
    int numRows = 3;
    int numMagPhase1 = 8;
    int numMagPhase2and3 = 10;
    int numMagInVerticalColFPMetamer = 15;
    int numMagInVerticalBlankMetamer = 8;
    double t1=0;
    double t2=80.5e-3;

    //Initialization!

    Actions *actions = Actions::instance();
    actions->setDeltaT(DELTAT);
    actions->setMaxRowsPerFile(maxRowsPerFile);
    actions->setMaxFilePerDir(maxFilePerDir);
    actions->setSimulationTimes(t1,t2);

    actions->setMQCAFloorplanParameters(gridWidth,gridHeight,numPhaseBlocksPerRow,numRows,numMagPhase1,
                                        numMagPhase2and3,numMagInVerticalColFPMetamer,numMagInVerticalBlankMetamer);

    bool settingsFlag = readSettings();
    if(settingsFlag == false){
        Wizard::instance()->startWizard();
        writeSettings();
    }

    Actions *action = Actions::instance();
    action->setWorkspace();
    action->setNASICFloorplanXmlPath(m_TechNode);
    action->setVectorWaveformFile(m_VWFPath);
    wizardFlag = true;

    return wizardFlag;
}

bool AppController::readSettings()
{

    QSettings m_Settings;
    m_WorkspacePath = m_Settings.value("AppControllerSettings/WorkspacePath").toString();
    m_Workspace.setPath(m_WorkspacePath);
    m_StimuliPath = m_Settings.value("AppControllerSettings/StimuliPath").toString();
    m_Stimuli.setPath(m_StimuliPath);
    m_VWFPath = m_Settings.value("AppControllerSettings/VWFPath").toString();
    m_VWF.setPath(m_VWFPath);

    m_TechTarget = m_Settings.value("TechSettings/TechTarget").toInt();
    m_TechNode = m_Settings.value("TechSettings/TechNode").toString();
    bool flag = m_Settings.value("AppControllerSettings/Stored", false).toBool();
    return flag;
}


void AppController::writeSettings()
{
    QSettings m_Settings;
    m_Settings.beginGroup("AppControllerSettings");
    m_Settings.setValue("WorkspacePath", m_WorkspacePath);
    m_Settings.setValue("StimuliPath", m_StimuliPath);
    m_Settings.setValue("VWFPath", m_VWFPath);
    m_Settings.setValue("Stored", true);
    m_Settings.endGroup();
    m_Settings.beginGroup("TechSettings");
    m_Settings.setValue("TechTarget", m_TechTarget);
    m_Settings.setValue("TechNode", m_TechNode);
    m_Settings.endGroup();
}

