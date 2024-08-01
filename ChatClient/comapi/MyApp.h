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
  static QString strAppPath_;           /// 应用程序路径
  static QString strDataPath_;          /// 数据保存路径
  static QString strRecvPath_;          /// 接收数据保存路径
  static QString strDataBasePath_;      /// 数据库保存路径
  static QString strConfigPath_;        /// 配置文件路径
  static QString strSoundPath_;         /// 音频文件路径
  static QString strRecordPath_;        /// 录音保存路径

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

