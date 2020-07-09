#include "packager2.h"
#include "renderarea.h"


#include <QFile>
#include <QTextStream>
#include <QtAlgorithms>
#include <cmath>

#include <algorithm>


void Packager2::setSTRIPH(int value)
{
    STRIPH = value;
}
void Packager2::setSTRIPW(int value)
{
    STRIPW = value;
}

void Packager2::init(QString filename) {
    QFile file(filename);

    _rectangles.clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QStringList line = in.readLine().split(",");
            QRect rect;
            if (!line.isEmpty()) {
                rect.setRect(0, 0, line[0].toInt(), line[1].toInt());
                _rectangles.push_back(rect);
            }
        }
    }
    file.close();
}

void Packager2::init(QList<QRect> rects, int H, int W)
{
    _rectangles.clear();

    _rectangles = rects;
    unpacked=rects;
    setSTRIPH(H);
    setSTRIPW(W);
}

int Packager2::getSize(void)
{
    return rectangles.size();
}

void Packager2::UseAlgorithm(void)
{
    rectangles.clear();
     // deleting oversizing rects from list
    for(auto rect: _rectangles) {
        if(rect.height()>STRIPH || rect.width()>STRIPW){
            _rectangles.removeOne(rect);
        }
    }
    pack(_rectangles,STRIPH, STRIPW);

    for(auto rect : rectangles){
        QRect rectd;
        rectd.setRect(0,0,rect.width(),rect.height());
        unpacked.removeOne(rectd);
    }


}

bool decreasingComparsion(const QRect r1, const QRect r2)
{
    if(r1.height()!=r2.height())
        return r1.height() > r2.height();
    else return r1.width()>=r2.width();
}

QList<QString> Packager2::unList()
{
    if (unpacked.empty())
        return QList<QString>();

    qSort( unpacked.begin(), unpacked.end(), decreasingComparsion);
    QList<QString> uList;
    QString temp ="";
    for(QRect rect: unpacked){
        if(temp.isEmpty()) {
            temp = QString::number(rect.height()) + " " + QString::number(rect.width())+ " " + "1";
        } else if(temp.split(" ")[0].toInt()==rect.height() && temp.split(" ")[1].toInt()==rect.width()) {
            temp=QString::number(rect.height()) + " " + QString::number(rect.width())+ " " + QString::number(temp.split(" ")[2].toInt()+1);
        } else {
            uList.push_back(temp);
            temp=QString::number(rect.height()) + " " + QString::number(rect.width())+ " " + "1";
        }
    }
    uList.push_back(temp);
    return uList;
}


const QRect Packager2::Level::put(const QRect &rect, int H, int W,bool f, bool leftJustified )
{
    QRect newRect;

    if (f) {
        if (leftJustified) {
            newRect.setRect(floor, H - (bottom + rect.height() + 1),
                            rect.width(), rect.height());
        } else {
            // 'ceiling' is used for right-justified rectangles packed on the floor
            newRect.setRect(W - (ceiling + rect.width()),
                            H - (bottom + rect.height() + 1),
                            rect.width(), rect.height());
            ceiling += rect.width();
        }
        floor += rect.width();
    } else {
        newRect.setRect(W - (ceiling + rect.width()),
                        H - (bottom + height + 1),
                        rect.width(), rect.height());
        ceiling += rect.width();
    }

    return newRect;
}

bool Packager2::Level::ceilingFeasible(const QRect &rect, const QList<QRect> existing, int H, int W)
{
    QRect testRect;
    testRect.setRect(W - (ceiling + rect.width()),
                     H - (bottom + height + 1),
                     rect.width(), rect.height());
    bool intersected = false;
    for (int i = 0; i < existing.size(); i++) {
        if (testRect.intersects(existing[i])) {
            intersected = true;
            break;
        }
    }
    bool fit = rect.width() <= (W - ceiling - initW);
    return fit && !intersected;
}

bool Packager2::Level::floorFeasible(const QRect &rect, const QList<QRect> existing, int H, int W)
{
    QRect testRect;
    testRect.setRect(floor,H-(bottom+ rect.height()+1),
                     rect.width(), rect.height());
    bool intersected = false;
    for (int i = 0; i < existing.size(); i++) {
        if (testRect.intersects(existing[i])) {
            intersected = true;
            break;
        }
    }

    return rect.width() <= (W - floor) && !intersected;

}

int Packager2::Level::getSpace(bool f, int W)
{
    if (f) {
        return W - floor;
    } else {
        return W - ceiling - initW;
    }
}



void Packager2::pack( QList<QRect> &rects, int H, int W, int dx, int dy,int dH)
{


    if(rects.size()==0) return;

    qSort(rects.begin(), rects.end(), decreasingComparsion);

    QList<Packager2::Level> levels;
    Packager2::Level level(dy, rects[0].height(), 0+dx, rects[0].width());
    if (level.floorFeasible(rects[0],rectangles,H, W)){
        rectangles.push_back(level.put(rects[0],H,W));
        rects.removeFirst();
        levels.push_back(level);
    } else return;


    while(!rects.isEmpty()){
      if(rects.first().height()*2<=rectangles.last().height() && (rects.first().width()+rectangles.last().right())<=W)
        {
           // const QRect &r=rectangles.last();       // для всех уровней

            for(auto level : levels){
                if(!rects.isEmpty() && level.floor+rects.first().width()<=W)
                pack(rects,H,W ,level.floor, level.bottom, H-level.bottom-level.height);
            }

        } else {
            int found = -1;
            int min = W;
            for (int j = 0; j < levels.size(); j++) {
                if (levels[j].floorFeasible(rects[0],rectangles,H, W)) {
                    if (levels[j].getSpace() < min) {
                        found = j;
                        min = levels[j].getSpace();
                    }
                }
            }
            if (found > -1) { // floor-pack on existing level
                rectangles.push_back(levels[found].put(rects[0],H,W));
                rects.removeFirst();
            } else {
                int found = -1;
                int min = W;
                for (int j = 0; j < levels.size(); j++) {
                    if (levels[j].ceilingFeasible(rects.first(), rectangles,H , W)) {
                        if (levels[j].getSpace(false) < min) {
                            found = j;
                            min = levels[j].getSpace(false);
                        }
                    }
                }
                if (found > -1) { // ceiling-pack on existing level
                    rectangles.push_back(levels[found].put(rects.first(), H,W, false));
                    rects.removeFirst();
                } else  if (levels.last().bottom + levels.last().height+rects.first().height()<=H-dH){ // a new level
                    Packager2::Level newLevel(levels.last().bottom + levels.last().height,
                                           rects.first().height(), dx, rects.first().width());
                    rectangles.push_back(newLevel.put(rects.first(),H,W));
                    levels.push_back(newLevel);
                    rects.removeFirst();
                } else {
                    return;
                }          //unpucked
            }
        }
    }

}

