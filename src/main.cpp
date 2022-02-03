/*
 * Sunako, fork of Chibi - Carla's mini-host plugin loader
 * Copyright (Chibi, Carla) (C) 2020 Filipe Coelho <falktx@falktx.com>
 * Copyright (Sunako) (C) 2022 Maxim Fedotov <f42.dobro@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "chibiwindow.h"

#include <QApplication>
#include <QSettings>
#include <QCommandLineParser>

#include <QMenu>
#include <QSystemTrayIcon>
#include <QIcon>

#include <cstdlib>

#include "CarlaUtils.h"
#include "src/carla/CarlaUtils.hpp"

int main(int argc, char *argv[])
{
    int64_t uniqueId = 1337;

    QCoreApplication::setApplicationName(QString("Sunako"));

    // Disable Qt Scaling size behaviour
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling, true);

    QApplication app(argc, argv);

    QString plugin_path;
    QString name;
    QString label;
    QCommandLineParser parser;
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

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.empty()) {
        parser.showHelp(0);
    }
    plugin_path = args.at(0);

    bool iconOptionSet = parser.isSet(iconOption);
    bool minimizedOptionSet = parser.isSet(minimizedOption);
    bool nameOptionSet = parser.isSet(nameOption);
    if (nameOptionSet) {
        name = parser.value(nameOption);
    } else {
        name = "Title";
    }

    ChibiWindow w(CarlaBackend::BINARY_NATIVE, CarlaBackend::PLUGIN_VST2, plugin_path, name, label, uniqueId);

    if (iconOptionSet) {
        QString iconOptionValue = parser.value(iconOption);
        QIcon icon(iconOptionValue);
        w.trayIcon->setIcon(icon);
        w.setWindowIcon(icon);
    }
    if (!minimizedOptionSet) {
        w.show();
    }

    return app.exec();
}
