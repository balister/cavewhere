#ifndef CWSURVEYCHUNKGROUPVIEW_H
#define CWSURVEYCHUNKGROUPVIEW_H

//Qt includes
#include <QDeclarativeItem>
class QModelIndex;

//Our includes
class cwSurveyChunkGroup;
class cwSurveyChunkView;
class cwSurveyChunk;

class cwSurveyChunkGroupView : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(cwSurveyChunkGroup* chunkGroup READ chunkGroup WRITE setChunkGroup NOTIFY chunkGroupChanged)
    Q_PROPERTY(float contentHeight READ contentHeight NOTIFY contentHeightChanged )
    Q_PROPERTY(float contentWidth READ contentWidth NOTIFY contentWidthChanged )
    Q_PROPERTY(float viewportX READ viewportX WRITE setViewportX NOTIFY viewportXChanged)
    Q_PROPERTY(float viewportY READ viewportY WRITE setViewportY NOTIFY viewportYChanged)
    Q_PROPERTY(float viewportWidth READ viewportWidth WRITE setViewportWidth NOTIFY viewportWidthChanged)
    Q_PROPERTY(float viewportHeight READ viewportHeight WRITE setViewportHeight NOTIFY viewportHeightChanged)


public:
    explicit cwSurveyChunkGroupView(QDeclarativeItem *parent = 0);

    void setChunkGroup(cwSurveyChunkGroup* chunkGroup);
    cwSurveyChunkGroup* chunkGroup() const;

    Q_INVOKABLE float contentHeight() const;
    Q_INVOKABLE float contentWidth() const;

    Q_INVOKABLE float viewportX() const;
    Q_INVOKABLE float viewportY() const;
    Q_INVOKABLE float viewportWidth() const;
    Q_INVOKABLE float viewportHeight() const;

    Q_INVOKABLE void setViewportX(float x);
    Q_INVOKABLE void setViewportY(float y);
    Q_INVOKABLE void setViewportWidth(float width);
    Q_INVOKABLE void setViewportHeight(float height);

signals:
    void chunkGroupChanged();
    void contentHeightChanged();
    void contentWidthChanged();

    void viewportXChanged();
    void viewportYChanged();
    void viewportWidthChanged();
    void viewportHeightChanged();

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    //The model
    cwSurveyChunkGroup* ChunkGroup;

    //GUI elements
    QList<cwSurveyChunkView*> ChunkViews; //The active chunks
    QList<QRectF> ChunkBoundingRects;  //Sorted in ascending order by y position
   // QList<float> ChunkYPositions; //All the chunk y positions, should be sorted ascending

    //The content height and width, all the content
    QSizeF ContentArea;

    //The viewport area, what is seen by the user
    QRectF ViewportArea;

    void UpdatePosition(int index);
    void UpdateActiveChunkViews();
    void UpdateContentArea(int beginIndex, int endIndex);

    QPair<int, int> VisableRange();
    void CreateChunkView(int index);
    void DeleteChunkView(int index);

private slots:
    void InsertRows(const QModelIndex& parent, int start, int end);
    void AddChunks(int beginIndex, int endIndex);

    void UpdateChunkHeight();

    void HandleSplitChunk(cwSurveyChunk* newChunk);

};

//inline float cwSurveyChunkGroupView::contentHeight() const { return ContentArea.height(); }
//inline float cwSurveyChunkGroupView::contentWidth() const { return ContentArea.width(); }
inline float cwSurveyChunkGroupView::viewportX() const { return ViewportArea.x(); }
inline float cwSurveyChunkGroupView::viewportY() const { return ViewportArea.y(); }
inline float cwSurveyChunkGroupView::viewportWidth() const { return ViewportArea.width(); }
inline float cwSurveyChunkGroupView::viewportHeight() const { return ViewportArea.height(); }

#endif // CWSURVEYCHUNKGROUPVIEW_H
