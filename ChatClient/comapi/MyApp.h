#pragma once

#include <QObject>

class QApplication;

class MyApp {
 public:
  static void initApp(const QString& appPath);

  static void createSettingFile();

  static void readSettingFile();

  static void setSettingFile(const QString& group, const QString& key,
                             const QString& value);

  static QVariant getSettingKeyValue(const QString& group, const QString& key,
                                     const QString& value);

  static void checkDirs();

  static void checkSounds();

  static void saveConfig();

 public:
  static QString strAppPath_;
  static QString strDataPath_;
  static QString strRecvPath_;
  static QString strDataBasePath_;
  static QString strConfigPath_;
  static QString strSoundPath_;
  static QString strRecordPath_;

  static QString strFacePath_;
  static QString strHeadPath_;
  static QString strIniFile_;

  static QString strHostAddr_;
  static int msgPort_;
  static int filePort_;
  static int groupPort_;

  static QString strUserName_;
  static QString strPasswd_;
  static QString strHeadFile_;

  static int id_;
  static int winX_;
  static int winY_;
};
