#ifndef MATRIXDATA_H
#define MATRIXDATA_H

struct MatrixData {
    short operation;
    short figure;
    short x1;
    short x2;
    short y1;
    short y2;
    short z1;
    short z2;
    short strNumber;

    MatrixData(short op, short fig, short X1, short X2, short Y1, short Y2, short Z1, short Z2, short strN) {
        operation = op;
        figure = fig;
        x1 = X1;
        x2 = X2;
        y1 = Y1;
        y2 = Y2;
        z1 = Z1;
        z2 = Z2;
        strNumber = strN;
    }
    void changeData(MatrixData *array) {
        operation = array->operation;
        figure = array->figure;
        x1 = array->x1;
        x2 = array->x2;
        y1 = array->y1;
        y2 = array->y2;
        z1 = array->z1;
        z2 = array->z2;
        strNumber = array->strNumber;
    }
    ~MatrixData() { }
};

#endif // MATRIXDATA_H
