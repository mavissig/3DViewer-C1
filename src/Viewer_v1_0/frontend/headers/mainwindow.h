/**
 * @file mainwindow.h
 * @brief Содержит объявление класса MainWindow, представляющего главное окно
 * приложения.
 * @details В файле также присутствуют включения заголовочных файлов, объявление
 * внешнего "C" блока и внешних функций.
 * @author Egor Kondratov(https://github.com/mavissig)
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QTimer>
#include <list>

#include "settings.h"
#include "ui_mainwindow.h"
#include "viewer.h"

extern "C" {
#include "../../backend/headers/calculate.h"
#include "../../backend/headers/parser.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Класс, представляющий главное окно приложения.
 *
 * MainWindow является наследником класса QMainWindow и предоставляет основное
 * окно приложения. Класс содержит слоты (private slots) и приватные поля и
 * методы для внутренней реализации.
 */
class MainWindow final : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор класса MainWindow.
   * @param parent Родительский объект (по умолчанию nullptr).
   */
  MainWindow(QWidget *parent = nullptr);

  /**
   * @brief Деструктор класса MainWindow.
   */
  ~MainWindow();

 private slots:

  void open_new_file();
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void on_rotationX_editingFinished();
  void on_rotationY_editingFinished();
  void on_rotationZ_editingFinished();
  void on_moveX_editingFinished();
  void on_moveY_editingFinished();
  void on_moveZ_editingFinished();
  void on_scaleX_editingFinished();
  void on_scaleY_editingFinished();
  void on_scaleZ_editingFinished();
  void getFromListView();
  void on_choose_color_clicked();
  void updateGif();
  void on_choose_color_lines_clicked();
  void on_choose_color_vertex_clicked();
  void on_action_saveJpeg_triggered();
  void on_action_saveBmp_triggered();
  void on_lineWidthSpin_editingFinished();
  void on_sizePointSpin_editingFinished();
  void on_vertex_type_currentIndexChanged(int index);
  void on_lines_type_currentIndexChanged(int index);
  void on_projection_type_currentIndexChanged(int index);
  void on_action_lightTheme_triggered();
  void on_action_drawAxis_triggered();
  void on_action_hideAxis_triggered();
  void on_action_darkTheme_triggered();
  void on_action_screencast_triggered();
  void on_action_SystemInformation_triggered();

 private:
  void createGif();
  void set_num_elements();
  void convertGif();
  void readConfig();
  void updateConfig();
  void reader(const QString &file);
  void saveObject(const QString &fileName);
  float gifTimeStart_ = 0.0, gifTimeEnd_ = 0.0;
  int counter_ = 1;
  const QString tempScreenDir_ = "screens";
  QTimer *timer;
  QString gifPath_ = "";
  QString gifName_ = "";
  std::list<QString> lastFiles_;
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
