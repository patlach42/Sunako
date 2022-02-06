//
// Created by ft on 06.02.22.
//

#ifndef CHIBI_SUNAKOCLI_H
#define CHIBI_SUNAKOCLI_H

#include <QCommandLineParser>
#include <QSettings>
#include <QApplication>
#include <SunakoSettings.h>

class SunakoCli {
private:
    QCommandLineParser parser;
    SunakoSettings *settings;
public:
    SunakoCli(SunakoSettings *settings);
    ~SunakoCli();
    void process(const QCoreApplication &app);
    void showHelp();
};


#endif //CHIBI_SUNAKOCLI_H
