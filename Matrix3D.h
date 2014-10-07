#ifndef MATRIX3D_H
#define MATRIX3D_H
#include <QGLWidget>
#include "MatrixData.h"

class Matrix3D {
private:
    bool ***array;
    short sizeValidator(short coordinate1, short coordinate2);
    bool operation(short operationIndex, short x, short y, short z);
public:
    unsigned short int height;
    unsigned short int width;
    unsigned short int depth;

    unsigned short int heightShift;
    unsigned short int widthShift;
    unsigned short int depthShift;

    short minX;
    short minY;
    short minZ;

    short maxX;
    short maxY;
    short maxZ;

    Matrix3D(unsigned short height, unsigned short width, unsigned short depth);
    virtual ~Matrix3D();

    void clear();
    void setDefaultBorders();
    void setBorders(short x1, short x2, short y1, short y2, short z1, short z2);
    void set(unsigned short x, unsigned short y, unsigned short z, bool value);
    bool get(unsigned short x, unsigned short y, unsigned short z);
    void updateMatrix(MatrixData *matrixData);
    void prizm     (short x1Length, short x2Length,
                    short y1Width,  short y2Width,
                    short z1Height, short z2Height, short operationIndex);
    void ellipsoid (short x1Length, short x2Length,
                    short y1Width,  short y2Width,
                    short z1Height, short z2Height, short operationIndex);
    void cone      (short x1Length, short x2Length,
                    short y1Width,  short y2Width,
                    short z1Height, short z2Height, short operationIndex);
};

#endif // MATRIX3D_H
