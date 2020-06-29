#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QtGui>


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

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 3);
    mainLayout->addWidget(paramSlider, 0, 3);
    mainLayout->addWidget(parLabel, 1, 3);

    ///////////////////////////////////////

    QLabel *sizeLbl = new QLabel("Размер полотна");
    QLabel *tableLbl = new QLabel("Перечень изделий");

    QLabel *hLbl = new QLabel("Высота, см: ");
    QLabel *wLbl = new QLabel("Ширина, см: ");

    m_hle = new QLineEdit;
    m_hle->setPlaceholderText("высота");
    m_hle->setFixedWidth(100);
    m_hle->setValidator(new QIntValidator(10,1000,this));

    m_wle = new QLineEdit;
    m_wle->setPlaceholderText("ширина");
    m_wle->setFixedWidth(100);
    m_wle->setValidator(new QIntValidator(10,1000,this));

    QHBoxLayout *hhbLt = new QHBoxLayout;
    hhbLt->addWidget(hLbl);
    hhbLt->addWidget(m_hle);

    QHBoxLayout *whbLt = new QHBoxLayout;
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
    colNames << "Высота" << "Ширина" << "Количество";
    m_table->setVerticalHeaderLabels(colNames);

    for (int i = 0; i < m_table->columnCount(); ++i)
        m_table->setColumnWidth(i, this->width() / 7.95 );

    QPushButton *calcBtn = new QPushButton("Рассчитать");

    mainLayout->addWidget(sizeLbl, 2, 0, Qt::AlignLeft );
    mainLayout->addWidget(tableLbl, 2, 2, Qt::AlignRight);
    mainLayout->addLayout(hhbLt, 3, 0, Qt::AlignLeft);
    mainLayout->addLayout(whbLt, 4, 0, Qt::AlignLeft);
    mainLayout->addLayout(bLt, 5, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_table, 3, 1, 3, 3);
    mainLayout->addWidget(calcBtn, 7, 1, 1, 2);

    setLayout(mainLayout);
    setWindowTitle(tr("Тестовое задание по двумерной упаковке"));

    connect(addBtn, &QPushButton::clicked, [=](){
        this->m_table->setRowCount(m_table->rowCount() + 1);
    } );

    connect(remBtn, SIGNAL(clicked()), this, SLOT(removeRow()) );
    connect(calcBtn, SIGNAL(clicked()), this, SLOT(calculate()));

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

void Window::removeRow()
{
    if (m_table->rowCount() > 0)
        m_table->removeRow( m_table->currentRow() != -1 ? m_table->currentRow() : m_table->rowCount() - 1 );
}

void Window::calculate()
{
    if (m_hle->text().isEmpty() && m_wle->text().isEmpty())
        return;

    m_height = m_hle->text().toInt();
    m_width = m_wle->text().toInt();

    if (m_table->rowCount() == 0)
        return;

    QList<QRect> rects;
    for(int row = 0; row < m_table->rowCount(); ++row)
        for(int count = 0; count < m_table->item(row, 2)->text().toInt(); ++count ){
            QRect rect;
            rect.setRect(0, 0, m_table->item(row, 1)->text().toInt(), m_table->item(row, 0)->text().toInt());
            rects.push_back(rect);
        }

    renderArea->fillArea(rects);
    renderArea->update();
}

