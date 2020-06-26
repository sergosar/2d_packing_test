#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QtGui>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTableWidget>

#include "renderarea.h"
#include "packager.h"
#include "window.h"

Window::Window()
{
    renderArea = new RenderArea;

    paramSlider = new QSlider(Qt::Vertical);
    paramSlider->setTickInterval(10);

    connect(paramSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(parChanged()));

    parLabel = new QLabel(tr("0"));
    parLabel->setBuddy(paramSlider);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(paramSlider, 0, 3);
    mainLayout->addWidget(parLabel, 1, 3);

    ////////////////////////////

    QLabel *sizeLbl = new QLabel ("Размер полотна");
    QLabel *tableLbl = new QLabel ("Перечень изделий");

    QLabel *hLbl = new QLabel("Высота, см:");
    QLabel *wLbl = new QLabel("Ширина, см");

    m_hle = new QLineEdit;
    m_hle->setPlaceholderText("высота");
    m_hle->setFixedWidth(100);

    m_wle = new QLineEdit;
    m_wle->setPlaceholderText("ширина");
    m_wle->setFixedWidth(100);

    QHBoxLayout *hhbLt = new QHBoxLayout;
    hhbLt->addWidget(hLbl);
    hhbLt->addWidget(m_hle);

    QHBoxLayout *whbLt= new QHBoxLayout;
    whbLt->addWidget(wLbl);
    whbLt->addWidget(m_wle);

    QPushButton *addBtn = new QPushButton("Добавить");
    QPushButton *remBtn = new QPushButton("Удалить");

    QHBoxLayout *bLt = new QHBoxLayout;
    bLt->addWidget(addBtn);
    bLt->addWidget(remBtn);

    m_table = new QTableWidget;
    m_table->setColumnCount(3);

    QStringList colNames;
    colNames<<"Высота"<<"Ширина"<<"Количество";
    m_table->setVerticalHeaderLabels(colNames);

    for(int i =0; i<m_table->colorCount(); i++){
        m_table->setColumnWidth(i,this->width()/6); ////
    }

    QPushButton *calcBtn = new QPushButton("Рассчитать");

    mainLayout->addWidget(sizeLbl, 2,0,Qt::AlignLeft);
    mainLayout->addWidget(tableLbl,2,2,Qt::AlignRight);
    mainLayout->addLayout(hhbLt,3,0,Qt::AlignLeft);
    mainLayout->addLayout(whbLt,4,0,Qt::AlignLeft);
    mainLayout->addLayout(bLt,5,0,Qt::AlignCenter);
    mainLayout->addWidget(m_table,3,1);
    mainLayout->addWidget(calcBtn,7,1,1,2);



    setLayout(mainLayout);
    setWindowTitle(tr("Тестовое задание по двумерной упаковке"));
}

void Window::algChanged()
{
//    renderArea->update();
}

void Window::inpChanged()
{
//    renderArea->update();
}

void Window::parChanged()
{
    QString text;
    text.setNum(paramSlider->value());
    qreal value =   static_cast<qreal>(paramSlider->value())
                  / static_cast<qreal>(100);

    parLabel->setText("0."+text);
    parLabel->update();

    renderArea->setParameter(value);
    renderArea->reuseAlg();
    renderArea->update();
}
