#pragma once

#include <mqttbase/MqttCallback.h>
#include <mqttbase/MqttClient.h>

#include <QObject>

class MqttDataCollector : public QObject, public MqttCallback
{
   Q_OBJECT
public:
   explicit MqttDataCollector(std::string mqtt_host, std::string mqtt_topic,
                              QObject* parent = nullptr);
   void connect();

signals:
   void isConnected();
   void receivedCo2(std::optional<unsigned int> co2);
   void receivedHumidity(std::optional<unsigned int> humidity);
   void receivedTemperature(std::optional<float> temperature);

private:
   void connected() override;
   void messageArrived(const std::string& topic, const std::string& value) override;

   MqttClient m_mqtt_client;
   std::string m_topic;
};
