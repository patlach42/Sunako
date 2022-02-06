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

#ifndef CHIBIWINDOW_H
#define CHIBIWINDOW_H

#include "jack/jack.h"
#include <QMainWindow>
#include <QSystemTrayIcon>

#include "CarlaHost.h"
#include "CarlaEngine.hpp"
#include <SunakoSettings.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ChibiWindow; }
QT_END_NAMESPACE

class ChibiWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChibiWindow(SunakoSettings* sunakoSettings);
    ~ChibiWindow();

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *tray_Action_Bypass;
    QAction *tray_Action_Mute;
    QAction *tray_Action_Restore;
    QAction *tray_Action_Quit;
    bool isBypass = false;
    bool isMute = false;

protected:
    void closeEvent(QCloseEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

private slots:
    void actionMute();
    void actionBypass();
    void restore();

private:
    Ui::ChibiWindow* const ui;
    const CarlaHostHandle handle;
    const int idleTimer;

    void engineCallback(EngineCallbackOpcode action, uint pluginId, int value1, int value2, int value3, float valuef, const char* valueStr);
    const char* fileCallback(FileCallbackOpcode action, bool isDir, const char* title, const char* filter);

    static void _engine_callback(void* const ptr,
                                 const EngineCallbackOpcode action, const uint pluginId,
                                 const int value1, const int value2, const int value3, const float valuef, const char* const valueStr)
    {
        if (ChibiWindow* const self = static_cast<ChibiWindow*>(ptr))
            self->engineCallback(action, pluginId, value1, value2, value3, valuef, valueStr);
    }

    static const char* _file_callback(void* const ptr,
                                      const FileCallbackOpcode action, const bool isDir, const char* const title, const char* const filter)
    {
        if (ChibiWindow* const self = static_cast<ChibiWindow*>(ptr))
            return self->fileCallback(action, isDir, title, filter);
        return nullptr;
    }
};

#endif // CHIBIWINDOW_H
