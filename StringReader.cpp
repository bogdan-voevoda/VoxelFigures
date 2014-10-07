#include "StringReader.h"
#define CLEAR_FIELD 1,2,1,1,1,1,1,1
#define ASCII_SHIFT 48

StringReader::StringReader() {
    strList = new QStringList;
}

void StringReader::reciever(QString *str) {
    *strList = str->split("\n");

    int strtingQuantity = strList->size();

    for (unsigned short i = 0; i < strtingQuantity; i++) {
        QString str = strList->at(i);

        if ( str.size() >= 18 ) {
            analyser(str, i);
        } else { interpreter(CLEAR_FIELD, i); }
    }
}

void StringReader::analyser(QString &strng, short numberOfStr) {
    bool flag = true;

    int tempSize = strng.size();

    const char* str =  strng.toUtf8().constData();

    int j = 0;
    int k = 0;

    char operation[tempSize];

    while ( *(str+j) == ' ' ) { j++; }
    for(  ; *(str+j) != ' ' && *(str+j) != '\0'; j++, k++ ) {
        *(operation+k) = *(str+j);
    }
    *(operation+k) = '\0';
    this->trim(operation);
    val_type operationIndex = this->operation(operation, flag);

    if (flag) {
        char figureType[tempSize-j];
        k = 0;

        for( ; *(str+j) != '(' && *(str+j) != '\0'; j++, k++) {
            *(figureType+k) = *(str+j);
        }
        *(figureType+k) = '\0';
        this->trim(figureType);
        val_type figureIndex = this->figure(figureType, flag);
        j += 1;

        if (flag) {
            char x1[tempSize-j];
            k = 0;

            for(  ; *(str+j) != ',' && *(str+j) != '\0'; j++, k++ ) {
                *(x1+k) = *(str+j);
            }
            *(x1+k) = '\0';
            this->trim(x1);
            val_type x1Value = this->coordinate(x1, flag);
            j += 1;

            if(flag) {
                char x2[tempSize-j];
                k = 0;

                for(  ; *(str+j) != ',' && *(str+j) != '\0'; j++, k++ ) {
                    *(x2+k) = *(str+j);
                }
                *(x2+k) = '\0';
                this->trim(x2);
                val_type x2Value = this->coordinate(x2, flag);
                j += 1;

                if(flag) {
                    char y1[tempSize-j];
                    k = 0;

                    for(  ; *(str+j) != ',' && *(str+j) != '\0'; j++, k++ ) {
                        *(y1+k) = *(str+j);
                    }
                    *(y1+k) = '\0';
                    this->trim(y1);
                    val_type y1Value = this->coordinate(y1, flag);
                    j += 1;

                    if(flag) {
                        char y2[tempSize-j];
                        k = 0;

                        for(  ; *(str+j) != ',' && *(str+j) != '\0'; j++, k++ ) {
                            *(y2+k) = *(str+j);
                        }
                        *(y2+k) = '\0';
                        this->trim(y2);
                        val_type y2Value = this->coordinate(y2, flag);
                        j += 1;

                        if(flag) {
                            char z1[tempSize-j];
                            k = 0;

                            for(  ; *(str+j) != ',' && *(str+j) != '\0'; j++, k++ ) {
                                *(z1+k) = *(str+j);
                            }
                            *(z1+k) = '\0';
                            this->trim(z1);
                            val_type z1Value = this->coordinate(z1, flag);
                            j += 1;

                            if(flag) {
                                char z2[tempSize-j];
                                k = 0;

                                for(  ; *(str+j) != ')' && *(str+j) != '\0'; j++, k++ ) {
                                    *(z2+k) = *(str+j);
                                }
                                *(z2+k) = '\0';
                                this->trim(z2);
                                val_type z2Value = this->coordinate(z2, flag);
                                j += 1;

                                if(flag) {
                                    char comma[tempSize-j];
                                    k = 0;

                                    for(  ; *(str+j) != ')' && *(str+j) != '\0'; j++, k++) {
                                        *(comma+k) = *(str+j);
                                    }
                                    *(comma+k) = '\0';
                                    this->trim(comma);

                                    if ( comma[0] == ';' && comma[1] == '\0' ) {
                                        interpreter(operationIndex, figureIndex,
                                                   x1Value, x2Value,y1Value,
                                                   y2Value,z1Value, z2Value, numberOfStr);
                                    } else { flag = false; }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ( !flag && numberOfStr >=0 ) {
        interpreter(CLEAR_FIELD, numberOfStr);
    }
}

void StringReader::swap(val_type *p1, val_type *p2) {
    val_type temp = *p1;

    *p1 = *p2;
    *p2 = temp;
}

void StringReader::trim(char *str) {
    unsigned short strSize = 0;
    unsigned short tempSize = 0;

    while ( *(str+strSize) != '\0' ) { strSize++; }
    char tmp[strSize];

    for ( unsigned short i = 0; *(str+i) != '\0'; i++ ) {
        if ( *(str+i) != ' ' ) {
            *(tmp+tempSize) = *(str+i);
            tempSize++;
        }
    }

    for ( unsigned short i = 0; i < tempSize; i++ ) {
        *(str+i) = *(tmp+i);
    }
    *(str+tempSize) = '\0';
}

val_type StringReader::operation(const char *str , bool &flag) {
    unsigned short strSize = 0;

    while ( *(str+strSize) != '\0' ) { strSize++; }

    if ( strSize > 3 ) {
        flag = false;
        return -1;
    }

    if      (*str == 'a' && *(str+1) == 'n' && *(str+2) == 'd')  { return 0; }
    else if (*str == 'o' && *(str+1) == 'r' && *(str+2) == '\0') { return 1; }
    else if (*str == 'x' && *(str+1) == 'o' && *(str+2) == 'r')  { return 2; }
    else if (*str == 'n' && *(str+1) == 'o' && *(str+2) == 't')  { return 3; }
    else    { flag = false; return -1; }
}

val_type StringReader::figure(char* ch , bool &flag) {
    if      ( *ch == 'E' && *(ch+1) == '\0' ) { return 0; }
    else if ( *ch == 'P' && *(ch+1) == '\0' ) { return 1; }
    else if ( *ch == 'C' && *(ch+1) == '\0' ) { return 2; }
    else    { flag = false; return -1; }
}


val_type StringReader::coordinate(const char *str , bool &flag) {
    if ( *str == '\0'                    ) { flag = false; return 0; }
    if ( *str == '-' && *(str+1) == '\0' ) { flag = false; return 0; }
    if ( *str == '0' && *(str+1) != '\0' ) { flag = false; return 0; }
    if ( *str == '-' && *(str+1) == '0'  ) { flag = false; return 0; }

    bool minus = false;
    val_type result = 0;
    val_type i = 0;

    if ( *str == '-' ) { minus = true; i = 1; }
    for ( ; *(str+i) != '\0'; i++ ) {
        if ( *(str+i) >= '0' && *(str+i) <= '9' ) {
            result = result*10 + (val_type)*(str+i) - ASCII_SHIFT;
        } else { flag = false; return 0; }
    }
    if ( minus ) { result *= -1; }
    return result;
}

void StringReader::interpreter(val_type opType, val_type figType, val_type x1, val_type x2,
                              val_type y1,     val_type y2,      val_type z1, val_type z2,
                              val_type stringNumber) {
    MatrixData *tmpData = new MatrixData(opType,figType,x1,x2,y1,y2,z1,z2,stringNumber);
    emit updateGeometry(tmpData);
    delete tmpData;
}


StringReader::~StringReader() {
    delete strList;
}
