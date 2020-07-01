#include <QtGui>

#include "renderarea.h"
#include "packager.h"

int RenderArea::getSTRIPH() const
{
    return STRIPH;
}

void RenderArea::setSTRIPH(int value)
{
    STRIPH = value;
}

int RenderArea::getSTRIPW() const
{
    return STRIPW;
}

void RenderArea::setSTRIPW(int value)
{
    STRIPW = value;
}

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    STRIPH = 500;
    STRIPW = 300;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

//    packager.init(":/data.txt");
//    packager.UseAlgorithm();
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(STRIPW / 4, STRIPH / 4);
}
QSize RenderArea::sizeHint() const
{
    return QSize(STRIPW, STRIPH);
}

void RenderArea::fillArea(QList<QRect> rects, int H, int W)
{
    packager.init(rects, H , W);
    packager.UseAlgorithm();

    QPainter painter(this);
    painter.setPen(QPen(QBrush(Qt::lightGray, Qt::SolidPattern), 3.0, Qt::DotLine));

    painter.save();
    painter.drawRect(0, 0, STRIPW, STRIPH);
    painter.restore();

    painter.setPen(QPen(Qt::black));

    int sumSpace = 0;
    int minY = STRIPH;
    for (int i = 0; i < packager.getSize(); i++) {
        if (packager.rectangles[i].y() < minY) minY = packager.rectangles[i].y();
        sumSpace += packager.rectangles[i].height() * packager.rectangles[i].width();
        painter.setBrush(QBrush(QColor(255, 200, 100).lighter(120 - 8 * i),
                                Qt::SolidPattern));
        painter.save();
        painter.drawRect(packager.rectangles[i].translated(QPoint(10,-10)));
        painter.restore();
    }
    qDebug("optimal = %d, obtained = %d", sumSpace / STRIPW, STRIPH - minY);

}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setPen(QPen(QBrush(Qt::lightGray, Qt::SolidPattern), 3.0, Qt::DotLine));

    painter.save();
    painter.drawRect(10, 0, STRIPW, STRIPH - 10);
    painter.restore();

    painter.setPen(QPen(Qt::black));

    int sumSpace = 0;
    int minY = STRIPH;
    for (int i = 0; i < packager.getSize(); i++) {
        if (packager.rectangles[i].y() < minY) minY = packager.rectangles[i].y();
        sumSpace += packager.rectangles[i].height() * packager.rectangles[i].width();
        painter.setBrush(QBrush(QColor(255, 200, 100).lighter(120 - 8 * i),
                                Qt::SolidPattern));
        painter.save();
        painter.drawRect(packager.rectangles[i].translated(QPoint(10,-10)));
        painter.restore();
    }
    qDebug("optimal = %d, obtained = %d", sumSpace / STRIPW, STRIPH - minY);
}


