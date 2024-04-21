#include "mqttdatacollector.h"

#include <QJsonDocument>
#include <QJsonObject>

MqttDataCollector::MqttDataCollector(std::string mqtt_host, std::string mqtt_topic,
                                     QObject* parent)
      : QObject{parent}
      , m_mqtt_client{MqttClientSettings{std::move(mqtt_host), {}, {}, "AirMonitor", {}}}
      , m_topic(std::move(mqtt_topic))
{
   m_mqtt_client.setCallback(*this);
}

void MqttDataCollector::connect()
{
   m_mqtt_client.connectAsync();
}

void MqttDataCollector::connected()
{
   m_mqtt_client.subscribe(m_topic);
   emit isConnected();
}

void MqttDataCollector::messageArrived(const std::string& topic, const std::string& value)
{
   if (topic != m_topic)
   {
      return;
   }

   const QJsonDocument doc = QJsonDocument::fromJson(
      QByteArray{value.c_str(), static_cast<int>(value.length())});
   const auto& obj = doc.object();
   const auto tempIt = obj.constFind("temperature");
   const auto humIt = obj.constFind("humidity");
   const auto co2It = obj.constFind("co2");
   if (co2It != obj.end() && tempIt != obj.end() && humIt != obj.end())
   {
      const float temp = static_cast<float>(tempIt->toDouble());
      const int humidity = static_cast<int>(std::round(humIt->toDouble()));
      const int co2 = static_cast<int>(std::round(co2It->toDouble()));
      emit dataReceived(temp, humidity, co2);
   }
}
