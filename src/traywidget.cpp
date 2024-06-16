#include "traywidget.h"

#include <QAction>
#include <QCoreApplication>
#include <QMenu>
#include <QPainter>
#include <QStringLiteral>

namespace
{
constexpr const char* NOT_AVAIL = u8"n/a";
}

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

void TrayWidget::dataCo2(std::optional<unsigned int> co2)
{
   m_co2 = co2;
   setIcon();
   updateToolTip();
}

void TrayWidget::dataTemperature(std::optional<float> temperature)
{
   m_temperature = temperature;
   updateToolTip();
}

void TrayWidget::dataHumidity(std::optional<unsigned int> humidity)
{
   m_humidity = humidity;
   updateToolTip();
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

void TrayWidget::updateToolTip()
{
   setToolTip(
      QStringLiteral(u"CO2 monitor\nTemperature %1°C\nHumidity %2%\nCO2 %3ppm")
         .arg(m_temperature.has_value() ? QString::number(m_temperature.value())
                                        : NOT_AVAIL)
         .arg(m_humidity.has_value() ? QString::number(m_humidity.value()) : NOT_AVAIL)
         .arg(m_co2.has_value() ? QString::number(m_co2.value()) : NOT_AVAIL));
}

void TrayWidget::setIcon()
{
   QRectF iconRect{0, 0, 32, 32};
   QPixmap pixmap{iconRect.size().toSize()};
   const auto color =
      m_co2.has_value() ? m_colormap.color(m_co2.value()) : QColorConstants::Gray;
   pixmap.fill(color);
   QPainter painter{&pixmap};
   painter.setPen(QColor{0, 0, 0});
   painter.drawText(iconRect,
                    m_co2.has_value() ? QString::number(m_co2.value()) : NOT_AVAIL,
                    QTextOption{Qt::AlignVCenter | Qt::AlignHCenter});
   QSystemTrayIcon::setIcon(QIcon{pixmap});
}
