/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

//Our includes
#include "cwBaseNoteStationInteraction.h"
#include "cwNoteStation.h"
//#include "cwNoteStationView.h"
#include "cwScrap.h"
#include "cwScrapView.h"
#include "cwScrapItem.h"
#include "cwScrapStationView.h"

cwBaseNoteStationInteraction::cwBaseNoteStationInteraction(QQuickItem *parent) :
    cwInteraction(parent),
//    NoteStationView(NULL),
    ScrapView(NULL)
{
}

/**
  \brief Adds a new station to the cwNoteItem

  The new station position will be converted from qtViewportCoordinates into
  a normalized position of the notes.

  \param qtViewportCoordinates - This is the unprojected viewport coordinates, ie
  the qt window coordinates were the top left is the origin.  These should be
  in local coordines of the cwNoteItem

  */
void  cwBaseNoteStationInteraction::addStation(QPointF notePosition) {
//    qDebug() << "Add station!" << notePosition;

    //Make sure we have a scrap view
    if(ScrapView == NULL) {
        return;
    }

    //Find what scrap we need to add this station to
    QList<cwScrapItem*> scrapItems = ScrapView->scrapItemsAt(notePosition);

    //Select the scrap that we're going to add the station to
    cwScrapItem* scrapItem = selectScrapForAdding(scrapItems);

    //Make sure we have a scrap to add to
    if(scrapItem == NULL) {
        //Do something to notify the user that they've clicked outside the bounds
        return;
    }

    cwScrap* scrap = scrapItem->scrap();

    //Make sure we have a scrap to add to
    if(scrap == NULL) {
        //Do something to notify the user that they've clicked outside the bounds
        return;
    }

    cwNoteStation newNoteStation;
    newNoteStation.setPositionOnNote(notePosition);

    //Try to guess the station name
    cwNoteStation selectedStation = scrapItem->stationView()->selectedNoteStation();
    QString stationName = scrap->guessNeighborStationName(selectedStation, notePosition);

    if(stationName.isEmpty()) {
        stationName = "Station Name";
    }

    newNoteStation.setName(stationName);
    scrap->addStation(newNoteStation);

    //Get the last station in the list and select it
    scrapItem->stationView()->setSelectedItemIndex(scrap->numberOfStations() - 1);

    //If this is the first item, we'll need to make it go into edit mode
    if(scrap->numberOfStations() == 1) {
        QQuickItem* stationItem = scrapItem->stationView()->selectedItem();
        QMetaObject::invokeMethod(stationItem, "startEditting");
    }
}

/**
    Sets the scrapView for the station interaction
*/
void cwBaseNoteStationInteraction::setScrapView(cwScrapView* scrapView) {
    if(ScrapView != scrapView) {
        ScrapView = scrapView;
        emit scrapViewChanged();
    }
}

/**
    This selects the scrap for add new station should be added to

    It will also select the scrap item of where the stations will be added to

    This will return NULL, if station can't be added
  */
cwScrapItem *cwBaseNoteStationInteraction::selectScrapForAdding(QList<cwScrapItem *> scrapItems) {
    //Station isn't on a scrap
    if(scrapItems.isEmpty()) {
        return NULL;
    }

    cwScrapItem* scrapItem = NULL;
    if(scrapItems.size() == 1) {
        //Select the only scrap item
        scrapItem = scrapItems.first();
        ScrapView->setSelectScrapIndex(ScrapView->indexOf(scrapItem)); //Select the first scrap
    } else {
        //More than one scrap under the station

        //If the scrapview's select scrap
        cwScrapItem* selectedScrapItem = ScrapView->selectedScrapItem();
        if(scrapItems.contains(selectedScrapItem)) {
            //Use the currently select item's scrap
            scrapItem = selectedScrapItem;
        } else {
            //Just use the first scrap in the list
            scrapItem = scrapItems.first();
            ScrapView->setSelectScrapIndex(ScrapView->indexOf(scrapItem)); //Select the first scrap
        }
    }

    return scrapItem;
}
