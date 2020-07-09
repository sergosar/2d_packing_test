#include <QtGui>

#include "renderarea.h"
#include "packager2.h"


void RenderArea::setSTRIPH(int value)
{
    STRIPH = value;
}

void RenderArea::setSTRIPW(int value)
{
    STRIPW = value;
}

QList<QString> RenderArea::getUnList()
{
    return packager2.unList();
}

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent), STRIPH(-1), STRIPW(-1),packager2()
{

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

//    packager.init(":/data.txt");
//    packager.UseAlgorithm();
}



void RenderArea::fillArea(QList<QRect> rects, int H, int W)
{
    packager2.init(rects, H , W);
    packager2.UseAlgorithm();
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setPen(QPen(QBrush(Qt::lightGray, Qt::SolidPattern), 3.0, Qt::DotLine));

    painter.save();
    if(STRIPH!=-1 && STRIPW!=-1)
        painter.drawRect(10, 10, STRIPW, STRIPH );
    painter.restore();

    painter.setPen(QPen(Qt::black));

    int minY = STRIPH;
    for (int i = 0; i < packager2.getSize(); i++) {
        if (packager2.rectangles[i].y() < minY) minY = packager2.rectangles[i].y();
        painter.setBrush(QBrush(QColor(255,100, 1).lighter(120-(8*i)%120),
                                Qt::SolidPattern));
        painter.save();
        painter.drawRect(packager2.rectangles[i].translated(QPoint(10,10)));
        painter.restore();
    }

}


