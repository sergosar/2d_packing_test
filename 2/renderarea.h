#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

#include "packager.h"

class RenderArea : public QWidget
{
    Q_OBJECT

public:

    RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setParameter(qreal param) {packager.SetParameter(param);}
    void reuseAlg() {packager.UseAlgorithm();}

    void fillArea(QList<QRect> rects);

    int getSTRIPH() const;
    void setSTRIPH(int value);

    int getSTRIPW() const;
    void setSTRIPW(int value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int STRIPH;
    int STRIPW;
    QPixmap pixmap;
    Packager packager;
    FCNR    floorCeil;
};

#endif // RENDERAREA_H
