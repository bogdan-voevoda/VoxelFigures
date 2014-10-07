#include "Matrix3D.h"
#define AND 0
#define OR  1
#define XOR 2
#define NOT 3

#define ELLIPSE 0
#define PRIZM   1
#define CONE    2

Matrix3D::Matrix3D(unsigned short height, unsigned short width, unsigned short depth) {
    if ( height%2 != 0 ) { height+=1; }
    if ( width %2 != 0 ) { width +=1; }
    if ( depth %2 != 0 ) { depth +=1; }

    this->height = height;
    this->width = width;
    this->depth = depth;

    this->heightShift = height/2;
    this->widthShift = width/2;
    this->depthShift = depth/2;

    array = new bool**[height];

    for ( int i = 0; i < height ; i++) {
        array[i] = new bool*[width];

        for ( int j = 0; j < width ; j++) {
            array[i][j] = new bool[depth];
        }
    }

    for(unsigned short x = 0; x < height; x++) {
        for(unsigned short y = 0; y < width; y++) {
            for(unsigned short z = 0; z < depth; z++) {
                this->array[x][y][z] = false;
            }
        }
    }

    setDefaultBorders();
}

void Matrix3D::setDefaultBorders() {
    minX = heightShift;
    minY = widthShift;
    minZ = depthShift;

    maxX = -heightShift;
    maxY = -widthShift;
    maxZ = -depthShift;
}

void Matrix3D::clear() {
    unsigned short xBorder = maxX+heightShift+1;
    for(unsigned short x = minX+heightShift; x < xBorder; x++) {
        unsigned short yBorder = maxY+widthShift+1;
        for(unsigned short y = minY+widthShift; y < yBorder; y++) {
            unsigned short zBorder = maxZ+depthShift+1;
            for(unsigned short z = minZ+depthShift; z < zBorder; z++) {
                this->array[x][y][z] = false;
            }
        }
    }
}

void Matrix3D::set(unsigned short x, unsigned short y, unsigned short z, bool value) {
    this->array[x][y][z] = value;
}

bool Matrix3D::get(unsigned short x, unsigned short y, unsigned short z) {
    return array[x][y][z];
}


bool Matrix3D::operation(short operationIndex, short x, short y, short z) {
    if      ( operationIndex == AND ) { return (this->array[x][y][z] & true); }
    else if ( operationIndex == OR  ) { return (this->array[x][y][z] | true); }
    else if ( operationIndex == XOR ) { return (this->array[x][y][z] ^ true); }
    else if ( operationIndex == NOT ) { return false; }
}

void Matrix3D::prizm(short x1Length, short x2Length,
                     short y1Width,  short y2Width,
                     short z1Height, short z2Height, short operationIndex) {

    setBorders(x1Length, x2Length, y1Width, y2Width, z1Height, z2Height);

    if ( sizeValidator(x1Length, x2Length) == 1 ) { x2Length -= 1; }
    if ( sizeValidator(y1Width ,  y2Width) == 1 ) { y2Width  -= 1; }
    if ( sizeValidator(z1Height, z2Height) == 1 ) { z2Height -= 1; }

    for (short x = x1Length; x <= x2Length; x++) {
        for (short y = y1Width; y <= y2Width; y++) {
            for (short z = z1Height; z <= z2Height; z++) {
                unsigned short X = x+this->heightShift;
                unsigned short Y = y+this->widthShift;
                unsigned short Z = z+this->depthShift;


                this->array[X][Y][Z] = operation(operationIndex, X, Y, Z);
            }
        }
    }
}

void Matrix3D::ellipsoid(short x1Length, short x2Length,
                         short y1Width,  short y2Width,
                         short z1Height, short z2Height, short operationIndex) {

    setBorders(x1Length, x2Length, y1Width, y2Width, z1Height, z2Height);

    GLfloat len = this->sizeValidator(x1Length, x2Length);
    GLfloat wid = this->sizeValidator(y1Width, y2Width);
    GLfloat height = this->sizeValidator(z1Height, z2Height);

    GLfloat xShift = (x1Length + x2Length)/2;
    GLfloat yShift = (y1Width + y2Width)/2;
    GLfloat zShift = (z1Height + z2Height)/2;

    GLfloat a = len/2;
    GLfloat b = wid/2;
    GLfloat c = height/2;

    a *= a;
    b *= b;
    c *= c;

    for (short x = -len; x < len; x++) {
        for (short y = -wid; y < wid; y++) {
            for (short z = -height; z < height; z++) {
                GLfloat temp = (x*x)/a + (y*y)/b + (z*z)/c;

                if ( temp <= 1.04 ) {
                    unsigned short X = x+this->heightShift+xShift;
                    unsigned short Y = y+this->widthShift+yShift;
                    unsigned short Z = z+this->depthShift+zShift;

                    this->array[X][Y][Z] = operation(operationIndex, X, Y, Z);
                }
            }
        }
    }
}

void Matrix3D::cone(short x1Length, short x2Length,
                    short y1Width,  short y2Width,
                    short z1Height, short z2Height, short operationIndex) {

    setBorders(x1Length, x2Length, y1Width, y2Width, z1Height, z2Height);

    GLfloat len = this->sizeValidator(x1Length, x2Length)/2;
    GLfloat wid = this->sizeValidator(y1Width, y2Width)/2;
    GLfloat height = this->sizeValidator(z1Height, z2Height)/2;

    GLfloat xShift = (x1Length + x2Length)/2;
    GLfloat yShift = (y1Width + y2Width)/2;
    GLfloat zShift = (z1Height + z2Height)/2;

    GLfloat a = len/2;
    GLfloat b = wid/2;
    GLfloat c = height/2;

    a *= a;
    b *= b;
    c *= c;
    height *= 2;

    for (short x = -len; x <= len; x++) {
        for (short y = -wid; y <= wid; y++) {
            for (short z = -height; z <= 0; z++) {
                GLdouble temp = (x*x)/a + (y*y)/b - (z*z)/c/4;

                if ( temp <= 0 || (z == -height && temp <= 0.05) ) {
                    unsigned short X = x+this->heightShift+xShift;
                    unsigned short Y = y+this->widthShift+yShift;
                    unsigned short Z = z+this->depthShift+zShift+height/2;

                    this->array[X][Y][Z] = operation(operationIndex, X, Y, Z);
                }
            }
        }
    }
}

short Matrix3D::sizeValidator(short coordinate1, short coordinate2) {
    short size;

    if ( coordinate1 >= 0 && coordinate2 >= 0 ) {
        size = (abs(coordinate2) - abs(coordinate1));
    } else if ( coordinate1 < 0 && coordinate2 < 0 ) {
        size = (abs(coordinate1) - abs(coordinate2));
    } else {
        size = (abs(coordinate1) + abs(coordinate2));
    }
    return size;
}


void Matrix3D::updateMatrix(MatrixData *matrixData) {
    if ( matrixData->figure == ELLIPSE ) {
        this->ellipsoid(matrixData->x1, matrixData->x2,
                        matrixData->y1, matrixData->y2,
                        matrixData->z1, matrixData->z2, matrixData->operation);
    } else if ( matrixData->figure == PRIZM ) {
        this->prizm(matrixData->x1, matrixData->x2,
                    matrixData->y1, matrixData->y2,
                    matrixData->z1, matrixData->z2, matrixData->operation);
    } else if ( matrixData->figure == CONE ) {
        this->cone(matrixData->x1, matrixData->x2,
                   matrixData->y1, matrixData->y2,
                   matrixData->z1, matrixData->z2, matrixData->operation);
    }
}

void Matrix3D::setBorders(short x1, short x2, short y1, short y2, short z1, short z2) {
    maxX = std::max((int)maxX, (int)x2);
    maxY = std::max((int)maxY, (int)y2);
    maxZ = std::max((int)maxZ, (int)z2);

    minX = std::min((int)minX, (int)x1);
    minY = std::min((int)minY, (int)y1);
    minZ = std::min((int)minZ, (int)z1);
}


Matrix3D::~Matrix3D() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            delete [] array[i][j];
        }
        delete [] array[i];
    }
    delete [] array;
}
