/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

#ifndef SURVEYEDITORMAINWINDOW_H
#define SURVEYEDITORMAINWINDOW_H


//Our includes
#include "ui_cwMainWindow.h"
class cwSurvexImporter;
class cwSurvexExporter;
class cwRootData;

//Qt includes
#include <QString>
#include <QUndoStack>
#include <QMainWindow>
class QTreeView;
class QGLWidget;

class cwMainWindow : public QMainWindow, private Ui::cwMainWindow
{
    Q_OBJECT

public:
    explicit cwMainWindow(QWidget *parent = 0);

    void changeEvent(QEvent *e);

protected slots:

    void reloadQML();

    //For undo / redo
    void updateUndoText(QString undoText);
    void updateRedoText(QString redoText);

    //For saving/loading the project
    void saveAs();
    void save();
    void load();
    void newProject();

private:
    cwSurvexExporter* SurvexExporter;

    //The global objects that all qml objects have access to
    cwRootData* Data;

    //For undo and redo
    QAction* UndoAction;
    QAction* RedoAction;

    QGLWidget* createGLWidget();
    void initGLEW();

    void initialWindowShape();

};

#endif // SURVEYEDITORMAINWINDOW_H
