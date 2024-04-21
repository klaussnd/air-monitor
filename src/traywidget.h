#pragma once

#include "colormap.h"

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
   void data(float temperature, int humidity, int co2);

private:
   void setIcon(int co2);

private slots:
   void iconActivated(QSystemTrayIcon::ActivationReason);

private:
   Colormap m_colormap;
};
