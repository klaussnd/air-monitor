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
   const auto temp_it = obj.constFind("temperature");
   if (temp_it != obj.end())
   {
      const auto temp = temp_it->isDouble()
                           ? std::optional<float>{static_cast<float>(temp_it->toDouble())}
                           : std::nullopt;
      emit receivedTemperature(temp);
   }
   const auto hum_it = obj.constFind("humidity");
   if (hum_it != obj.end())
   {
      const auto humidity = hum_it->isDouble()
                               ? std::optional<unsigned int>{static_cast<unsigned int>(
                                  std::max(std::round(hum_it->toDouble()), 0.0))}
                               : std::nullopt;
      emit receivedHumidity(humidity);
   }
   const auto co2_it = obj.constFind("co2");
   if (co2_it != obj.end())
   {
      const auto co2 = co2_it->isDouble()
                          ? std::optional<unsigned int>{static_cast<unsigned int>(
                             std::max(std::round(co2_it->toDouble()), 0.0))}
                          : std::nullopt;
      emit receivedCo2(co2);
   }
}
