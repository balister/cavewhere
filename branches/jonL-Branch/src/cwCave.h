#ifndef CWCAVE_H
#define CWCAVE_H

//Our include
class cwTrip;
#include <cwStation.h>
#include <cwUndoer.h>

//Qt includes
#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <QUndoCommand>
#include <QDebug>
#include <QWeakPointer>
#include <QVariant>

class cwCave : public QObject, public cwUndoer
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit cwCave(QUndoStack* stack = NULL, QObject* parent = NULL);
    cwCave(const cwCave& object);
    cwCave& operator=(const cwCave& object);

    QString name() const;
    void setName(QString name);

    int tripCount() const;
    cwTrip* trip(int index) const;

    Q_INVOKABLE void addTrip(cwTrip* trip = NULL);
    void insertTrip(int i, cwTrip* trip);
    void removeTrip(int i);
    int indexOf(cwTrip* trip) const;

    bool hasStation(QString name);
    QWeakPointer<cwStation> station(QString name);
    void addStation(QSharedPointer<cwStation> station);
    void removeStation(QString name);
    void setStationData(QSharedPointer<cwStation> station, QVariant data, cwStation::DataRoles role);
    QVariant stationData(QSharedPointer<cwStation> station, cwStation::DataRoles role) const;

    QList< QWeakPointer<cwStation> > stations() const;

signals:
    void beginInsertTrips(int begin, int end);
    void insertedTrips(int begin, int end);

    void beginRemoveTrips(int begin, int end);
    void removedTrips(int begin, int end);

    void nameChanged(QString name);

    void stationAddedToCave(QString name);
    void stationRemovedFromCave(QString name);
    void stationDataChanged(QSharedPointer<cwStation>, cwStation::DataRoles role);

protected:
    QList<cwTrip*> Trips;
    QString Name;

    QMap<QString, QWeakPointer<cwStation> > StationLookup;

private:
    cwCave& Copy(const cwCave& object);
    void addTripNullHelper();

////////////////////// Undo Redo commands ///////////////////////////////////
    class NameCommand : public QUndoCommand {
    public:
        NameCommand(cwCave* cave, QString name);
        void redo();
        void undo();
    private:
        QWeakPointer<cwCave> CavePtr;
        QString newName;
        QString oldName;
    };

    class InsertRemoveTrip : public QUndoCommand {
    public:
        InsertRemoveTrip(cwCave* cave, int beginIndex, int endIndex);

    protected:
        void insertTrips();
        void removeTrips();

        QList<cwTrip*> Trips;
    private:
        QWeakPointer<cwCave> CavePtr;
        int BeginIndex;
        int EndIndex;
    };

    class InsertTripCommand : public InsertRemoveTrip {
    public:
        InsertTripCommand(cwCave* cave, cwTrip* Trip, int index);
        InsertTripCommand(cwCave* cave, QList<cwTrip*> Trip, int index);
        virtual void redo();
        virtual void undo();
    };

    class RemoveTripCommand : public InsertRemoveTrip {
    public:
        RemoveTripCommand(cwCave* cave, int beginIndex, int endIndex);
        virtual void redo();
        virtual void undo();
    };

    class StationDataCommand : public QUndoCommand {
    public:
        StationDataCommand(cwCave* cave,
                           QSharedPointer<cwStation> station,
                           QVariant data,
                           cwStation::DataRoles role);
        virtual void redo();
        virtual void undo();

    private:
        QWeakPointer<cwCave> Cave;
        QSharedPointer<cwStation> Station;
        QVariant NewData;
        QVariant OldData;
        cwStation::DataRoles Role;
    };

};

/**
  \brief Get's the name of the cave
  */
inline QString cwCave::name() const {
    return Name;
}

/**
  \brief Get's the number of survey trips in the cave
  */
inline int cwCave::tripCount() const {
    return Trips.count();
}

/**
  \brief Get's the trip at an index

  If the index is out of bounds this return NULL
  */
inline cwTrip* cwCave::trip(int index) const {
    if(index < 0 || index >= Trips.size()) { return NULL; }
    return Trips[index];
}

/**
  \brief Test if this caves has a station with the name, name.

  Returns true if it has a station with that name
  */
inline bool cwCave::hasStation(QString name) {
    QWeakPointer<cwStation> pointer = station(name);
    return !pointer.isNull();
}

/**
  \brief Get's the station from it's name
  */
inline QWeakPointer<cwStation> cwCave::station(QString name) {
    return StationLookup.value(name, QWeakPointer<cwStation>());
}

/**
  \brief Adds the station to the cave
  */
inline void cwCave::addStation(QSharedPointer<cwStation> station) {
    if(!station->name().isEmpty()) {
        StationLookup[station->name()] = station.toWeakRef();
    }
}

/**
  \brief Gets all the stations in the cave

  To figure out how stations are structured see cwTrip and cwSurveyChunk

  Shots and stations are stored in the cwSurveyChunk
  */
inline QList< QWeakPointer<cwStation> > cwCave::stations() const {
    return StationLookup.values();
}

/**
  \brief Gets the index of the trip inside of the cave
  */
inline int cwCave::indexOf(cwTrip* trip) const {
    return Trips.indexOf(trip);
}

#endif // CWCAVE_H