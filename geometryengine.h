#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QGLFunctions>
#include <QGLShaderProgram>
#include "mainwindow.h"
#include "Matrix3D.h"

class MainWindow;

class GeometryEngine : public QObject, protected QGLFunctions {
    Q_OBJECT
public:
    GeometryEngine(MainWindow *parent);
    virtual ~GeometryEngine();

    void init();
    void drawGeometry(QGLShaderProgram *program);
    void getVertices(Matrix3D *field,
                     std::vector<GLfloat> &vertices,
                     std::vector<GLuint> &indices);

    void addVertices(float x, float y, float z, int indexer,
                     std::vector<GLfloat> &vertices,
                     std::vector<GLuint> &indices);
    void initGeometry();
    void updateVertices();
    Matrix3D* getMatrix();

private:
    QMap <short, MatrixData*> matData;
    MainWindow *parent;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    Matrix3D* field = NULL;
    GLuint vboIds[2];
    bool isComparable(MatrixData *arr, MatrixData *data);

public slots:
    void update_Geometry(MatrixData *matrixData);
};

#endif // GEOMETRYENGINE_H
