#include "MyApp.h"

#include "qvariant.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdir.h"

// 应用程序配置目录
QString MyApp::strAppPath_           = "./";
QString MyApp::strDataPath_          = "";
QString MyApp::strRecvPath_          = "";
QString MyApp::strDataBasePath_      = "";
QString MyApp::strConfigPath_        = "";
QString MyApp::strFacePath_          = "";
QString MyApp::strHeadPath_          = "";
QString MyApp::strSoundPath_         = "";
QString MyApp::strRecordPath_        = "";
QString MyApp::strIniFile_           = "config.ini";

// 服务器相关配置
QString MyApp::strHostAddr_          = "192.168.2.199";
int     MyApp::msgPort_              = 60001;
int     MyApp::filePort_             = 60002;
int     MyApp::groupPort_            = 60003;

QString MyApp::strUserName_          = "zhangsan";
QString MyApp::strPasswd_            = "123456";
QString MyApp::strHeadFile_          = "head-64.png";

int     MyApp::id_                   = -1;
int     MyApp::winX_                 = 0;
int     MyApp::winY_                 = 0;

void MyApp::initApp(const QString& appPath) {
    strAppPath_         = appPath + "/";

    strDataPath_        = strAppPath_ + "Data/";
    strRecvPath_        = strDataPath_ + "RecvFiles/";
    strDataBasePath_    = strDataPath_ + "DataBase/";
    strConfigPath_      = strDataPath_ + "Conf/";
    strHeadPath_        = strDataPath_ + "Head/";
    strSoundPath_       = strDataPath_ + "Sound/";
    strRecordPath_      = strDataPath_ + "Record/";
    strFacePath_        = strDataPath_ + "Face/";
    strIniFile_         = strDataPath_ + "config.ini";

    checkDirs();

    checkSounds();

    createSettingFile();

    readSettingFile();
}

void MyApp::createSettingFile() {
    // 写入配置文件
    QSettings settings(strIniFile_, QSettings::IniFormat);
    QString strGroups = settings.childGroups().join("");
    if (!QFile::exists(strIniFile_) || (strGroups.isEmpty()))
    {
        /*系统设置*/
        settings.beginGroup("UserCfg");
        settings.setValue("User", strUserName_);
        settings.setValue("Passwd", strPasswd_);
        settings.endGroup();

        /*其他配置*/
        settings.beginGroup("Server");
        settings.setValue("HostAddr", strHostAddr_);
        settings.setValue("MsgPort", msgPort_);
        settings.setValue("FilePort", filePort_);
        settings.setValue("GroupPort", groupPort_);
        settings.endGroup();
        settings.sync();
    }
#ifdef Q_WS_QWS
    QProcess::execute("sync");
#endif
}

void MyApp::readSettingFile() {
    QSettings settings(strIniFile_, QSettings::IniFormat);
    settings.beginGroup("UserCfg");
    strUserName_ = settings.value("User", "zhangsan").toString();
    strPasswd_ = settings.value("Passwd", "123456").toString();
    settings.endGroup();

    settings.beginGroup("Server");
    strHostAddr_ = settings.value("HostAddr", "192.168.2.199").toString();
    msgPort_ = settings.value("MsgPort", 32101).toInt();
    filePort_ = settings.value("FilePort", 32102).toInt();
    groupPort_ = settings.value("GroupPort", 32103).toInt();
    settings.endGroup();
}

void MyApp::setSettingFile(const QString& group, const QString& key,
                           const QString& value) {
    QSettings settings(strIniFile_, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.sync();
}

QVariant MyApp::getSettingKeyValue(const QString& group, const QString& key,
                                   const QString& value) {
    QSettings settings(strIniFile_, QSettings::IniFormat);
    settings.beginGroup(group);
    return settings.value(key, value);
}

void MyApp::checkDirs() {
    // 数据文件夹
    QDir dir(strDataPath_);
    if (!dir.exists()) {
        dir.mkdir(strDataPath_);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
    // 接收文件目录
    dir.setPath(strRecvPath_);
    if (!dir.exists()) {
        dir.mkdir(strRecvPath_);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
    // 数据库目录
    dir.setPath(strDataBasePath_);
    if (!dir.exists()) {
        dir.mkdir(strDataBasePath_);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
    // 配置文件目录
    dir.setPath(strConfigPath_);
    if (!dir.exists()) {
        dir.mkdir(strConfigPath_);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
    // 表情目录
    dir.setPath(strFacePath_);
    if (!dir.exists()) {
        dir.mkdir(strFacePath_);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
    // 头像检测目录
    dir.setPath(strHeadPath_);
    if (!dir.exists()) {
        dir.mkdir(strHeadPath_);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
    // 音频目录
    dir.setPath(strSoundPath_);
    if (!dir.exists()) {
        dir.mkdir(strSoundPath_);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
}

void MyApp::checkSounds() {
    if (!QFile::exists(MyApp::strSoundPath_ + "message.wav")) {
        QFile::copy(":/sound/resource/sound/message.wav", MyApp::strSoundPath_ + "message.wav");
}

    if (!QFile::exists(MyApp::strSoundPath_ + "msg.wav")) {
        QFile::copy(":/sound/resource/sound/msg.wav", MyApp::strSoundPath_ + "msg.wav");
    }

    if (!QFile::exists(MyApp::strSoundPath_ + "ringin.wav")) {
        QFile::copy(":/sound/resource/sound/ringin.wav", MyApp::strSoundPath_ + "ringin.wav");
    }

    if (!QFile::exists(MyApp::strSoundPath_ + "system.wav")) {
        QFile::copy(":/sound/resource/sound/system.wav", MyApp::strSoundPath_ + "system.wav");
    }

    if (!QFile::exists(MyApp::strSoundPath_ + "userlogon.wav")) {
        QFile::copy(":/sound/resource/sound/userlogon.wav", MyApp::strSoundPath_ + "userlogon.wav");
    }
}

void MyApp::saveConfig() {
    QSettings settings(strIniFile_, QSettings::IniFormat);

    /*系统设置*/
    settings.beginGroup("UserCfg");
    settings.setValue("User", strUserName_);
    settings.setValue("Passwd", strPasswd_);
    settings.endGroup();

    /*其他配置*/
    settings.beginGroup("Server");
    settings.setValue("HostAddr", strHostAddr_);
    settings.setValue("MsgPort", msgPort_);
    settings.setValue("FilePort", filePort_);
    settings.setValue("GroupPort", groupPort_);
    settings.endGroup();
    settings.sync();
}
