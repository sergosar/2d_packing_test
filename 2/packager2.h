#ifndef PACKEGER2_H
#define PACKEGER2_H

#include <QRect>
#include <QList>
#include <QMap>
#include <QColor>



///////////////////////////////////////////////////////
class Packager2
{

private:
    QList<QSize> _rectangles;
    int STRIPH;
    int STRIPW;
    QList<QSize> unpacked;

    class CoaPoint
    {
    public:
        CoaPoint(QRect& r1, QRect& r2, QPoint p):
            r1(r1),
            r2(r2),
            point(p){}

        QRect r1;
        QRect r2;
        QPoint point;

    };

    class CoAction
    {
    public:
        CoAction(CoaPoint cPoint, QRect r):
            cPoint(cPoint),
            testRect(r){}
    CoaPoint cPoint;
    QRect testRect;
    double cavingDegree;
short int cornerDegree;
short int edgeDegree;
int area=0;
int maxBottom=0;
    };

    QPoint getIntersCoords(QRect &r1, QRect &r2);
    bool edgeOverlap(QRect &r1, QRect &r2);
    void coaSort(QList<CoAction> & actions);
    QList<QRect> basicProgramm(QList<QRect> rectangles, QList<QSize>& unpacked, QList<CoaPoint> coaPointsList);
    //void?
    void basicProgramm2(QList<QRect> rectangles, QList<QSize> unpacked, QList<Packager2::CoaPoint> coaPointsList, CoAction& action);
    void coaSort2(QList<Packager2::CoAction> &actions);
public:
    Packager2(void){}

    void init(QList<QSize> rects, int H, int W);
    int getSize(void);
    void UseAlgorithm(void);
    void setSTRIPH(int value);
    void setSTRIPW(int value);

    QList<QString> unList();
    QList<QRect> rectangles;

    bool Intersected(QRect &r, QList<QRect>& rectangles);
    bool feasibleCOA(QRect &r, CoaPoint &cPoint, QList<QRect> & rectangles);
    double calcCavingDegree(CoAction &action);
    double dist(QRect &r1, QRect &r2);
    void UseAlgorithm2();
};
#endif // PACKEGER2_H
