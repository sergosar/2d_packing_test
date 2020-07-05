#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QWidget>

#include "packager.h"

class RenderArea : public QWidget
{
    Q_OBJECT

public:

    RenderArea(QWidget *parent = 0);


    void fillArea(QList<QRect> rects, int H, int W);
    void setSTRIPH(int value);
    void setSTRIPW(int value);
    QList<QString> getUnList();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int STRIPH;
    int STRIPW;

    Packager packager;
};

#endif // RENDERAREA_H
