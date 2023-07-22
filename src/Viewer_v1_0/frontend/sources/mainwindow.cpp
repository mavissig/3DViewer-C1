#include "../headers/mainwindow.h"

#include "../headers/gif.h"

namespace {
QString getName(const QString& filePath);
QString contains(const std::list<QString>& filePaths, const QString& fileName);
QString getFilePath(const QString& absolutePath);
}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      timer(std::move(new QTimer(this))),
      ui(std::move(new Ui::MainWindow)) {
  ui->setupUi(this);
  setAcceptDrops(true);
  on_action_lightTheme_triggered();
  connect(ui->upload_file, SIGNAL(clicked()), this, SLOT(open_new_file()));
  connect(ui->action_openNewFile, SIGNAL(triggered()), this,
          SLOT(open_new_file()));
  try {
    readConfig();
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Configuration", e.what());
  }
}

void MainWindow::readConfig() {
  const QColor backGroundColor = Settings::getColor("colorBackground");
  const QColor lineColor = Settings::getColor("colorLine");
  const QColor vertexColor = Settings::getColor("colorVertex");
  const int projectionType = Settings::getTypeView<int>("/projectionType");
  const int lineType = Settings::getTypeView<int>("/lineType");
  const int vertexType = Settings::getTypeView<int>("/vertexType");
  const double vertexSize = Settings::getTypeView<double>("/vertexSize");
  const double lineSize = Settings::getTypeView<double>("/lineSize");
  ui->model->setBackgroundColor(backGroundColor);
  ui->model->setLineColor(lineColor);
  ui->model->setPointColor(vertexColor);
  ui->model->setProjectionType(static_cast<bool>(projectionType));
  ui->model->setLineType(static_cast<bool>(lineType));
  ui->model->setPointType(vertexType);
  ui->model->setPointSize(vertexSize);
  ui->model->setLineWidth(lineSize);
  ui->back_color->setStyleSheet("background-color:rgba(" +
                                QString::number(backGroundColor.red()) + "," +
                                QString::number(backGroundColor.green()) + "," +
                                QString::number(backGroundColor.blue()) + "," +
                                QString::number(backGroundColor.alpha()) + ")");
  ui->lines_color->setStyleSheet("background-color:rgba(" +
                                 QString::number(lineColor.red()) + "," +
                                 QString::number(lineColor.green()) + "," +
                                 QString::number(lineColor.blue()) + "," +
                                 QString::number(lineColor.alpha()) + ")");
  ui->vertex_color->setStyleSheet("background-color:rgba(" +
                                  QString::number(vertexColor.red()) + "," +
                                  QString::number(vertexColor.green()) + "," +
                                  QString::number(vertexColor.blue()) + "," +
                                  QString::number(vertexColor.alpha()) + ")");
  ui->projection_type->setCurrentIndex(
      static_cast<int>(static_cast<bool>(projectionType)));
  ui->lines_type->setCurrentIndex(
      static_cast<int>(static_cast<bool>(lineType)));
  ui->vertex_type->setCurrentIndex(vertexType);
  ui->lineWidthSpin->setValue(lineSize);
  ui->sizePointSpin->setValue(vertexSize);
}

void MainWindow::updateConfig() {
  Settings::setColor(ui->model->getBackgroundColor(), "colorBackground");
  Settings::setColor(ui->model->getLineColor(), "colorLine");
  Settings::setColor(ui->model->getPointColor(), "colorVertex");
  Settings::setTypeView(ui->model->getProjectionType(), "/projectionType");
  Settings::setTypeView(ui->model->getLineType(), "/lineType");
  Settings::setTypeView(ui->model->getPointType(), "/vertexType");
  Settings::setTypeView(ui->model->getLineWidth(), "/lineSize");
  Settings::setTypeView(ui->model->getPointSize(), "/vertexSize");
}

MainWindow::~MainWindow() {
  try {
    updateConfig();
    QDir trash{this->gifPath_ + this->tempScreenDir_ + "/"};
    trash.removeRecursively();
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Warning", e.what());
  }
  delete this->timer;
  delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) { event->accept(); }

void MainWindow::dropEvent(QDropEvent* event) {
  const QMimeData* mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    const QList<QUrl> urlList = mimeData->urls();
    const QString file = urlList.first().toLocalFile();
    const QFileInfo fileInfo(file);
    if (fileInfo.suffix().toLower() == "obj") {
      reader(file);
      event->acceptProposedAction();
      return;
    }
  }
  event->ignore();
}

void MainWindow::open_new_file() {
  const QString path =
      (lastFiles_.empty()) ? "/home/" : getFilePath(this->lastFiles_.back());
  const QStringList files = QFileDialog::getOpenFileNames(
      this, tr("Открыть файлы"), path, "Все файлы (*.obj);;");
  if (!files.empty()) {
    const QString object = QFileInfo(files.at(0)).absoluteFilePath();
    reader(object);
  }
}

void MainWindow::reader(const QString& file) {
  float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
  FILE* objectFile = fopen(file.toUtf8().data(), "r");
  if (nullptr != objectFile) {
    vertex_s vertex = {.coords = nullptr, .size = 0, .capacity = 3};
    vertex.coords = static_cast<float*>(calloc(vertex.capacity, sizeof(float)));
    surface_s surface = {.points = nullptr, .size = 0, .capacity = 3};
    surface.points = static_cast<unsigned int*>(
        calloc(surface.capacity, sizeof(unsigned int)));
    parser(objectFile, &vertex, &surface, &maxX, &maxY, &maxZ);
    if (0 != vertex.size && 0 != surface.size) {
      ui->model->setMaxCoords(std::move(maxX), std::move(maxY),
                              std::move(maxZ));
      ui->model->setVertexArray(std::move(vertex));
      ui->model->setIndexArray(std::move(surface));
      ui->model->repaint();
      set_num_elements();
      saveObject(file);
      ui->fileName->setMinimumWidth(file.length() * 10);
      ui->fileName->setText(file);
      ui->moveX->setValue(0.0);
      ui->moveY->setValue(0.0);
      ui->moveZ->setValue(0.0);
      ui->rotationX->setValue(0.0);
      ui->rotationY->setValue(0.0);
      ui->rotationZ->setValue(0.0);
      ui->scaleX->setValue(1.0);
      ui->scaleY->setValue(1.0);
      ui->scaleZ->setValue(1.0);
      ui->model->setDefaultOffset();
      ui->model->setDefaultAngle();
    }
    fclose(objectFile);
  } else {
    if (contains(lastFiles_, getName(file)) != "") {
      size_t index = 0;
      for (auto iter = lastFiles_.cbegin(); index < lastFiles_.size();
           ++iter, ++index) {
        if (*iter == file) {
          lastFiles_.erase(iter);
          ui->lastFiles_2->removeAction(ui->lastFiles_2->actions().at(index));
          break;
        }
      }
    }
  }
}

void MainWindow::saveObject(const QString& fileName) {
  const QString objName = getName(fileName);
  try {
    if (contains(this->lastFiles_, objName) == "") {
      if (this->lastFiles_.size() == 5) {
        this->lastFiles_.remove(*this->lastFiles_.begin());
        ui->lastFiles_2->removeAction(ui->lastFiles_2->actions().at(0));
      }
      QAction* action = new QAction();
      action->setText(objName);
      this->lastFiles_.push_back(fileName);
      connect(action, SIGNAL(triggered(bool)), this, SLOT(getFromListView()));
      ui->lastFiles_2->addAction(std::move(action));
    }
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Warning", "Something went wrong");
  }
}

void MainWindow::createGif() {
  QDir screenDir;
  screenDir.mkdir(this->gifPath_ + this->tempScreenDir_);
  this->gifTimeStart_ = 0.0;
  this->gifTimeEnd_ = 100.0;
  connect(this->timer, SIGNAL(timeout()), this, SLOT(updateGif()));
  this->timer->start(this->gifTimeEnd_);
}

void MainWindow::updateGif() {
  if (this->gifTimeStart_ == this->gifTimeEnd_) {
    const QString fileName = this->gifPath_ + this->tempScreenDir_ + "/" +
                             QString::number(this->counter_) + ".bmp";
    ui->model->saveImage(fileName);
    ++this->counter_;
    this->gifTimeEnd_ += 100.0;
  }
  if (this->gifTimeStart_ == 5000) {
    convertGif();
    timer->stop();
    timer->disconnect();
    this->counter_ = 1;
  }
  this->gifTimeStart_ += 100.0;
}

void MainWindow::convertGif() {
  const QByteArray data = (this->gifPath_ + this->gifName_).toLocal8Bit();
  GifWriter creator{};
  try {
    GifBegin(&creator, data.data(), ui->model->grab().width(),
             ui->model->grab().height(), 10);
    for (int i = 1; i < this->counter_; ++i) {
      const QString filename = this->gifPath_ + this->tempScreenDir_ + "/" +
                               QString::number(i) + ".bmp";
      const QImage image(filename);
      GifWriteFrame(&creator,
                    image.convertToFormat(QImage::Format_Indexed8)
                        .convertToFormat(QImage::Format_RGBA8888)
                        .constBits(),
                    ui->model->grab().width(), ui->model->grab().height(), 10);
    }
    GifEnd(&creator);
    QDir trash{this->gifPath_ + this->tempScreenDir_ + "/"};
    trash.removeRecursively();
  } catch (std::exception& e) {
    QMessageBox::warning(this, "Save gif", "Something went wrong");
  }
}

void MainWindow::getFromListView() {
  const QAction* action = static_cast<QAction*>(sender());
  reader(contains(this->lastFiles_, action->text()));
}

void MainWindow::on_rotationX_editingFinished() {
  if (ui->rotationX->hasFocus()) {
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 - val2; });
    rotate_model_X(ui->model->getVertex(), ui->rotationX->value());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 + val2; });
    ui->model->repaint();
    ui->model->setAngleX(ui->rotationX->value());
  }
}

void MainWindow::on_rotationY_editingFinished() {
  if (ui->rotationY->hasFocus()) {
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 - val2; });
    rotate_model_Y(ui->model->getVertex(), ui->rotationY->value());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 + val2; });
    ui->model->repaint();
    ui->model->setAngleY(ui->rotationY->value());
  }
}

void MainWindow::on_rotationZ_editingFinished() {
  if (ui->rotationZ->hasFocus()) {
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 - val2; });
    rotate_model_Z(ui->model->getVertex(), ui->rotationZ->value());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 + val2; });
    ui->model->repaint();
    ui->model->setAngleZ(ui->rotationZ->value());
  }
}

void MainWindow::on_moveX_editingFinished() {
  if (ui->moveX->hasFocus()) {
    move_model(ui->model->getVertex(), ui->moveX->value(), 0, 0);
    ui->model->repaint();
    ui->model->setOffsetX(ui->moveX->value());
  }
}

void MainWindow::on_moveY_editingFinished() {
  if (ui->moveY->hasFocus()) {
    move_model(ui->model->getVertex(), 0, ui->moveY->value(), 0);
    ui->model->repaint();
    ui->model->setOffsetY(ui->moveY->value());
  }
}

void MainWindow::on_moveZ_editingFinished() {
  if (ui->moveZ->hasFocus()) {
    move_model(ui->model->getVertex(), 0, 0, ui->moveZ->value());
    ui->model->repaint();
    ui->model->setOffsetZ(ui->moveZ->value());
  }
}

void MainWindow::on_scaleX_editingFinished() {
  if (ui->scaleX->value() != 0 && ui->scaleX->hasFocus()) {
    rotate_model_X(ui->model->getVertex(), -ui->model->getAngleX());
    rotate_model_Y(ui->model->getVertex(), -ui->model->getAngleY());
    rotate_model_Z(ui->model->getVertex(), -ui->model->getAngleZ());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 - val2; });

    scale_model(ui->model->getVertex(), ui->scaleX->value(), 1, 1);
    rotate_model_X(ui->model->getVertex(), ui->model->getAngleX());
    rotate_model_Y(ui->model->getVertex(), ui->model->getAngleY());
    rotate_model_Z(ui->model->getVertex(), ui->model->getAngleZ());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 + val2; });

    ui->model->repaint();
  }
}

void MainWindow::on_scaleY_editingFinished() {
  if (ui->scaleY->value() != 0 && ui->scaleY->hasFocus()) {
    rotate_model_X(ui->model->getVertex(), -ui->model->getAngleX());
    rotate_model_Y(ui->model->getVertex(), -ui->model->getAngleY());
    rotate_model_Z(ui->model->getVertex(), -ui->model->getAngleZ());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 - val2; });

    scale_model(ui->model->getVertex(), 1, ui->scaleY->value(), 1);
    rotate_model_X(ui->model->getVertex(), ui->model->getAngleX());
    rotate_model_Y(ui->model->getVertex(), ui->model->getAngleY());
    rotate_model_Z(ui->model->getVertex(), ui->model->getAngleZ());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 + val2; });

    ui->model->repaint();
  }
}

void MainWindow::on_scaleZ_editingFinished() {
  if (ui->scaleZ->value() != 0 && ui->scaleZ->hasFocus()) {
    rotate_model_X(ui->model->getVertex(), -ui->model->getAngleX());
    rotate_model_Y(ui->model->getVertex(), -ui->model->getAngleY());
    rotate_model_Z(ui->model->getVertex(), -ui->model->getAngleZ());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 - val2; });

    scale_model(ui->model->getVertex(), 1, 1, ui->scaleZ->value());
    rotate_model_X(ui->model->getVertex(), ui->model->getAngleX());
    rotate_model_Y(ui->model->getVertex(), ui->model->getAngleY());
    rotate_model_Z(ui->model->getVertex(), ui->model->getAngleZ());
    ui->model->changeVertexArray(
        [](const float& val1, const float& val2) { return val1 + val2; });
    ui->model->repaint();
  }
}

void MainWindow::set_num_elements() {
  const vertex_s* vertex = ui->model->getVertex();
  const surface_s* surface = ui->model->getSurface();
  if (nullptr != surface->points && nullptr != vertex->coords &&
      0 != vertex->size && 0 != surface->size) {
    ui->num_vertex->setText(QString::number(vertex->size / 3));
    ui->num_surface->setText(QString::number(surface->size / 3));
    ui->num_edge->setText(
        QString::number(get_amount_of_edges(surface, vertex->size / 3)));
  } else {
    ui->num_vertex->setText("No data");
    ui->num_surface->setText("No data");
    ui->num_edge->setText("No data");
  }
}

void MainWindow::on_choose_color_clicked() {
  const QColor color =
      QColorDialog::getColor(ui->model->getBackgroundColor(), this);
  if (color.isValid()) {
    ui->back_color->setStyleSheet(
        "background-color:rgba(" + QString::number(color.red()) + "," +
        QString::number(color.green()) + "," + QString::number(color.blue()) +
        "," + QString::number(color.alpha()) + ")");
    ui->model->setBackgroundColor(color);
    ui->model->repaint();
    ui->model->update();
  }
}

void MainWindow::on_choose_color_lines_clicked() {
  const QColor color = QColorDialog::getColor(ui->model->getLineColor(), this);
  if (color.isValid()) {
    ui->lines_color->setStyleSheet(
        "background-color:rgba(" + QString::number(color.red()) + "," +
        QString::number(color.green()) + "," + QString::number(color.blue()) +
        "," + QString::number(color.alpha()) + ")");
    ui->model->setLineColor(color);
    ui->model->repaint();
  }
}

void MainWindow::on_choose_color_vertex_clicked() {
  const QColor color = QColorDialog::getColor(ui->model->getPointColor(), this);
  if (color.isValid()) {
    ui->vertex_color->setStyleSheet(
        "background-color:rgba(" + QString::number(color.red()) + "," +
        QString::number(color.green()) + "," + QString::number(color.blue()) +
        "," + QString::number(color.alpha()) + ")");
    ui->model->setPointColor(color);
    ui->model->repaint();
  }
}

void MainWindow::on_action_saveJpeg_triggered() {
  QFileDialog dialog;
  const QString path =
      (lastFiles_.empty()) ? "/home/" : getFilePath(this->lastFiles_.back());
  dialog.setDirectory(path);
  QString filePath = dialog.getSaveFileName();
  if (!filePath.isEmpty()) {
    filePath += (filePath.split(".").back() != "jpeg") ? ".jpeg" : "";
    ui->model->saveImage(filePath);
  }
}

void MainWindow::on_action_saveBmp_triggered() {
  QFileDialog dialog;
  const QString path =
      (lastFiles_.empty()) ? "/home/" : getFilePath(this->lastFiles_.back());
  dialog.setDirectory(path);
  QString filePath = dialog.getSaveFileName();
  if (!filePath.isEmpty()) {
    filePath += (filePath.split(".").back() != "bmp") ? ".bmp" : "";
    ui->model->saveImage(filePath);
  }
}

void MainWindow::on_lineWidthSpin_editingFinished() {
  const float width = ui->lineWidthSpin->value();
  if (width != ui->model->getLineWidth()) {
    ui->model->setLineWidth(width);
    ui->model->repaint();
  }
}

void MainWindow::on_sizePointSpin_editingFinished() {
  const float size = ui->sizePointSpin->value();
  if (size != ui->model->getPointSize()) {
    ui->model->setPointSize(size);
    if (ui->model->getPointType()) {
      ui->model->repaint();
    }
  }
}

void MainWindow::on_vertex_type_currentIndexChanged(int index) {
  if (index != ui->model->getPointType()) {
    ui->model->setPointType(index);
    if (ui->model->isInit() == false) {
      ui->model->setPointColor(ui->model->getPointColor());
    }
    ui->model->repaint();
  }
}

void MainWindow::on_lines_type_currentIndexChanged(int index) {
  if (index != ui->model->getLineType()) {
    ui->model->setLineType(index);
    ui->model->repaint();
  }
}

void MainWindow::on_projection_type_currentIndexChanged(int index) {
  if (index != ui->model->getProjectionType()) {
    ui->model->setProjectionType(index);
    ui->model->resizeWindow();
    ui->model->update();
  }
}

void MainWindow::on_action_SystemInformation_triggered() {
  try {
#ifdef Q_OS_MAC
    QDir dir{QCoreApplication::applicationDirPath()};
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    const QString path = "file://" + dir.path() + "/guide/html/index.html";
#else
    const QString path =
        QCoreApplication::applicationDirPath() + "/../guide/html/index.html";
#endif
    QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
  } catch (const std::exception& e) {
    QMessageBox::warning(this, "Help", QString(e.what()));
  }
}

void MainWindow::on_action_drawAxis_triggered() {
  ui->model->showAxis();
  ui->model->repaint();
}

void MainWindow::on_action_hideAxis_triggered() {
  ui->model->hideAxis();
  ui->model->repaint();
}

void MainWindow::on_action_lightTheme_triggered() {
  const QColor greyLightColor(222, 221, 218);
  const QColor greenLightColor(48, 151, 174);
  const QString greyLightStr =
      "background-color: rgb(" + QString::number(greyLightColor.red()) + "," +
      QString::number(greyLightColor.green()) + "," +
      QString::number(greyLightColor.blue()) + ");" + "color:black;";
  const QString greenLightStr = "rgb(" +
                                QString::number(greenLightColor.red()) + "," +
                                QString::number(greenLightColor.green()) + "," +
                                QString::number(greenLightColor.blue()) + ");";
  this->setStyleSheet("background-color:white; color:black;");
  ui->upload_file->setStyleSheet("background-color:" + greenLightStr +
                                 "color:white;");
  ui->choose_color->setStyleSheet(greyLightStr);
  ui->projection_type->setStyleSheet(greyLightStr);
  ui->choose_color_lines->setStyleSheet(greyLightStr);
  ui->lines_type->setStyleSheet(greyLightStr);
  ui->choose_color_vertex->setStyleSheet(greyLightStr);
  ui->vertex_type->setStyleSheet(greyLightStr);
  ui->scrollArea->setStyleSheet(
      "QScrollBar{background: white;} QScrollBar::handle{background: "
      "rgb(222,221,218);} QScrollBar::add-line{background: white;} "
      "QScrollBar::sub-line{background: white;}");
  ui->menuBar->setStyleSheet(
      "QMenu {background-color: rgb(248,248,248);} QMenu::item:selected "
      "{background: "
      "rgb(222,221,218);} QMenu::icon:checked {background: gray;border: 1px "
      "inset gray;}"
      "QMenuBar::item:selected {background: "
      "rgb(222,221,218);}QMenuBar::item:pressed {background: "
      "rgb(222,221,218);}QMenu::separator {background: rgb(48, 151, 174); "
      "height: 1px}");
  ui->projection_type->setStyleSheet("QComboBox{" + greyLightStr + "}");
  ui->lines_type->setStyleSheet("QComboBox{" + greyLightStr + "}");
  ui->vertex_type->setStyleSheet("QComboBox{" + greyLightStr + "}");
  ui->moveX->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->moveY->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->moveZ->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->rotationX->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->rotationY->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->rotationZ->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->scaleX->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->scaleY->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->scaleZ->setStyleSheet("selection-background-color:" + greenLightStr);
  ui->lineWidthSpin->setStyleSheet("selection-background-color:" +
                                   greenLightStr);
  ui->sizePointSpin->setStyleSheet("selection-background-color:" +
                                   greenLightStr);
  QString separatorColor =
      "background-color:" + greenLightStr + "border: 1px solid white;";
  ui->separator_info_down->setStyleSheet(separatorColor);
  ui->separator_model_up->setStyleSheet(separatorColor);
  ui->separator_model_down->setStyleSheet(separatorColor);
  ui->separator_settings_up->setStyleSheet(separatorColor);
  ui->separator_settings_down->setStyleSheet(separatorColor);
}

void MainWindow::on_action_darkTheme_triggered() {
  const QColor greyDarkColor(64, 64, 64);
  const QColor greenDarkColor(0, 107, 60);
  const QString greyDarkStr =
      "background-color: rgb(" + QString::number(greyDarkColor.red()) + "," +
      QString::number(greyDarkColor.green()) + "," +
      QString::number(greyDarkColor.blue()) + ");" + "color:white;";
  const QString greenDarkStr = "rgb(" + QString::number(greenDarkColor.red()) +
                               "," + QString::number(greenDarkColor.green()) +
                               "," + QString::number(greenDarkColor.blue()) +
                               ");";
  this->setStyleSheet("background-color:rgb(48,48,48); color:white;");
  ui->upload_file->setStyleSheet("background-color:" + greenDarkStr +
                                 "color:white;");
  ui->choose_color->setStyleSheet(greyDarkStr);
  ui->projection_type->setStyleSheet(greyDarkStr);
  ui->choose_color_lines->setStyleSheet(greyDarkStr);
  ui->lines_type->setStyleSheet(greyDarkStr);
  ui->choose_color_vertex->setStyleSheet(greyDarkStr);
  ui->vertex_type->setStyleSheet(greyDarkStr);
  ui->scrollArea->setStyleSheet(
      "QScrollBar{background: rgb(48,48,48);} QScrollBar::handle{background: "
      "rgb(64,64,64);} QScrollBar::add-line{background: rgb(48,48,48);} "
      "QScrollBar::sub-line{background: rgb(48,48,48);}");
  ui->menuBar->setStyleSheet(
      "QMenu {background-color: rgb(56,56,56);} QMenu::item:selected "
      "{background: "
      "rgb(64,64,64);} QMenu::icon:checked {background: gray;border: 1px "
      "inset gray;}"
      "QMenuBar::item:selected {background: "
      "rgb(64,64,64);}QMenuBar::item:pressed {background: "
      "rgb(64,64,64);}QMenu::separator {background: rgb(0, 107, 60); height: "
      "1px}");
  ui->projection_type->setStyleSheet("QComboBox{" + greyDarkStr + "}");
  ui->lines_type->setStyleSheet("QComboBox{" + greyDarkStr + "}");
  ui->vertex_type->setStyleSheet("QComboBox{" + greyDarkStr + "}");
  ui->moveX->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->moveY->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->moveZ->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->rotationX->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->rotationY->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->rotationZ->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->scaleX->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->scaleY->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->scaleZ->setStyleSheet("selection-background-color:" + greenDarkStr);
  ui->lineWidthSpin->setStyleSheet("selection-background-color:" +
                                   greenDarkStr);
  ui->sizePointSpin->setStyleSheet("selection-background-color:" +
                                   greenDarkStr);
  QString separatorColor =
      "background-color:" + greenDarkStr + "border: 1px solid rgb(48,48,48);";
  ui->separator_info_down->setStyleSheet(separatorColor);
  ui->separator_model_up->setStyleSheet(separatorColor);
  ui->separator_model_down->setStyleSheet(separatorColor);
  ui->separator_settings_up->setStyleSheet(separatorColor);
  ui->separator_settings_down->setStyleSheet(separatorColor);
}

void MainWindow::on_action_screencast_triggered() {
  QFileDialog dialog;
  const QString path =
      (lastFiles_.empty()) ? "/home/" : getFilePath(this->lastFiles_.back());
  dialog.setDirectory(path);
  dialog.setNameFilter("GIF files (*.gif);;");
  QString absoluteFilePath = dialog.getSaveFileName();
  if (!absoluteFilePath.isEmpty()) {
    absoluteFilePath +=
        (absoluteFilePath.split(".").back() != "gif") ? ".gif" : "";
    this->gifName_ = getName(absoluteFilePath);
    this->gifPath_ = getFilePath(absoluteFilePath);
    createGif();
  }
}

namespace {

QString contains(const std::list<QString>& filePaths, const QString& fileName) {
  QString path{""};
  for (const auto& item : filePaths) {
    if (fileName == getName(item)) {
      path += item;
      break;
    }
  }
  return path;
}

QString getName(const QString& filePath) {
  QString name{""};
  const char separator{'/'};
  const std::string base = filePath.toStdString();
  for (size_t i = base.size() - 1; base[i] != separator; --i) {
    name.push_front(base[i]);
  }
  return name;
}

QString getFilePath(const QString& absolutePath) {
  QString path{""};
  const QString name = getName(absolutePath);
  const size_t size = name.size();
  for (int i = absolutePath.size() - size - 1; i >= 0; --i) {
    path.push_front(absolutePath[i]);
  }
  return path;
}

}  // namespace
