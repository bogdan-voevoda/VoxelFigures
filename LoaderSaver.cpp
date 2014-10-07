#include "LoaderSaver.h"
#define ASCII_SHIFT 48

LoaderSaver::LoaderSaver() {}

void LoaderSaver::save(Matrix3D* matrix3D, QString filename) {
    const char* tmp =  filename.toUtf8().constData();
    FILE* pFile;
    pFile = fopen (tmp, "w");

    if ( pFile ) {

        putBorders(matrix3D->minX, matrix3D->maxX,
                   matrix3D->minY, matrix3D->maxY,
                   matrix3D->minZ, matrix3D->maxZ, pFile);

        int x = matrix3D->minX + matrix3D->heightShift;
        int xBorder = matrix3D->maxX + matrix3D->heightShift;
        int y = matrix3D->minY + matrix3D->widthShift;
        int yBorder = matrix3D->maxY + matrix3D->widthShift;
        int z = matrix3D->minZ + matrix3D->depthShift;
        int zBorder = matrix3D->maxZ + matrix3D->depthShift;

        for ( short first = x; first <= xBorder; first++) {
            for (short second = y; second <= yBorder; second++) {
                for (short third = z; third <= zBorder; third++) {
                    if( matrix3D->get(first, second, third) ) { fputc ('1', pFile); }
                    else { fputc ('0', pFile); }
                }
            }
        }
        fclose(pFile);
    }
}

void LoaderSaver::load(Matrix3D* matrix3D, QString filename) {
    const char* tmp =  filename.toUtf8().constData();
    FILE* pFile;
    pFile = fopen (tmp , "r");
    char temp = 0;

    if ( pFile ) {
        std::string str;
        std::string sx1;
        std::string sx2;
        std::string sy1;
        std::string sy2;
        std::string sz1;
        std::string sz2;

        while ( temp != '\n' ) {
            temp = fgetc(pFile);
            str.push_back(temp);
        }

        unsigned short i = 0;

        while ( str[i] != ';' ) { sx1.push_back(str[i]); i++; }
        i++;
        while ( str[i] != ';' ) { sx2.push_back(str[i]); i++; }
        i++;
        while ( str[i] != ';' ) { sy1.push_back(str[i]); i++; }
        i++;
        while ( str[i] != ';' ) { sy2.push_back(str[i]); i++; }
        i++;
        while ( str[i] != ';' ) { sz1.push_back(str[i]); i++; }
        i++;
        while ( str[i] != ';' ) { sz2.push_back(str[i]); i++; }

        short x1 = toDigit(sx1.data());
        short x2 = toDigit(sx2.data());
        short y1 = toDigit(sy1.data());
        short y2 = toDigit(sy2.data());
        short z1 = toDigit(sz1.data());
        short z2 = toDigit(sz2.data());

        matrix3D->setBorders(x1,x2,y1,y2,z1,z2);

        for (short x = x1; x <= x2; x += 1 ) {
            for(short y = y1; y <= y2; y += 1 ) {
                for(short z = z1; z <= z2; z += 1 ) {
                    temp = fgetc(pFile);
                    if( temp == '1' ) { matrix3D->set(x+matrix3D->heightShift,
                                                      y+matrix3D->widthShift,
                                                      z+matrix3D->depthShift, true); }
                }
            }
        }
        fclose(pFile);
    }
}

short LoaderSaver::toDigit(const char *str) {
    bool minus = false;
    short result = 0;
    short i = 0;

    if ( *str == '-' ) { minus = true; i = 1; }
    for ( ; *(str+i) != '\0'; i++ ) {
        if ( *(str+i) >= '0' && *(str+i) <= '9' ) {
            result = result*10 + (short)*(str+i) - ASCII_SHIFT;
        } else { return 0; }
    }
    if ( minus ) { result *= -1; }
    return result;
}

void LoaderSaver::putBorders(short minX, short maxX, short minY, short maxY, short minZ, short maxZ, FILE* pFile) {
    fprintf(pFile, "%d", minX);
    fputc (';', pFile);
    fprintf(pFile, "%d", maxX);
    fputc (';', pFile);
    fprintf(pFile, "%d", minY);
    fputc (';', pFile);
    fprintf(pFile, "%d", maxY);
    fputc (';', pFile);
    fprintf(pFile, "%d", minZ);
    fputc (';', pFile);
    fprintf(pFile, "%d", maxZ);
    fputc (';', pFile);
    fputc ('\n', pFile);
}

LoaderSaver::~LoaderSaver() {}
