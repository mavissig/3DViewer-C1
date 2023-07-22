#include "../headers/viewer.h"

Viewer::Viewer(QWidget* parent)
    : QOpenGLWidget(parent),
      vertex_(std::move(new vertex_s())),
      surface_(std::move(new surface_s())) {
  defaultScene();
  this->colorArray_ = nullptr;
  this->colorPointsArray_ = nullptr;
  this->maxX_ = this->maxY_ = this->maxZ_ = 1;
}

void Viewer::initializeGL() {
  glClearColor(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glDisable(GL_MULTISAMPLE);
}

void Viewer::resizeGL(const int nWidth, const int nHeight) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const GLfloat ratio =
      static_cast<GLfloat>(nHeight) / static_cast<GLfloat>(nWidth);
  float left = 0, right = 0, bottom = 0, top = 0, near = 0, far = 0;
  if (nWidth >= nHeight) {
    left = -1.0 / ratio;
    right = 1.0 / ratio;
    bottom = -1.0;
    top = 1.0;
    near = -10.0;
    far = 1.0;
  } else {
    left = -1.0;
    right = 1.0;
    bottom = -1.0 * ratio;
    top = 1.0 * ratio;
    near = -10.0;
    far = 1.0;
  }
  if (this->projectionType_) {
    glFrustum(left, right, bottom, top, near, far);
  } else {
    glOrtho(left, right, bottom, top, near, far);
  }
  glViewport(0, 0, nWidth, nHeight);
}

void Viewer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(this->nSca_, this->nSca_, this->nSca_);
  glTranslatef(0.0f, this->zTra_, 0.0f);
  glRotatef(this->xRot_, 1.0f, 0.0f, 0.0f);
  glRotatef(this->yRot_, 0.0f, 1.0f, 0.0f);
  glRotatef(this->zRot_, 0.0f, 0.0f, 1.0f);
  if (this->axisVisible_) {
    drawAxis();
  }
  glClearColor(this->backgroundColor_.redF(), this->backgroundColor_.greenF(),
               this->backgroundColor_.blueF(), this->backgroundColor_.alphaF());
  if (nullptr != this->vertex_->coords && nullptr != this->surface_->points &&
      nullptr != this->colorArray_) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(this->lineWidth_);
    if (this->lineType_) {
      glLineStipple(4, 0xAAAA);
      glEnable(GL_LINE_STIPPLE);
    }
    glVertexPointer(3, GL_FLOAT, 0, this->vertex_->coords);
    glColorPointer(3, GL_FLOAT, 0, this->colorArray_);
    glDrawElements(GL_TRIANGLES, this->surface_->size, GL_UNSIGNED_INT,
                   this->surface_->points);
    if (this->pointType_) {
      drawVertex();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    if (this->lineType_) {
      glDisable(GL_LINE_STIPPLE);
    }
  }
}

void Viewer::drawVertex() const {
  if (1 == this->pointType_) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  }
  glPointSize(this->pointSize_);
  glColorPointer(3, GL_FLOAT, 0, this->colorPointsArray_);
  glDrawArrays(GL_POINTS, 0, this->vertex_->size / 3);
  if (1 == this->pointType_) {
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_MULTISAMPLE);
  }
}

void Viewer::mousePressEvent(QMouseEvent* event) {
  ptrMousePosition_ = event->pos();
}

void Viewer::mouseMoveEvent(QMouseEvent* event) {
  this->xRot_ += 10 / nSca_ *
                 static_cast<GLfloat>(event->y() - ptrMousePosition_.y()) /
                 this->height();
  this->zRot_ += 10 / nSca_ *
                 static_cast<GLfloat>(event->x() - ptrMousePosition_.x()) /
                 this->width();
  ptrMousePosition_ = event->pos();
  this->repaint();
}

vertex_s* Viewer::getVertex() const noexcept { return this->vertex_; }

surface_s* Viewer::getSurface() const noexcept { return this->surface_; }

void Viewer::wheelEvent(QWheelEvent* event) {
  if ((event->angleDelta().ry()) > 0)
    scale_plus();
  else if ((event->angleDelta().ry()) < 0)
    scale_minus();
  this->repaint();
}

void inline Viewer::scale_plus() noexcept { this->nSca_ *= 1.1; }

void inline Viewer::scale_minus() noexcept { this->nSca_ /= 1.1; }

void Viewer::drawAxis() const {
  glLineWidth(3.0f);
  glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
  glBegin(GL_LINES);
  glVertex3f(this->maxX_ * 2, 0.0f, 0.0f);
  glVertex3f(-this->maxX_ * 2, 0.0f, 0.0f);
  glEnd();
  glBegin(GL_LINES);
  glColor4f(0.00f, 1.00f, 0.00f, 1.0f);
  glVertex3f(0.0f, this->maxY_ * 2, 0.0f);
  glVertex3f(0.0f, -this->maxY_ * 2, 0.0f);
  glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
  glVertex3f(0.0f, 0.0f, this->maxZ_ * 2);
  glVertex3f(0.0f, 0.0f, -this->maxZ_ * 2);
  glEnd();
}

void inline Viewer::defaultScene() noexcept {
  this->xRot_ = -90;
  this->yRot_ = 0;
  this->zRot_ = 0;
  this->zTra_ = 0;
  this->nSca_ = 1;
}

void Viewer::setVertexArray(vertex_s&& vertex) {
  delete[] this->vertex_->coords;
  this->vertex_->size = vertex.size;
  this->vertex_->coords = vertex.coords;
  vertex.coords = nullptr;
  vertex.size = 0;
  vertex.capacity = 0;
  this->setLineColor(this->linesColor_);
  if (this->pointType_) {
    this->setPointColor(this->pointColor_);
  }
  const int coeffX = (0 == this->maxX_);
  const int coeffY = (0 == this->maxY_);
  const int coeffZ = (0 == this->maxZ_);
  this->nSca_ = 1.0 / sqrt((this->maxX_ + coeffX) * (this->maxY_ + coeffY) *
                           (this->maxZ_ + coeffZ));
}

void Viewer::setLineColor(const QColor& color) {
  delete[] this->colorArray_;
  this->linesColor_ = color;
  this->colorArray_ = new float[this->vertex_->size]();
  const float red = color.redF();
  const float green = color.greenF();
  const float blue = color.blueF();
  for (size_t i = 0; i < this->vertex_->size; i += 3) {
    this->colorArray_[i] = red;
    this->colorArray_[i + 1] = green;
    this->colorArray_[i + 2] = blue;
  }
}

void Viewer::setMaxCoords(float&& x, float&& y, float&& z) noexcept {
  this->maxX_ = x;
  this->maxY_ = y;
  this->maxZ_ = z;
  x = 0;
  y = 0;
  z = 0;
}

void Viewer::setIndexArray(surface_s&& surface) {
  delete[] this->surface_->points;
  this->surface_->size = surface.size;
  this->surface_->points = surface.points;
  surface.points = nullptr;
  surface.size = 0;
  surface.capacity = 0;
}

void Viewer::setPointColor(const QColor& color) {
  delete[] this->colorPointsArray_;
  this->pointColor_ = color;
  this->colorPointsArray_ = new float[this->vertex_->size]();
  const float red = color.redF();
  const float green = color.greenF();
  const float blue = color.blueF();
  for (size_t i = 0; i < this->vertex_->size; i += 3) {
    this->colorPointsArray_[i] = red;
    this->colorPointsArray_[i + 1] = green;
    this->colorPointsArray_[i + 2] = blue;
  }
}

void Viewer::setBackgroundColor(const QColor& color) {
  this->backgroundColor_ = color;
}

void Viewer::saveImage(const QString& filename) {
  const QStringList sufix = filename.split(".");
  const QImage render = grabFramebuffer();
  if (sufix.back() == "jpg" || sufix.back() == "jpeg" ||
      sufix.back() == "JPG" || sufix.back() == "JPEG") {
    render.save(filename, "JPEG");
  } else if (sufix.back() == "bmp" || sufix.back() == "BMP") {
    render.save(filename, "BMP");
  }
}

void Viewer::showAxis() noexcept { this->axisVisible_ = true; }

void Viewer::hideAxis() noexcept { this->axisVisible_ = false; }

QColor Viewer::getLineColor() const noexcept { return this->linesColor_; }

QColor Viewer::getPointColor() const noexcept { return this->pointColor_; }

QColor Viewer::getBackgroundColor() const noexcept {
  return this->backgroundColor_;
}

float Viewer::getLineWidth() const noexcept { return this->lineWidth_; }

float Viewer::getPointSize() const noexcept { return this->pointSize_; }

int Viewer::getPointType() const noexcept { return this->pointType_; }

bool Viewer::getLineType() const noexcept { return this->lineType_; }

bool Viewer::getProjectionType() const noexcept {
  return this->projectionType_;
}

void Viewer::setLineType(const bool& type) noexcept { this->lineType_ = type; }

void Viewer::setPointType(const int& type) noexcept { this->pointType_ = type; }

void Viewer::setPointSize(const float& size) noexcept {
  this->pointSize_ = size;
}

void Viewer::setLineWidth(const float& width) noexcept {
  this->lineWidth_ = width;
}

void Viewer::setProjectionType(const bool& type) noexcept {
  this->projectionType_ = type;
}

bool Viewer::isInit() const noexcept {
  return this->colorPointsArray_ != nullptr;
}

void Viewer::resizeWindow() {
  this->resizeGL(this->grab().width(), this->grab().height());
}

void Viewer::setOffsetX(const float& offset) noexcept {
  this->offsetX_ += offset;
}

void Viewer::setOffsetY(const float& offset) noexcept {
  this->offsetY_ += offset;
}

void Viewer::setOffsetZ(const float& offset) noexcept {
  this->offsetZ_ += offset;
}

float Viewer::getOffsetX() const noexcept { return this->offsetX_; }

float Viewer::getOffsetY() const noexcept { return this->offsetY_; }

float Viewer::getOffsetZ() const noexcept { return this->offsetZ_; }

void Viewer::setDefaultOffset() noexcept {
  this->offsetX_ = 0.0;
  this->offsetY_ = 0.0;
  this->offsetZ_ = 0.0;
}

void Viewer::changeVertexArray(
    std::function<float(const float&, const float&)> summator) {
  for (size_t i = 0; i < this->vertex_->size; i += 3) {
    this->vertex_->coords[i] =
        summator(this->vertex_->coords[i], this->offsetX_);
    this->vertex_->coords[i + 1] =
        summator(this->vertex_->coords[i + 1], this->offsetY_);
    this->vertex_->coords[i + 2] =
        summator(this->vertex_->coords[i + 2], this->offsetZ_);
  }
}

void Viewer::setAngleX(const float& angle) noexcept { this->angleX_ += angle; }

void Viewer::setAngleY(const float& angle) noexcept { this->angleY_ += angle; }

void Viewer::setAngleZ(const float& angle) noexcept { this->angleZ_ += angle; }

float Viewer::getAngleX() const noexcept { return this->angleX_; }

float Viewer::getAngleY() const noexcept { return this->angleY_; }

float Viewer::getAngleZ() const noexcept { return this->angleZ_; }

void Viewer::setDefaultAngle() noexcept {
  this->angleX_ = 0.0;
  this->angleY_ = 0.0;
  this->angleZ_ = 0.0;
}

Viewer::~Viewer() {
  delete[] this->colorArray_;
  this->colorArray_ = nullptr;

  delete[] this->colorPointsArray_;
  this->colorPointsArray_ = nullptr;

  delete[] this->vertex_->coords;
  this->vertex_->coords = nullptr;

  delete this->vertex_;
  this->vertex_ = nullptr;

  delete[] this->surface_->points;
  this->surface_->points = nullptr;

  delete this->surface_;
  this->surface_ = nullptr;
}
