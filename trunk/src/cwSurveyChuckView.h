#ifndef CWSURVEYCHUCKVIEW_H
#define CWSURVEYCHUCKVIEW_H

//Qt includes
#include <QObject>
#include <QDeclarativeItem>
#include <QList>
#include <QVector>

//Our includes
class cwSurveyChunk;
class cwStation;
class cwShot;


class cwSurveyChunkView : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(cwSurveyChunk* model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit cwSurveyChunkView(QDeclarativeItem *parent = 0);

    cwSurveyChunk* model();
    void setModel(cwSurveyChunk* chunk);

    QRectF boundingRect() const;

    static float elementHeight();
    static float heightHint(int numberElements);

signals:
    void modelChanged();

    void createdNewChunk(cwSurveyChunk* chunk);

public slots:

protected:


private slots:
    //void StationFocusChanged(bool focus);
    void StationValueHasChanged();

    void AddStations(int beginIndex, int endIndex);
    void AddShots(int beginIndex, int endIndex);

    void RemoveStations(int beginIndex, int endIndex);
    void RemoveShots(int beginIndex, int endIndex);

    void Clear();

    void RightClickOnStation(int index);
    void RightClickOnShot(int index);

    void SplitOnStation(int index);
    void SplitOnShot(int index);

private:

    class Row {
    public:
        Row(int rowIndex, int numberOfItems);

        int rowIndex() { return RowIndex; }
        QVector<QDeclarativeItem*> items() { return Items; }

    protected:
        QVector<QDeclarativeItem*> Items;
        int RowIndex;
    };

    class StationRow : public Row {
    public:

        StationRow();
        StationRow(cwSurveyChunkView* Chunk, int RowIndex);

        QDeclarativeItem* station() { return Items[Station]; }
        QDeclarativeItem* left() { return Items[Left]; }
        QDeclarativeItem* right() { return Items[Right]; }
        QDeclarativeItem* up() { return Items[Up]; }
        QDeclarativeItem* down() { return Items[Down]; }

        private:

        enum {
            Station,
            Left,
            Right,
            Up,
            Down,
            NumberItems
        };
    };

    class ShotRow : public Row {
    public:
        ShotRow();
        ShotRow(cwSurveyChunkView* Chunk, int RowIndex);

        QDeclarativeItem* distance() { return Items[Distance]; }
        QDeclarativeItem* frontCompass() { return Items[FrontCompass]; }
        QDeclarativeItem* backCompass() { return Items[BackCompass]; }
        QDeclarativeItem* frontClino() { return Items[FrontClino]; }
        QDeclarativeItem* backClino() { return Items[BackClino]; }

    private:
        enum {
            Distance,
            FrontCompass,
            BackCompass,
            FrontClino,
            BackClino,
            NumberItems
        };
    };

    friend class StationRow;
    friend class ShotRow;

    cwSurveyChunk* Model;
    QList<StationRow> StationRows;
    QList<ShotRow> ShotRows;

    //Stations and shots are added to the navigation queue
    //When the are added and remove.  The navigation queue
    //The navigation queues are checked by UpdateNavigation()
    QList<int> StationNavigationQueue;
    QList<int> ShotNavigationQueue;

    QDeclarativeComponent* StationDelegate;
    QDeclarativeComponent* TitleDelegate;
    QDeclarativeComponent* LeftDelegate;
    QDeclarativeComponent* RightDelegate;
    QDeclarativeComponent* UpDelegate;
    QDeclarativeComponent* DownDelegate;
    QDeclarativeComponent* DistanceDelegate;
    QDeclarativeComponent* FrontCompassDelegate;
    QDeclarativeComponent* BackCompassDelegate;
    QDeclarativeComponent* FrontClinoDelegate;
    QDeclarativeComponent* BackClinoDelegate;


    QDeclarativeItem* StationTitle;
    QDeclarativeItem* DistanceTitle;
    QDeclarativeItem* AzimuthTitle;
    QDeclarativeItem* ClinoTitle;
    QDeclarativeItem* LeftTitle;
    QDeclarativeItem* RightTitle;
    QDeclarativeItem* UpTitle;
    QDeclarativeItem* DownTitle;

    QMenu* RightClickMenu;

    void CreateTitlebar();
    void SetupDelegates();

    void PositionStationRow(StationRow row, int index);
    void PositionElement(QDeclarativeItem* item, QDeclarativeItem* titleItem, int index, int yOffset = 0, QSizeF size = QSizeF());
    void ConnectStation(cwStation* station, StationRow row);

    void PositionShotRow(ShotRow row, int index);
    void ConnectShot(cwShot* shot, ShotRow row);

    void UpdateNavigation();
    void UpdateStationTabNavigation(int index);
    void UpdateShotTabNavigation(int index);
    void LRUDTabNavigation(StationRow row, QDeclarativeItem* previous, QDeclarativeItem* next);
    void SetTabOrder(QDeclarativeItem* item, QDeclarativeItem* previous, QDeclarativeItem* next);
    void UpdateStationArrowNavigation(int index);
    void UpdateShotArrowNavigaton(int index);
    void SetArrowNavigation(QDeclarativeItem* item, QDeclarativeItem* left, QDeclarativeItem* right, QDeclarativeItem* up, QDeclarativeItem* down);

    ShotRow GetShotRow(int index);
    StationRow GetStationRow(int index);

    void UpdateLastRowBehaviour();
    void UpdatePositionsAfterIndex(int index);
    void UpdateIndexes(int index);
    void UpdateDimensions();

    bool InterfaceValid();
};



#endif // CWSURVEYCHUCKVIEW_H
