#include "mainwidget.h"
#include <QMouseEvent>
#include <math.h>

GLfloat MainWidget::xRot = -45;
GLfloat MainWidget::yRot = 0;
GLfloat MainWidget::zRot = -45;
GLfloat MainWidget::zTra = 0;
GLfloat MainWidget::nSca = 0.07;

MainWidget::MainWidget(MainWindow *parent, GeometryEngine *geometries)
    : parent(parent), geometries(geometries) {}

MainWidget::~MainWidget() {}

void MainWidget::mousePressEvent(QMouseEvent* pe) { ptrMousePosition = pe->pos(); }

void MainWidget::mouseReleaseEvent(QMouseEvent* pe) {}

void MainWidget::mouseMoveEvent(QMouseEvent* pe) {
    xRot += 180*(GLfloat)(pe->y()-ptrMousePosition.y())/height();
    zRot += 180*(GLfloat)(pe->x()-ptrMousePosition.x())/width();
    ptrMousePosition = pe->pos();

    parent->updateGLViews();
}

void MainWidget::wheelEvent(QWheelEvent* pe) {
    if ( (pe->delta()) > 0 ) scale_plus();
    else if ( (pe->delta()) < 0 ) scale_minus();

    parent->updateGLViews();
}

void MainWidget::scale_plus() { nSca = nSca*1.1; }

void MainWidget::scale_minus() { nSca = nSca/1.1; }

void MainWidget::initializeGL() {
    initializeGLFunctions();
    qglClearColor(Qt::black);
    initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    geometries->init();
}

void MainWidget::initShaders() {
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl")) {
        parent->close();
    }
    if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl")) {
        parent->close();
    }
    if (!program.link()) {
        parent->close();
    }
    if (!program.bind()) {
        parent->close();
    }
}

void MainWidget::resizeGL(int nWidth, int nHeight) {
    static GLdouble aspect = (GLdouble) nWidth / nHeight;
    const GLdouble pi = 3.14;

    GLdouble horizontal, vertical;
    GLdouble _near = 0.001;
    GLdouble _far = 100;
    GLdouble fovY = 30;

    glViewport(0, 0, nWidth, nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    vertical = tan( fovY / 360 * pi ) * _near;
    horizontal = vertical * aspect;

    glFrustum(-horizontal, horizontal, -vertical, vertical, _near, _far);
    glMatrixMode(GL_MODELVIEW);
}

void MainWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;

    matrix.scale(nSca, nSca, nSca);
    matrix.translate(0.0f, zTra, 0.0f);
    matrix.rotate(xRot, 1.0f, 0.0f, 0.0f);
    matrix.rotate(yRot, 0.0f, 1.0f, 0.0f);
    matrix.rotate(zRot, 0.0f, 0.0f, 1.0f);

    program.setUniformValue("mvp_matrix", projection * matrix );
    geometries->drawGeometry(&program);
}

