#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"

#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <mainwindow.h>
#include <QGLShaderProgram>

class QGLShaderProgram;
class GeometryEngine;
class MainWindow;

class MainWidget : public QGLWidget, protected QGLFunctions {
    Q_OBJECT

public:
    explicit MainWidget(MainWindow *parent = 0, GeometryEngine *geometries = 0);
    ~MainWidget();

protected:
    void initializeGL();
    void initShaders();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);
    void scale_plus();
    void scale_minus();
private:
    static GLfloat xRot;
    static GLfloat yRot;
    static GLfloat zRot;
    static GLfloat zTra;
    static GLfloat nSca;
    MainWindow *parent;
    GeometryEngine* geometries;
    QGLShaderProgram program;
    QMatrix4x4 projection;
    QPoint ptrMousePosition;
};

#endif // MAINWIDGET_H
