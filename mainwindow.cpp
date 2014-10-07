#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#define CLEAR_DATA -1,-1,0,0,0,0,0,0,0

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {

    loadSave =  new LoaderSaver;
    sr = new StringReader();
    ui->setupUi(this);
    centralWgt   = new QWidget;
    solution = new GeometryEngine(this);
    solutionView = new MainWidget(this, solution);
    centralWgt->setLayout(ui->gridLayout);
    this->setCentralWidget(centralWgt);
    ui->view2->addWidget(solutionView);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

#ifdef Q_OS_SYMBIAN
    infoLabel = new QLabel(tr("<i>Choose a menu option</i>"));
#else
    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
#endif
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    createActions();
    createMenus();

#ifndef Q_OS_SYMBIAN
    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);
#endif

    setMinimumSize(160, 160);
    resize(480, 580);

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_textCanged()));
    connect(this->sr, SIGNAL(updateGeometry(MatrixData*)), this->solution, SLOT(update_Geometry(MatrixData*)));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.exec(event->globalPos());
}

void MainWindow::newFile() {
    infoLabel->setText(tr(" <b>File|New</b>"));
    sr->interpreter(CLEAR_DATA);
    ui->textEdit->clear();
}

void MainWindow::open() {
    infoLabel->setText(tr("Invoked <b>File|Open</b>"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Text files (*.txt)"));
    solution->getMatrix()->clear();
    loadSave->load(solution->getMatrix(), fileName);
    solution->updateVertices();
}

void MainWindow::updateGLViews() {
    solutionView->updateGL();
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(0,"Save file",QDir::currentPath(),
                                                    "Text files (*.txt);;All files (*.*)",
                                                        new QString("Text files (*.txt)"));
    loadSave->save(solution->getMatrix(), fileName);
    infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

void MainWindow::on_textCanged() {
    QString str = ui->textEdit->toPlainText();

    if ( str != NULL ) {
        sr->reciever(&str);
    } else { sr->interpreter(CLEAR_DATA); }
}

MainWindow::~MainWindow() {
    delete solution;
    delete solutionView;
    delete centralWgt;
    delete sr;
    delete loadSave;
    delete ui;
}
