//
// Created by ft on 06.02.22.
//

#include "SunakoCli.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QString>
#include <QCoreApplication>


SunakoCli::SunakoCli(SunakoSettings *settings){
    this -> settings = settings;
    parser.setApplicationDescription("Sunako - single vst host.");
    parser.addHelpOption();
    QCommandLineOption iconOption(QString("i"),
                                  QCoreApplication::translate("main", "Use <icon.png> as icon."),
                                  QCoreApplication::translate("main", "icon"));
    QCommandLineOption nameOption(QString("n"),
                                  QCoreApplication::translate("main", "Use <name> as title and client name."),
                                  QCoreApplication::translate("main", "name"));
    QCommandLineOption minimizedOption(QString("m"),
                                       QCoreApplication::translate("main", "Start minimized to tray"));

    parser.addOption(iconOption);
    parser.addOption(nameOption);
    parser.addOption(minimizedOption);
    parser.addPositionalArgument("plugin_path",
                                 QCoreApplication::translate("main", "Path to VST2 .so plugin."));
}

void SunakoCli::process(const QCoreApplication &app) {
    parser.process(app);
    const QStringList args = parser.positionalArguments();
    if (!args.empty() && !args.at(0).isEmpty()) {
        settings->setPluginPath(args.at(0));
    }

    bool minimizedOptionSet = parser.isSet("m");
    settings -> setMinimizeToTray(minimizedOptionSet);
    if (parser.isSet("n")) {
        settings->setName(parser.value("n"));
    } else {
        settings->setName("Title");
    }
    if (parser.isSet("i")) {
        settings->setIcon(parser.value("i"));
    }
}

void SunakoCli::showHelp() {
    parser.showHelp();
}

SunakoCli::~SunakoCli() {}