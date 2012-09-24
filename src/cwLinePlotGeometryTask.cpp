//Our includes
#include "cwLinePlotGeometryTask.h"
#include "cwSurveyChunk.h"
#include "cwCavingRegion.h"
#include "cwCave.h"
#include "cwTrip.h"
#include "cwSurveyChunk.h"
#include "cwStationPositionLookup.h"
#include "cwDebug.h"

cwLinePlotGeometryTask::cwLinePlotGeometryTask(QObject *parent) :
    cwTask(parent)
{
    Region = NULL;
}

/**
  \brief This runs the task

  This will generate the line geometry for a region.  This will iterate through
  all the caves and trips and survey chunks.  It'll produce a vector of point data
  and indexs to that point data that'll create lines between the stations.
  */
void cwLinePlotGeometryTask::runTask() {
    PointData.clear();
    IndexData.clear();
    StationIndexLookup.clear();

    for(int caveIndex = 0; caveIndex < Region->caveCount(); caveIndex++) {
        addStationPositions(caveIndex);
        addShotLines(caveIndex);
    }

    PointData.squeeze();
    IndexData.squeeze();

    StationIndexLookup.clear();

    emit done();
}

/**
  \brief Helper to runTask()

  This adds the station positions to PointData
  */
void cwLinePlotGeometryTask::addStationPositions(int caveIndex) {
    cwCave* cave = Region->cave(caveIndex);

    QMapIterator<QString, QVector3D> iter(cave->stationPositionLookup().positions());

    while(iter.hasNext()) {
        iter.next();

        QString fullName = fullStationName(caveIndex, cave->name(), iter.key());

        StationIndexLookup.insert(fullName, PointData.size());

        PointData.append(iter.value());
    }
}

/**
  \brief Helper to runTask

  addStationPositions() needs to be run for the cave before calling this method

  This will generate the IndexData.  This function connects the PointData with lines.
  OpenGL can the draw lines between the point data and the indexData
  */
void cwLinePlotGeometryTask::addShotLines(int caveIndex) {
    cwCave* cave = Region->cave(caveIndex);

    unsigned int firstStationIndex;

    //Go through all the trips in the cave
    for(int tripIndex = 0; tripIndex < cave->tripCount(); tripIndex++) {
        cwTrip* trip = cave->trip(tripIndex);

        //Go through all the chunks in the trip
        foreach(cwSurveyChunk* chunk, trip->chunks()) {

            if(chunk->stationCount() < 2) { continue; }

            cwStation firstStation = chunk->station(0);

            QString fullName = fullStationName(caveIndex, cave->name(), firstStation.name());
            if(!StationIndexLookup.contains(fullName)) {
                qDebug() << "Warning! Couldn't find station position index (will result in rendering artifacts): " << fullName << LOCATION;
            }

            unsigned int previousStationIndex = StationIndexLookup.value(fullName, 0);

            //Go through all the the stations/shots in the chunk
            for(int stationIndex = 1; stationIndex < chunk->stationCount(); stationIndex++) {
                cwStation station = chunk->station(stationIndex);

                //Look up the index
                fullName = fullStationName(caveIndex, cave->name(), station.name());
                if(StationIndexLookup.contains(fullName)) {
                    unsigned int stationIndex = StationIndexLookup.value(fullStationName(caveIndex, cave->name(), station.name()), 0);

                    if(station.name() == "1") {
                        firstStationIndex = previousStationIndex;
                    }

                    IndexData.append(previousStationIndex);
                    IndexData.append(stationIndex);

                    previousStationIndex = stationIndex;
                }
            }
        }
    }
}


