#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QWidget>
#include "packager2.h"

class RenderArea : public QWidget
{
    Q_OBJECT

public:

    RenderArea(QWidget *parent = nullptr);

    void fillArea(QList<QSize> rects, int H, int W);
    void setSTRIPH(int value);
    void setSTRIPW(int value);
    QList<QString> getUnList();

    // Устанавливаем тип алгоритма, считанный из ComboBox'a
    void setAlgType( int alg ) { m_algType = alg; }

protected:
    void paintEvent(QPaintEvent *event);

private:
    int STRIPH;
    int STRIPW;
    int m_algType;

    Packager2 packager2;
};

#endif // RENDERAREA_H
