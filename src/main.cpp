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

#include "SunakoSettings.h"
#include "SunakoCli.h"
#include "ChibiWindow.h"

#include <QApplication>
#include <QSettings>
#include <QCommandLineParser>

#include <QMenu>

#include "CarlaUtils.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QString("Sunako"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling, true);

    QApplication app(argc, argv);

    auto *sunakoSettings = new SunakoSettings();
    SunakoCli sunakoCli(sunakoSettings);
    sunakoCli.process(app);
    if (sunakoSettings -> getPluginPath().isEmpty()) {
        sunakoCli.showHelp();
        exit(0);
    }
    sunakoSettings -> printSettings();

    ChibiWindow w(sunakoSettings);

    return app.exec();
}
