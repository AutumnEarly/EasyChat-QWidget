
#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include <QApplication>

#include "global.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString fileName = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath();
    QString config_path = QDir::toNativeSeparators( app_path + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;
    qDebug() << "gate url " << gate_url_prefix << " config_path " << config_path;
    w.show();
    return a.exec();

}
