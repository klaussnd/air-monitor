#include "traywidget.h"

#include <QAction>
#include <QCoreApplication>
#include <QMenu>
#include <QPainter>
#include <QStringLiteral>

TrayWidget::TrayWidget(QObject* parent)
      : QSystemTrayIcon{parent}
      , m_colormap(600., 1200.)
{
   setupUi();
}

TrayWidget::~TrayWidget()
{
}

void TrayWidget::isConnected()
{
   setToolTip(toolTip() + " - connected");
}

void TrayWidget::data(float temperature, int humidity, int co2)
{
   setToolTip(QStringLiteral(u"CO2 monitor\nTemperature %1°C\nHumidity %2%\nCO2 %3ppm")
                 .arg(temperature)
                 .arg(humidity)
                 .arg(co2));
   setIcon(co2);
}

void TrayWidget::setupUi(void)
{
   setToolTip("CO2 monitor - no data");

   // context menu entries
   QAction* act_quit = new QAction(tr("&Quit"), this);
   connect(act_quit, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));
   // context menu for system tray
   QMenu* tray_menu = new QMenu();
   tray_menu->addAction(act_quit);
   setContextMenu(tray_menu);
   // temporary icon
   QPixmap pixmap{32, 32};
   pixmap.fill(QColorConstants::Gray);
   QSystemTrayIcon::setIcon(QIcon{pixmap});
}

/** Called when user clicks on the System Tray Icon */
void TrayWidget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
   switch (reason)
   {
   case QSystemTrayIcon::Trigger:
   case QSystemTrayIcon::DoubleClick:
      break;
   case QSystemTrayIcon::MiddleClick:
      break;
   default:
      break;
   }
}

void TrayWidget::setIcon(int co2)
{
   QRectF iconRect{0, 0, 32, 32};
   QPixmap pixmap{iconRect.size().toSize()};
   const auto color = m_colormap.color(co2);
   pixmap.fill(color);
   QPainter painter{&pixmap};
   painter.setPen(QColor{0, 0, 0});
   painter.drawText(iconRect, QString::number(co2),
                    QTextOption{Qt::AlignVCenter | Qt::AlignHCenter});
   QSystemTrayIcon::setIcon(QIcon{pixmap});
}
