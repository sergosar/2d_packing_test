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

    const QList<QRect> pack(const QList<QRect> rects, int H, int W);
    void setParam(qreal param) {}
};


///////////////////////////////////////////////////////
class Packager
{
protected:
    FCNR* algorithm;// убрать

private:
    QList<QRect> _rectangles;
    int STRIPH;
    int STRIPW;


public:
    Packager(void){}
    const QList<QRect> pack(const QList<QRect> rects, int H, int W);
    void init(QString filename);
    void init(QList<QRect> rects, int H, int W);
    int getSize(void);
    void UseAlgorithm(void);
    void SetParameter(qreal param) {algorithm->setParam(param);}

    QList<QString> subtruct(QList<QRect> full, QList<QRect> packed);

    QList<QRect> rectangles;
    QList<QRect> unpacked;

    class Level
    {
    public:
        Level(int b, int h, int f=0, int w=0) : bottom(b),
                                                height(h),
                                                floor(f),
                                                initW(w),
                                                ceiling(0){}

        const QRect put(const QRect &rect,int H, int W, bool f=true, bool leftJustified=true ); //!!!!!!!
        bool ceilingFeasible(const QRect &rect, const QList<QRect> existing, int H, int W);
        bool floorFeasible(const QRect &rect, int W);
        int getSpace(bool f=true, int W=0);

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
