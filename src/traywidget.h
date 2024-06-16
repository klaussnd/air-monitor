#pragma once

#include "colormap.h"
#include <optional>

#include <QSystemTrayIcon>

class TrayWidget : public QSystemTrayIcon
{
   Q_OBJECT
public:
   explicit TrayWidget(QObject* parent = nullptr);
   ~TrayWidget() override;
   void setupUi(void);

public slots:
   void isConnected();
   void dataCo2(std::optional<unsigned int> co2);
   void dataTemperature(std::optional<float> temperature);
   void dataHumidity(std::optional<unsigned int> humidity);

private:
   void updateToolTip();
   void setIcon();

private slots:
   void iconActivated(QSystemTrayIcon::ActivationReason);

private:
   Colormap m_colormap;
   std::optional<float> m_temperature;
   std::optional<unsigned int> m_humidity;
   std::optional<unsigned int> m_co2;
};
