#include "../headers/settings.h"

Settings::Settings(QObject* parent)
    : QObject(parent), m_settings(".settings.cfg", QSettings::IniFormat) {}

void Settings::set(const QString& key, const QString& value) {
  m_settings.setValue(key, value);
}

QString Settings::get(const QString& key) const {
  return m_settings.value(key).toString();
}

QColor Settings::getColor(const QString& section) {
  Settings settings;
  QColor colorOut;
  const int red = settings.get(section + "/red").toInt();
  const int green = settings.get(section + "/green").toInt();
  const int blue = settings.get(section + "/blue").toInt();
  const int alpha = settings.get(section + "/alpha").toInt();
  colorOut.setRgb(red, green, blue, alpha);
  return colorOut;
}

void Settings::setColor(const QColor& color, const QString& section) {
  Settings settings;
  settings.set(section + "/red", QString::number(color.red()));
  settings.set(section + "/green", QString::number(color.green()));
  settings.set(section + "/blue", QString::number(color.blue()));
  settings.set(section + "/alpha", QString::number(color.alpha()));
}
