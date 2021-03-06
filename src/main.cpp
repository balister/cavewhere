/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

//Qt includes
#include <QGuiApplication>
#include <QApplication>
#include <QQmlContext>
#include <QThread>
#include <QQuickView>
#include <QModelIndex>
#include <QWidget>
#include <QDir>
#include <QImageReader>
#include <QOpenGLFunctions>
#include <QMessageBox>
#include <QQmlApplicationEngine>

//Our includes
//#include "cwMainWindow.h"
#include "cwStation.h"
#include "cwSurveyChunk.h"
#include "cwCavingRegion.h"
#include "cwImage.h"
#include "cwGlobalDirectory.h"
#include "cwStation.h"
#include "cwQMLRegister.h"
#include "cwRootData.h"
#include "cwProject.h"
#include "cwImageProvider.h"
#include "cwOpenFileEventHandler.h"
#include "cwQMLReload.h"

#ifndef CAVEWHERE_VERSION
#define CAVEWHERE_VERSION "Sauce-Release"
#endif

QUrl mainWindowSourcePath() {
    QString mainWindowPath = "/qml/CavewhereMainWindow.qml";
    QString workingDirectory = QDir::currentPath() + mainWindowPath;
    QString applicationDirectory = QApplication::applicationDirPath() + mainWindowPath;
    if(QFileInfo(workingDirectory).isFile()) {
        return QUrl::fromLocalFile(workingDirectory);
    } else if(QFileInfo(applicationDirectory).isFile()){
        return QUrl::fromLocalFile(applicationDirectory);
    }
    qDebug() << "Couldn't find Main Window searched:" << workingDirectory << "and" << applicationDirectory;
    return QUrl();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    cwRootData* rootData = new cwRootData();

    //Handles when the user clicks on a file in Finder(Mac OS X) or Explorer (Windows)
    cwOpenFileEventHandler* openFileHandler = new cwOpenFileEventHandler(&a);
    openFileHandler->setProject(rootData->project());
    a.installEventFilter(openFileHandler);

//    foreach(QByteArray imageFormats, QImageReader::supportedImageFormats()) {
//        qDebug() << "Image formats:" << imageFormats;
//    }

    qRegisterMetaType<QThread*>("QThread*");
    qRegisterMetaType<cwCavingRegion>("cwCavingRegion");
    qRegisterMetaType<QList <QString> >("QList<QString>");
    qRegisterMetaType<QList <cwImage> >("QList<cwImage>");
    qRegisterMetaType<QList <cwStation > >("QList<cwStation>");
    qRegisterMetaType<QModelIndex>("QModelIndex");
    qRegisterMetaType<cwImage>("cwImage");
    qRegisterMetaType<GLuint>("GLuint");

    QApplication::setOrganizationName("Vadose Solutions");
    QApplication::setOrganizationDomain("cavewhere.com");
    QApplication::setApplicationName("Cavewhere");
    QApplication::setApplicationVersion("0.1");

    cwGlobalDirectory::setupBaseDirectory();

    //Register all of the cavewhere types
    cwQMLRegister::registerQML();

    QUrl mainWindowPath = mainWindowSourcePath();
    QQmlApplicationEngine applicationEnigine;

    rootData->qmlReloader()->setApplicationEngine(&applicationEnigine);

//    applicationEnigine.rootContext();

//    QQuickView view;
//    view.setTitle(QString("Cavewhere - %1").arg(CAVEWHERE_VERSION));

//    QSurfaceFormat format = view.format();
//    format.setSamples(4);

     //&view);

//    rootData->setQuickView(&view);
//    rootData->project()->load(QDir::homePath() + "/Dropbox/quanko.cw");
//    rootData->project()->load(QDir::homePath() + "/test.cw");
    QQmlContext* context =  applicationEnigine.rootContext(); //view.rootContext();

    context->setContextObject(rootData);
    context->setContextProperty("rootData", rootData);
    //    context->setContextProperty("mainWindow", &view);

    //This allow to extra image data from the project's image database
    cwImageProvider* imageProvider = new cwImageProvider();
    imageProvider->setProjectPath(rootData->project()->filename());
    QObject::connect(rootData->project(), SIGNAL(filenameChanged(QString)), imageProvider, SLOT(setProjectPath(QString)));
    context->engine()->addImageProvider(cwImageProvider::Name, imageProvider);

    //Allow the engine to quit the application
    QObject::connect(context->engine(), SIGNAL(quit()), &a, SLOT(quit()));

//    QUrl mainWindowPath = mainWindowSourcePath();

    if(!mainWindowPath.isEmpty()) {
        applicationEnigine.load(mainWindowPath);
        //        view.setFormat(format);
//        view.setResizeMode(QQuickView::SizeRootObjectToView);
//        view.setSource(mainWindowPath);
//        view.show();
    } else {
        QMessageBox mainWindowNotFoundMessage(QMessageBox::Critical,
                                              "Cavewhere Failed to Load Main Window",
                                              "ಠ_ರೃ Cavewhere has failed to load its main window... <br><b>This is a bug!</b>",
                                              QMessageBox::Close);
        mainWindowNotFoundMessage.exec();
        return 1;
    }

    return a.exec();
}
