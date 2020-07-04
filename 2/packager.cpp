#include "packager.h"
#include "renderarea.h"


#include <QFile>
#include <QTextStream>
#include <QtAlgorithms>
//#include <algorithm>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
//  Packager class implementation
///////////////////////////////////////////////////////////////////////////////



void Packager::setSTRIPH(int value)
{
    STRIPH = value;
}

void Packager::setSTRIPW(int value)
{
    STRIPW = value;
}

void Packager::init(QString filename) {
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

void Packager::init(QList<QRect> rects, int H, int W)
{
    _rectangles.clear();
    unpacked.clear();
    _rectangles = rects;
    setSTRIPH(H);
    setSTRIPW(W);
}

int Packager::getSize(void)
{
    return rectangles.size();
}

void Packager::UseAlgorithm(void)
{
    rectangles.clear();
    rectangles = this->pack(_rectangles,STRIPH, STRIPW);
    //unpacked = _rectangles;

}

static bool decreasingComparsion(const QRect &r1, const QRect &r2)
{
    if(r1.height()!=r2.height())
        return r1.height() > r2.height();
    else return r1.width()>=r2.width();
}


QList<QString> Packager::unList()
{
    qSort(unpacked.begin(), unpacked.end(), decreasingComparsion);
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


const QRect Packager::Level::put(const QRect &rect, int H, int W,bool f, bool leftJustified )
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

bool Packager::Level::ceilingFeasible(const QRect &rect, const QList<QRect> existing, int H, int W)
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

bool Packager::Level::floorFeasible(const QRect &rect, int W)//проверка на укладку очередного прямоугольника на ширину
{
    return rect.width() <= (W - floor);
}

int Packager::Level::getSpace(bool f, int W)
{
    if (f) {
        return W - floor;
    } else {
        return W - ceiling - initW;
    }
}

///////////////////////////////////////////////////////////////////////////////
//  Algorithm class implementation
///////////////////////////////////////////////////////////////////////////////


static bool decreasingHeightComparsion(const QRect &r1, const QRect &r2)
{
    return r1.height() >= r2.height();
}
static bool decreasingWidthComparsion(const QRect &r1, const QRect &r2)
{
    return r1.width() >= r2.width();
}

const QList<QRect>Packager::pack( QList<QRect> rects, int H, int W)
{
    // deleting oversizing rects from list

//    QList<QRect>::iterator i;
//    for (i = rects.begin(); i != rects.end(); ++i){
//        if(i->height()>H || i->width()>W){
//            this->unpacked.push_back(*i);
//            rects.erase(i);
//        }
//    }

    for(auto rect: rects) {
        if(rect.height()>H || rect.width()>W){
            this->unpacked.push_back(rect);
            rects.removeOne(rect);
        }
    }

    QList<QRect> packed;
    if(rects.size()==0) return packed ;

    QList<QRect> unpacked = rects;
    qSort(unpacked.begin(), unpacked.end(), decreasingComparsion);



    QList<Packager::Level> levels;
    Packager::Level level(0, unpacked[0].height(), 0, unpacked[0].width());


    packed.push_back(level.put(unpacked[0],H,W));
    levels.push_back(level);

    for (int i = 1; i < unpacked.size(); i++) {
        int found = -1;
        int min = W;
        for (int j = 0; j < levels.size(); j++) {
            if (levels[j].floorFeasible(unpacked[i], W)) {
                if (levels[j].getSpace() < min) {
                    found = j;
                    min = levels[j].getSpace();
                }
            }
        }
        if (found > -1) { // floor-pack on existing level
            packed.push_back(levels[found].put(unpacked[i],H,W));
        } else {
            int found = -1;
            int min = W;
            for (int j = 0; j < levels.size(); j++) {
                if (levels[j].ceilingFeasible(unpacked[i], packed,H , W)) {
                    if (levels[j].getSpace(false) < min) {
                        found = j;
                        min = levels[j].getSpace(false);
                    }
                }
            }
            if (found > -1) { // ceiling-pack on existing level
                packed.push_back(levels[found].put(unpacked[i], H,W, false));
            } else  if (levels.last().bottom + levels.last().height+unpacked[i].height()<=H){ // a new level, условие на переполнение высоты
                Packager::Level newLevel(levels.last().bottom + levels.last().height,
                                         unpacked[i].height(), 0, unpacked[i].width());
                packed.push_back(newLevel.put(unpacked[i],H,W));
                levels.push_back(newLevel);
            } else {
                this->unpacked.push_back(unpacked[i]);
            }          //заполнение unpucked
        }
    }

    return packed;
}

