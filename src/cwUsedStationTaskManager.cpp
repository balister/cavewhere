/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

//Our includes
#include "cwUsedStationTaskManager.h"
#include "cwUsedStationsTask.h"
#include "cwCave.h"

//Qt includes
#include <QThread>

cwUsedStationTaskManager::cwUsedStationTaskManager(QObject *parent) :
    QObject(parent),
    Cave(NULL)
{
    Thread = new QThread(this);
    Thread->start();

    Task = new cwUsedStationsTask();
    Task->setThread(Thread);
    connect(Task, SIGNAL(shouldRerun()), SLOT(calculateUsedStations()));
    connect(Task, SIGNAL(usedStations(QList<QString>)), SLOT(setUsedStations(QList<QString>)));
}

cwUsedStationTaskManager::~cwUsedStationTaskManager() {
    Task->stop();

    Thread->quit(); //Quit the event loop
    Thread->wait(); //wait to finish

    delete Task;
}

/**
  If listen to cave changes is set to true,
  */
void cwUsedStationTaskManager::setListenToCaveChanges(bool listen) {
    if(ListenToCaveChanges != listen) {
        ListenToCaveChanges = listen;

        hookupCaveToTask();

        if(ListenToCaveChanges) {
            calculateUsedStations();
        }
    }
}

/**
  Sets the cave for the manager
  */
void cwUsedStationTaskManager::setCave(cwCave* cave) {
    if(Cave != cave) {
        if(Cave != NULL) {
            disconnect(Cave, SIGNAL(destroyed()), this, SLOT(caveDestroyed()));
        }

        Cave = cave;

        if(Cave != NULL) {
            connect(Cave, SIGNAL(destroyed()), this, SLOT(caveDestroyed()));
            calculateUsedStations();
        }
    }
}

/**
  This starts the running the used station task on an external thread
  */
void cwUsedStationTaskManager::calculateUsedStations() {
    if(Task->isReady()) {
        if(Cave != NULL) {
            QList<cwStation> stations = Cave->stations();

            QMetaObject::invokeMethod(Task, //Object
                                      "setStationNames", //Function
                                      Qt::AutoConnection, //Connection to the function
                                      Q_ARG(QList<cwStation>, stations)); //Arguments to the function

            Task->start();
        }
    } else {
        Task->restart();
    }
}

/**
  Called when the task has completed, stations are set to this object
  */
void cwUsedStationTaskManager::setUsedStations(QList<QString> stations) {
    UsedStations = stations;
    emit usedStationsChanged();
}

/**
 * @brief cwUsedStationTaskManager::caveDestroyed
 *
 * Stops listening to the station, because it's being destoried
 */
void cwUsedStationTaskManager::caveDestroyed()
{
    disconnect(Cave, 0, this, 0);
    Cave = NULL;
}



/**
  \brief Helper function to setListenToCaveChanges

  This hooks up the cave to the task or disconnect it, depending on the value of ListenToCaveCHanges.

  If ListenToCaveChanges is true it connects it, else it disconnects it.

  If Cave is null this function does nothing
  */
void cwUsedStationTaskManager::hookupCaveToTask() {
    if(Cave == NULL) { return; }
    if(ListenToCaveChanges) {
        connect(Cave, SIGNAL(stationAddedToCave(QString)), SLOT(calculateUsedStations()));
        connect(Cave, SIGNAL(stationRemovedFromCave(QString)), SLOT(calculateUsedStations()));
    } else {
        disconnect(Cave, 0, this, 0);
    }
}
