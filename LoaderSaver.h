#ifndef LOADERSAVER_H
#define LOADERSAVER_H
#include "Matrix3D.h"

class LoaderSaver {
private:
    short toDigit(const char *str);
    void putBorders(short minX, short maxX, short minY, short maxY, short minZ, short maxZ, FILE* pFile);
public:
    LoaderSaver();
    void save(Matrix3D* matrix3D, QString filename);
    void load(Matrix3D* matrix3D, QString filename);

    ~LoaderSaver();
};

#endif // LOADERSAVER_H

