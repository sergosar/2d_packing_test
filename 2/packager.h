#ifndef PACKEGER_H
#define PACKEGER_H

#include <QRect>
#include <QList>
#include <QMap>
#include <QColor>


///////////////////////////////////////////////////////////////////////////////
//  Offline algorithms
///////////////////////////////////////////////////////////////////////////////

class FCNR
{
public:
    FCNR(){}

    const QList<QRect> pack(const QList<QRect> rects);
    void setParam(qreal param) {}
};


///////////////////////////////////////////////////////
class Packager
{
protected:
    FCNR* algorithm;

private:
    QList<QRect> _rectangles;
    int STRIPH;
    int STRIPW;


public:
    Packager(void){}

    void init(QString filename);
    void init(QList<QRect> rects, int H, int W);
    int getSize(void);
    void UseAlgorithm(void);
    void SetParameter(qreal param) {algorithm->setParam(param);}

    QList<QRect> rectangles;

    class Level
    {
    public:
        Level(int b, int h, int f=0, int w=0) : bottom(b),
                                                height(h),
                                                floor(f),
                                                initW(w),
                                                ceiling(0){}

        const QRect put(const QRect &rect, bool f=true, bool leftJustified=true);
        bool ceilingFeasible(const QRect &rect, const QList<QRect> existing);
        bool floorFeasible(const QRect &rect);
        int getSpace(bool f=true);

        int bottom;
        int height;
        int floor;
        int initW;
        int ceiling;
    };
    int getSTRIPH() const;
    void setSTRIPH(int value);
    int getSTRIPW() const;
    void setSTRIPW(int value);
};
#endif // PACKEGER_H
