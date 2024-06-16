#include "mqttdatacollector.h"
#include "traywidget.h"
#include <mqttbase/MqttClient.h>

#include <QApplication>
#include <QMessageBox>
#include <QSettings>

struct Settings
{
   std::string mqtt_host;
   std::string topic;
};

Settings readSettings();
std::string settingsFilename();

int main(int argc, char* argv[])
{
   QApplication app(argc, argv);
   QCoreApplication::setApplicationName("AirMonitor");
   QCoreApplication::setOrganizationName("KSZ");

   const auto settings = readSettings();
   try
   {
      if (settings.mqtt_host.empty() || settings.topic.empty())
      {
         throw std::runtime_error("Settings not defined.\n"
                                  "Edit the file "
                                  + settingsFilename()
                                  + " and set:\n"
                                    "MqttHost=mqtt-host\n"
                                    "MqttTopic=mqtt/topic");
      }
      MqttDataCollector collector(settings.mqtt_host, settings.topic);

      TrayWidget widget;
      QObject::connect(&collector, &MqttDataCollector::isConnected, &widget,
                       &TrayWidget::isConnected);
      QObject::connect(&collector, &MqttDataCollector::receivedCo2, &widget,
                       &TrayWidget::dataCo2);
      QObject::connect(&collector, &MqttDataCollector::receivedTemperature, &widget,
                       &TrayWidget::dataTemperature);
      QObject::connect(&collector, &MqttDataCollector::receivedHumidity, &widget,
                       &TrayWidget::dataHumidity);
      widget.show();

      collector.connect();

      return app.exec();
   }
   catch (const std::exception& err)
   {
      QMessageBox::critical(nullptr, "Error", err.what());
   }
}

Settings readSettings()
{
   Settings settings;
   QSettings qset;
   settings.mqtt_host = qset.value("MqttHost", "").toString().toStdString();
   settings.topic = qset.value("MqttTopic", "").toString().toStdString();
   return settings;
}

std::string settingsFilename()
{
   return QSettings{}.fileName().toStdString();
}
