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
    static const int STRIPH = 500;
    static const int STRIPW = 286;
    RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setParameter(qreal param) {packager.SetParameter(param);}
    void reuseAlg() {packager.UseAlgorithm();}

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap pixmap;
    Packager packager;
    FCNR    floorCeil;
};

#endif // RENDERAREA_H
