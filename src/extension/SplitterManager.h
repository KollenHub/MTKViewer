#include <QSettings>
#include <QStandardPaths>
#include <QApplication>
#include <QDebug>
#include <QSplitter>

class SplitterManager {
public:
    static void SaveSplitterState(QSplitter* splitter, const QString& configName = "splitter_state");
    
    
    static void RestoreSplitterState(QSplitter* splitter, const QString& configName = "splitter_state");
};