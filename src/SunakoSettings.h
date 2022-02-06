//
// Created by ft on 05.02.22.
//

#ifndef CHIBI_SUNAKOSETTINGS_H
#define CHIBI_SUNAKOSETTINGS_H

#include <QSettings>
#include <QString>
#include <QTemporaryFile>
#include "CarlaUtils.h"
#include "carla/CarlaUtils.hpp"

class SunakoSettings {
private:
    QTemporaryFile tmpFile;
    QSettings *settings;
public:
    SunakoSettings();
    ~SunakoSettings();
    void printSettings();
    void setMinimizeToTray(bool value);
    bool getMinimizeToTray();
    void setName(QString value);
    void setIcon(QString value);
    QString getName();
    QString getIcon();
    void setPluginPath(QString value);
    QString getPluginPath();
    CarlaBackend::BinaryType getBinaryType();
    CarlaBackend::PluginType getPluginType();
};


#endif //CHIBI_SUNAKOSETTINGS_H
