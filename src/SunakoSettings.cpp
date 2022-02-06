//
// Created by ft on 05.02.22.
//

#include "SunakoSettings.h"
#include <QString>
#include "CarlaUtils.h"
#include <QSettings>
#include "carla/CarlaUtils.hpp"

SunakoSettings::SunakoSettings() {
    tmpFile.open();
    settings = new QSettings(tmpFile.fileName(), QSettings::IniFormat);
}
SunakoSettings::~SunakoSettings() {

}

void SunakoSettings::setMinimizeToTray(bool value) {
    settings ->setValue("minimize_to_tray", value);
}

bool SunakoSettings::getMinimizeToTray() {
    return settings ->value("minimize_to_tray", false).toBool();
}

void SunakoSettings::setName(QString value) {
    settings ->setValue("name", value);
}

void SunakoSettings::setIcon(QString value) {
    settings ->setValue("icon", value);
}

QString SunakoSettings::getName() {
    return settings ->value("name", QString("Title")).toString();
}

void SunakoSettings::setPluginPath(QString value) {
    settings ->setValue("plugin_path", value);
}

QString SunakoSettings::getPluginPath() {
    return settings ->value("plugin_path", QString()).toString();
}

QString SunakoSettings::getIcon() {
    return settings ->value("icon", QString()).toString();
}


CarlaBackend::BinaryType SunakoSettings::getBinaryType() {
    return CarlaBackend::BINARY_NATIVE;
}
CarlaBackend::PluginType SunakoSettings::getPluginType() {
    return CarlaBackend::PLUGIN_VST2;
}

void SunakoSettings::printSettings() {
    printf("Icon: %s", this -> getIcon().toStdString().c_str());
}