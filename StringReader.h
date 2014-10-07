#ifndef STRINGREADER_H
#define STRINGREADER_H

#include <QObject>
#include "Matrix3D.h"
#include "mainwindow.h"
#include <QString>
#include "MatrixData.h"

class MainWindow;
typedef int val_type;

class StringReader : public QObject {
    Q_OBJECT
private:
    QStringList* strList;

    void swap(val_type *p1, val_type *p2);
    void trim(char *str);
    val_type operation(const char *str , bool &flag);
    val_type figure(char* ch , bool &flag);
    val_type coordinate(const char *str , bool &flag);
    void analyser(QString &strng, short numberOfStr);

public:
    StringReader();
    ~StringReader();
    void reciever(QString *str);
    void interpreter(val_type opType, val_type figType, val_type x1, val_type x2,
                    val_type y1,     val_type y2,      val_type z1, val_type z2,
                    val_type stringNumber);
signals:
    void updateGeometry(MatrixData *matrixData);
};

#endif // STRINGREADER_H
