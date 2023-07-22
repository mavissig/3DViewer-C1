/**
 * @file settings.h
 * @brief Содержит объявление класса Settings, отвечающего за чтение и
 * сохранение настроек
 * @details Класс Settings предоставляет методы для работы с настройками
 * приложения, такими как чтение, запись и установка значений по умолчанию. Он
 * использует QSettings для доступа к файлу настроек и хранения значений. Класс
 * наследуется от QObject и предоставляет сигналы и слоты для обработки событий.
 * @author Egor Kondratov(https://github.com/mavissig)
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QObject>
#include <QSettings>

class Settings final : public QObject {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор класса Settings
   *
   * @param parent Родительский объект
   */
  explicit Settings(QObject* parent = nullptr);

  /**
   * @brief Получает значение настройки по ключу
   *
   * @param key Ключ настройки
   * @return Значение настройки
   */
  QString get(const QString& key) const;

  /**
   * @brief Записывает значение настройки в .cfg через ключ
   *
   * @param key Ключ
   * @param value Значение
   * @details пример1:
   * @code set(key, QString::number(color.green())); @endcode
   * @details пример2:
   * @code set("colorBackground/green", 255); @endcode
   */
  void set(const QString& key, const QString& value);

  /**
   * @brief Получает цвет, исходя из настроек конфига
   *
   * @details Получает настройки цвета из конфига и формирует из них QColor цвет
   * @param section секция
   * @return QColor
   */
  static QColor getColor(const QString& section);

  /**
   * @brief Получает тип отображения для определенного ключа секции "TypeView",
   * исходя из настроек конфига
   *
   * @param key ключ в секции "TypeView"
   * @return T значение из TypeView/key
   *
   * @warning ключ передаётся без секции!
   *
   * пример:
   * @code
   * Settings::getTypeView("projection");
   * @endcode
   */
  template <typename T>
  static T getTypeView(const QString& key) {
    Settings settings;
    QString val = settings.get("TypeView" + key);
    const auto value = (val.contains('.')) ? val.toDouble() : val.toInt();
    return value;
  }

  /**
   * @brief Записывает значения цвета в секцию
   *
   * @param color
   * @param section
   *
   */
  static void setColor(const QColor& color, const QString& section);

  /**
   * @brief Записывает параметры отображения по ключу в секцию "TypeView"
   *
   * @details Так как типы у нас могут быть только в одном состоянии, то мы
   * меняем только val, он может быть 0 или 1 для типа проекции и типа линии,
   * для вершин он может быть 1, 2 или 3
   *
   * @param val записываемое значение в "TypeView/key"
   * @param key ключ
   *
   * @warning ключ передаётся без секции!
   *
   * пример:
   * @code Settings::void setType(1, "projection"); @endcode
   *
   * @todo можно по ситуации изменить val с int на QString
   */
  template <typename T>
  static void setTypeView(const T& val, const QString& key) {
    Settings settings;
    settings.set("TypeView/" + key, QString::number(val));
  }

 private:
  QSettings m_settings;
};

#endif  // SETTINGS_H
