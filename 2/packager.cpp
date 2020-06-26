#include "packager.h"
#include "renderarea.h"

#include <QFile>
#include <QTextStream>
#include <QtAlgorithms>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
//  Packager class implementation
///////////////////////////////////////////////////////////////////////////////

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

int Packager::getSize(void)
{
    return rectangles.size();
}

void Packager::UseAlgorithm(void)
{
    rectangles.clear();
    rectangles = algorithm->pack(_rectangles);
}

const QRect Packager::Level::put(const QRect &rect, bool f, bool leftJustified)
{
    QRect newRect;

    if (f) {
        if (leftJustified) {
            newRect.setRect(floor, RenderArea::STRIPH - (bottom + rect.height() + 1),
                            rect.width(), rect.height());
        } else {
            // 'ceiling' is used for right-justified rectangles packed on the floor
            newRect.setRect(RenderArea::STRIPW - (ceiling + rect.width()),
                            RenderArea::STRIPH - (bottom + rect.height() + 1),
                            rect.width(), rect.height());
            ceiling += rect.width();
        }
        floor += rect.width();
    } else {
        newRect.setRect(RenderArea::STRIPW - (ceiling + rect.width()),
                        RenderArea::STRIPH - (bottom + height + 1),
                        rect.width(), rect.height());
        ceiling += rect.width();
    }

    return newRect;
}

bool Packager::Level::ceilingFeasible(const QRect &rect, const QList<QRect> existing)
{
    QRect testRect;
    testRect.setRect(RenderArea::STRIPW - (ceiling + rect.width()),
                     RenderArea::STRIPH - (bottom + height + 1),
                     rect.width(), rect.height());
    bool intersected = false;
    for (int i = 0; i < existing.size(); i++) {
        if (testRect.intersects(existing[i])) {
            intersected = true;
            break;
        }
    }
    bool fit = rect.width() <= (RenderArea::STRIPW - ceiling - initW);
    return fit && !intersected;
}

bool Packager::Level::floorFeasible(const QRect &rect)
{
    return rect.width() <= (RenderArea::STRIPW - floor);
}

int Packager::Level::getSpace(bool f)
{
    if (f) {
        return RenderArea::STRIPW - floor;
    } else {
        return RenderArea::STRIPW - ceiling - initW;
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

const QList<QRect> FCNR::pack(const QList<QRect> rects)
{
    QList<QRect> unpacked = rects;
    qSort(unpacked.begin(), unpacked.end(), decreasingHeightComparsion);

    QList<Packager::Level> levels;
    Packager::Level level(0, unpacked[0].height(), 0, unpacked[0].width());
    QList<QRect> packed;

    packed.push_back(level.put(unpacked[0]));
    levels.push_back(level);

    for (int i = 1; i < unpacked.size(); i++) {
        int found = -1;
        int min = RenderArea::STRIPW;
        for (int j = 0; j < levels.size(); j++) {
            if (levels[j].floorFeasible(unpacked[i])) {
                if (levels[j].getSpace() < min) {
                    found = j;
                    min = levels[j].getSpace();
                }
            }
        }
        if (found > -1) { // floor-pack on existing level
            packed.push_back(levels[found].put(unpacked[i]));
        } else {
            int found = -1;
            int min = RenderArea::STRIPW;
            for (int j = 0; j < levels.size(); j++) {
                if (levels[j].ceilingFeasible(unpacked[i], packed)) {
                    if (levels[j].getSpace(false) < min) {
                        found = j;
                        min = levels[j].getSpace(false);
                    }
                }
            }
            if (found > -1) { // ceiling-pack on existing level
                packed.push_back(levels[found].put(unpacked[i], false));
            } else { // a new level
                Packager::Level newLevel(levels.last().bottom + levels.last().height,
                                         unpacked[i].height(), 0, unpacked[i].width());
                packed.push_back(newLevel.put(unpacked[i]));
                levels.push_back(newLevel);
            }
        }
    }
    return packed;
}

