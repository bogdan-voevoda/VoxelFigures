#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QActionGroup>
#include <mainwidget.h>
#include "Matrix3D.h"
#include "LoaderSaver.h"
#include "StringReader.h"
#include "geometryengine.h"

class SolutionEngine;
class GeometryEngine;
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class MainWidget;
class Matrix3D;
class StringReader;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QWidget* centralWgt;
    MainWidget* solutionView;
    GeometryEngine* solution;
    LoaderSaver* loadSave;
    StringReader* sr;
    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QLabel *infoLabel;
public:
    explicit MainWindow(QWidget *parent = 0);
    void updateGLViews();
    ~MainWindow();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private slots:
     void newFile();
     void open();
     void save();
     void on_textCanged();
private:
     void createActions();
     void createMenus();
};

#endif // MAINWINDOW_H
