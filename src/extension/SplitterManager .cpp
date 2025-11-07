#include "SplitterManager .h"

void SplitterManager::SaveSplitterState(QSplitter *splitter, const QString &configName)
{
    // 使用INI文件格式保存到应用程序目录
    QSettings settings(QCoreApplication::applicationDirPath() + "/layout.ini", QSettings::IniFormat);
    settings.setValue(configName, splitter->saveState());

    qDebug() << "Splitter state saved to:" << settings.fileName();
}

void SplitterManager::RestoreSplitterState(QSplitter *splitter, const QString &configName)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/layout.ini", QSettings::IniFormat);
    QVariant state = settings.value(configName);

    if (state.isValid())
    {
        splitter->restoreState(state.toByteArray());
        qDebug() << "Splitter state restored from:" << settings.fileName();
    }
    else
    {
        qDebug() << "No saved splitter state found, using defaults";
    }
}