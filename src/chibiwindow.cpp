/*
 * Chibi - Carla's mini-host plugin loader
 * Copyright (C) 2020 Filipe Coelho <falktx@falktx.com>
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
#include "ui_chibiwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QObject>
#include <QApplication>
#include <QStyle>
#include "src/carla/CarlaUtils.hpp"

ChibiWindow::ChibiWindow(BinaryType btype,
                         PluginType ptype,
                         const QString& filename,
                         const QString& name,
                         const QString& label,
                         int64_t uniqueId)
    : QMainWindow(nullptr)
    , ui(new Ui::ChibiWindow)
    , handle(carla_standalone_host_init())
    , idleTimer(startTimer(30))
{
    ui->setupUi(this);

    const QString properName = QString(name);
    this->setWindowTitle(properName);

    // Tray behaviour
    trayIcon = new QSystemTrayIcon(this->windowIcon(),this);
    trayMenu = new QMenu(this);
    tray_Action_Quit = new QAction(QString("Quit"), this);
    tray_Action_Restore = new QAction(QString("Show"), this);
    tray_Action_Mute = new QAction(QString("Mute"), this);
    tray_Action_Mute -> setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
    tray_Action_Bypass = new QAction(QString("Bypass"), this);
    tray_Action_Bypass -> setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
    trayIcon -> setVisible(true);
    trayMenu -> addAction(tray_Action_Bypass);
    trayMenu -> addAction(tray_Action_Mute);
    trayMenu -> addAction(tray_Action_Restore);
    trayMenu -> addAction(tray_Action_Quit);
    trayIcon -> setContextMenu(trayMenu);
    connect(tray_Action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(tray_Action_Restore, SIGNAL(triggered()), this, SLOT(restore()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(restore()));
    connect(tray_Action_Mute, SIGNAL(triggered()), this, SLOT(actionMute()));
    connect(tray_Action_Bypass, SIGNAL(triggered()), this, SLOT(actionBypass()));

    carla_set_engine_option(handle, CarlaBackend::ENGINE_OPTION_OSC_ENABLED, 0, nullptr);

    {
        const QPalette pal(palette());

        const QColor& bgColor = pal.window().color();
        const QColor& fgColor = pal.text().color();

        // qt's rgba is actually argb, so convert that
        QRgb bgColorValue = bgColor.rgba();
        QRgb fgColorValue = fgColor.rgba();

        bgColorValue = ((bgColorValue & 0xffffff) << 8) | (bgColorValue >> 24);
        fgColorValue = ((fgColorValue & 0xffffff) << 8) | (fgColorValue >> 24);

        carla_set_engine_option(handle, CarlaBackend::ENGINE_OPTION_FRONTEND_BACKGROUND_COLOR, bgColorValue, nullptr);
        carla_set_engine_option(handle, CarlaBackend::ENGINE_OPTION_FRONTEND_FOREGROUND_COLOR, fgColorValue, nullptr);
    }

    carla_set_engine_option(handle, CarlaBackend::ENGINE_OPTION_FRONTEND_UI_SCALE, static_cast<int>(devicePixelRatioF() * 1000), "");

    carla_set_engine_callback(handle, _engine_callback, this);
    carla_set_file_callback(handle, _file_callback, this);

    // TODO set frontendWinId in case we cannot embed

    carla_set_engine_option(handle, CarlaBackend::ENGINE_OPTION_PREFER_UI_BRIDGES, 0, nullptr);

    // NOTE: this assumes JACK driver for now
    CARLA_SAFE_ASSERT_RETURN(carla_engine_init(handle, "JACK", "Chibi"),);

    if (! carla_add_plugin(handle, btype, ptype, 
                           filename.toUtf8(), properName.toUtf8(), label.toUtf8(),
                           uniqueId, nullptr, 0x0))
    {
        carla_stderr2("Failed to add plugin, error was: %s", carla_get_last_error(handle));
        return;
    }


    void* const ptr = carla_embed_custom_ui(handle, 0, (void*)(intptr_t)ui->embedwidget->winId());
    ui->embedwidget->setup(ptr);

    if (ui->embedwidget->wasResized())
        resize(ui->embedwidget->size());
    else
        adjustSize();
}

ChibiWindow::~ChibiWindow()
{
    if (carla_is_engine_running(handle))
        carla_engine_close(handle);

    delete ui;
}

void ChibiWindow::closeEvent(QCloseEvent* const event)
{
    if (trayIcon->isVisible() && this->isVisible()) {
        hide();
        event->ignore();
        return;
    }


    killTimer(idleTimer);

    if (carla_is_engine_running(handle))
        carla_set_engine_about_to_close(handle);

    this->trayIcon->setVisible(false);

    QMainWindow::closeEvent(event);
}

void ChibiWindow::timerEvent(QTimerEvent* const event)
{
    if (carla_is_engine_running(handle))
        carla_engine_idle(handle);

    QMainWindow::timerEvent(event);
}

void ChibiWindow::engineCallback(const EngineCallbackOpcode action, const uint pluginId,
                                 const int value1, const int value2, const int value3, const float valuef, const char* const valueStr)
{
    switch (action)
    {
    case CarlaBackend::ENGINE_CALLBACK_IDLE:
        qApp->processEvents();
        break;
    case CarlaBackend::ENGINE_CALLBACK_INFO:
        QMessageBox::information(this, "Information", valueStr);
        break;
    case CarlaBackend::ENGINE_CALLBACK_ERROR:
        QMessageBox::critical(this, "Error", valueStr);
        break;
    case CarlaBackend::ENGINE_CALLBACK_EMBED_UI_RESIZED:
        carla_stdout("resized to %i %i", value1, value2);
        ui->embedwidget->setFixedSize(value1, value2);
        adjustSize();
        setFixedSize(width(), height());
        break;
    default:
        break;
    }
}

const char* ChibiWindow::fileCallback(const FileCallbackOpcode action, const bool isDir, const char* const title, const char* const filter)
{
    QString ret;
    const QFileDialog::Option option = static_cast<QFileDialog::Option>(isDir ? QFileDialog::ShowDirsOnly : 0x0);

    switch (action)
    {
    case CarlaBackend::FILE_CALLBACK_OPEN:
        ret = QFileDialog::getOpenFileName(this, title, "", filter, nullptr, option);
        break;
    case CarlaBackend::FILE_CALLBACK_SAVE:
        ret = QFileDialog::getSaveFileName(this, title, "", filter, nullptr, option);
        break;
    default:
        return nullptr;
    }

    if (ret.isEmpty())
        return nullptr;

    static QByteArray sRet = ret.toUtf8();
    return sRet;
}

void ChibiWindow::actionMute() {
    if (isMute) {
        carla_set_volume(handle, 0, 1);
        isMute = false;
        tray_Action_Mute -> setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
    } else {
        carla_set_volume(handle, 0, 0);
        isMute = true;
        tray_Action_Mute -> setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    }
}
void ChibiWindow::actionBypass() {
    if (isBypass) {
        carla_set_drywet(handle, 0, 1);
        isBypass = false;
        tray_Action_Bypass -> setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton));
    } else {
        carla_set_drywet(handle, 0, 0);
        isBypass = true;
        tray_Action_Bypass -> setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    }
}

void ChibiWindow::restore() {
    this->show();
    this->setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    this->raise();  // for MacOS
    this->activateWindow(); // for Windows
}