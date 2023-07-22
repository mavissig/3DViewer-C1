/**
 * @file viewer.h
 * @brief Содержит объявление класса Viewer, представляющего виджет для
 * отображения 3D-сцены.
 * @details В файле также присутствуют включения заголовочных файлов, объявление
 * внешнего "C" блока и внешних структур данных.
 * @author Egor Kondratov(https://github.com/mavissig)
 */

#ifndef VIEWER_H
#define VIEWER_H

#include <QColor>
#include <QImage>
#include <QOpenGLWidget>
#include <QtGui>
#include <cmath>
#include <functional>

extern "C" {
#include "../../backend/headers/calculate.h"
#include "../../backend/headers/parser_s.h"
}

/**
 * @class Viewer
 * @brief Класс, представляющий виджет для отображения 3D-сцены.
 *
 * @details является наследником класса QOpenGLWidget и предоставляет
 * функциональность для отображения 3D-сцены. Класс содержит приватные поля
 * и методы для внутренней реализации.
 */
class Viewer final : public QOpenGLWidget {
 private:
  vertex_s* vertex_;
  float offsetX_ = 0;
  float offsetY_ = 0;
  float offsetZ_ = 0;
  float angleX_ = 0;
  float angleY_ = 0;
  float angleZ_ = 0;
  float* colorArray_;
  float* colorPointsArray_;
  surface_s* surface_;
  float xRot_;
  float yRot_;
  float zRot_;
  float zTra_;
  float nSca_;
  float maxX_;
  float maxY_;
  float maxZ_;
  QPoint ptrMousePosition_;
  QColor backgroundColor_ = Qt::white;
  QColor linesColor_ = Qt::black;
  QColor pointColor_ = Qt::black;
  int pointType_ = 0;
  float pointSize_ = 1.0f;
  float lineWidth_ = 1.0f;
  bool lineType_ = false;
  bool projectionType_ = false;
  bool axisVisible_ = false;

  void scale_plus() noexcept;
  void scale_minus() noexcept;
  void defaultScene() noexcept;
  void drawAxis() const;
  void drawVertex() const;

 protected:
  /**
   * @brief Инициализирует окно 3D-сцены.
   *
   */
  void initializeGL() override;

  /**
   * @brief Изменяет свойства окна 3D-сцены.
   *
   * @param nWidth Ширина окна.
   * @param nHeight Высота окна.
   */
  void resizeGL(const int nWidth, const int nHeight) override;

  /**
   * @brief Отрисовка 3D-сцены.
   *
   */
  void paintGL() override;

  /**
   * @brief Захват позиции курсора при нажатии мыши.
   *
   * @param event Событие нажатия на кнопку мыши.
   */
  void mousePressEvent(QMouseEvent* event) override;

  /**
   * @brief Захват позиции курсора при перемещении зажатой клавиши мыши.
   *
   * @param event Событие нажатия на кнопку мыши.
   */
  void mouseMoveEvent(QMouseEvent* event) override;

  /**
   * @brief Захват позиции колеса мыши.
   *
   * @param event Событие вращения колеса мыши.
   */
  void wheelEvent(QWheelEvent* event) override;

 public:
  /**
   * @brief Конструктор класса Viewer.
   *
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  explicit Viewer(QWidget* parent = 0);

  /**
   * @brief Получение указателя на объект vertex_s, содержащий вершины сцены.
   *
   * @return Указатель на объект vertex_s.
   */
  vertex_s* getVertex() const noexcept;

  /**
   * @brief Получение указателя на объект surface_s, содержащий примитивы сцены.
   *
   * @return Указатель на объект surface_s.
   */
  surface_s* getSurface() const noexcept;

  /**
   * @brief Задание цветов для ребер.
   *
   * @param color Цвет ребер (по умолчанию черный).
   */
  void setLineColor(const QColor& color = Qt::black);

  /**
   * @brief Задание цветов для вершин.
   *
   * @param color Цвет вершин (по умолчанию черный).
   */
  void setPointColor(const QColor& color = Qt::black);

  /**
   * @brief Задать максимальные значения координат x,y,z
   *
   * @param x Координата x.
   * @param y Координата y.
   * @param z Координата z.
   */
  void setMaxCoords(float&& x, float&& y, float&& z) noexcept;

  /**
   * @brief Установка массива вершин для отрисовки.
   *
   * @param vertex Указатель на объект vertex_s с вершинами сцены.
   */
  void setVertexArray(vertex_s&& vertex);

  /**
   * @brief Установка массива индексов вершин.
   *
   * @param surface Указатель на объект surface_s с индексами вершин.
   */
  void setIndexArray(surface_s&& surface);

  /**
   * @brief Установка цвета фона сцены.
   *
   * @param color Цвет сцены.
   */
  void setBackgroundColor(const QColor& color);

  /**
   * @brief Сохраняет изображение.
   *
   * @param filename Абсолютный путь до объекта.
   */
  void saveImage(const QString& filename);

  /**
   * @brief Получает цвет ребер.
   *
   * @return Цвет ребер.
   */
  QColor getLineColor() const noexcept;

  /**
   * @brief Получает цвет вершин.
   *
   * @return Цвет вершин.
   */
  QColor getPointColor() const noexcept;

  /**
   * @brief Получает цвет фона.
   *
   * @return Цвет фона.
   */
  QColor getBackgroundColor() const noexcept;

  /**
   * @brief Получает толщину ребер.
   *
   * @return Толщина вершины.
   */
  float getLineWidth() const noexcept;

  /**
   * @brief Получает размер вершин.
   *
   * @return Размер вершин.
   */
  float getPointSize() const noexcept;

  /**
   * @brief Получает тип вершин.
   *
   * @return Тип вершин.
   * @retval 0 - нет.
   * @retval 1 - круг.
   * @retval 2 - квадрат.
   */
  int getPointType() const noexcept;

  /**
   * @brief Получает тип ребер.
   *
   * @return Тип ребер.
   * @retval false - сплошная.
   * @retval true - пунктирная.
   */
  bool getLineType() const noexcept;

  /**
   * @brief Получает тип проекции.
   *
   * @return Тип проекции.
   * @retval false - параллельная.
   * @retval true - центральная.
   */
  bool getProjectionType() const noexcept;

  /**
   * @brief Устанавливает тип ребер.
   *
   * @param type тип ребер.
   */
  void setLineType(const bool& type) noexcept;

  /**
   * @brief Устанавливает тип вершин.
   *
   * @param type тип вершин.
   */
  void setPointType(const int& type) noexcept;

  /**
   * @brief Устанавливает размер вершин.
   *
   * @param size размер вершин.
   */
  void setPointSize(const float& size) noexcept;

  /**
   * @brief Устанавливает толщину ребер.
   *
   * @param width толщина ребер.
   */
  void setLineWidth(const float& width) noexcept;

  /**
   * @brief Устанавливает тип проекции.
   *
   * @param type тип проекции.
   */
  void setProjectionType(const bool& type) noexcept;

  /**
   * @brief Вызывает resizeGL().
   *
   */
  void resizeWindow();

  /**
   * @brief Отображает оси 3D-сцены.
   *
   */
  void showAxis() noexcept;

  /**
   * @brief Скрывает оси 3D-сцены.
   *
   */
  void hideAxis() noexcept;

  /**
   * @brief Проверяет создание объекта colorPointsArray_.
   *
   * @return
   * @retval false - объект не создан.
   * @retval true - объект создан.
   */
  bool isInit() const noexcept;

  void setOffsetX(const float& offset) noexcept;
  void setOffsetY(const float& offset) noexcept;
  void setOffsetZ(const float& offset) noexcept;
  float getOffsetX() const noexcept;
  float getOffsetY() const noexcept;
  float getOffsetZ() const noexcept;
  void setDefaultOffset() noexcept;
  void changeVertexArray(
      std::function<float(const float&, const float&)> summator);
  void setAngleX(const float& angle) noexcept;
  void setAngleY(const float& angle) noexcept;
  void setAngleZ(const float& angle) noexcept;
  float getAngleX() const noexcept;
  float getAngleY() const noexcept;
  float getAngleZ() const noexcept;
  void setDefaultAngle() noexcept;

  /**
   * @brief Деструктор класса Viewer.
   */
  ~Viewer();
};

#endif  // VIEWER_H
