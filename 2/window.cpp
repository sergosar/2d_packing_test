#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QtGui>
#include <QComboBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QSpinBox>


#include "renderarea.h"
#include "packager.h"
#include "window.h"

Window::Window()
{
    renderArea = new RenderArea;


    scrollArea = new QScrollArea;
    scrollArea->setWidget(renderArea);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    qvsb = scrollArea->verticalScrollBar();


    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(scrollArea, 0, 0, 1, 3);

    ///////////////////////////////////////

    QLabel *sizeLbl = new QLabel("Размер полотна");
    QLabel *tableLbl = new QLabel("Перечень изделий");

    QLabel *hLbl = new QLabel("Высота, см: ");
    QLabel *wLbl = new QLabel("Ширина, см: ");

    QLabel *resLbl = new QLabel("Не уложились");

    m_hle = new QLineEdit;
    m_hle->setPlaceholderText("высота");
    m_hle->setFixedWidth(100);
    m_hle->setValidator(new QIntValidator(10,9999,this));

    m_wle = new QLineEdit;
    m_wle->setPlaceholderText("ширина");
    m_wle->setFixedWidth(100);
    m_wle->setValidator(new QIntValidator(10,9999,this));

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
    colNames << "Высота" << "Ширина" << "Количество"; //, не работает , почему!!!                           !!!
    m_table->setHorizontalHeaderLabels(colNames);

    for (int i = 0; i < m_table->columnCount(); ++i)
        m_table->setColumnWidth(i, this->width() / 7.51 );

    QPushButton *calcBtn = new QPushButton("Рассчитать");
    QPushButton *clearBtn = new QPushButton("Очистить таблицу");

    m_cbbx = new QComboBox;


    mainLayout->addWidget(sizeLbl, 2, 0, Qt::AlignLeft );
    mainLayout->addWidget(tableLbl, 2, 2, Qt::AlignRight);
    mainLayout->addLayout(hhbLt, 3, 0, Qt::AlignLeft);
    mainLayout->addLayout(whbLt, 4, 0, Qt::AlignLeft);
    mainLayout->addLayout(bLt, 5, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_table, 3, 1, 3, 3 );
    mainLayout->addWidget(resLbl,6,0,Qt::AlignCenter);
    mainLayout->addWidget(calcBtn, 7, 1, 1, 1);
    mainLayout->addWidget(clearBtn,7,2);
    mainLayout->addWidget(m_cbbx, 7, 0, Qt::AlignJustify);

    mainLayout->sizeConstraint();

    setLayout(mainLayout);
    setWindowTitle(tr("Тестовое задание по двумерной упаковке"));
    this->resize(600,900);


    connect(addBtn, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(remBtn, SIGNAL(clicked()), this, SLOT(removeRow()) );
    connect(calcBtn, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(clearBtn, SIGNAL(clicked()), this, SLOT(clearTable()));

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
//    QString text;
//    text.setNum(paramSlider->value());
//    qreal value =   static_cast<qreal>(paramSlider->value())
//                  / static_cast<qreal>(100);

//    parLabel->setText("0."+text);
//    parLabel->update();

//    renderArea->setParameter(value);
//    renderArea->reuseAlg();
//    renderArea->update();
}

void Window::addRow()
{
    m_table->setRowCount(m_table->rowCount()+1);
    int rowNum = m_table->rowCount();

    for(int i=0; i< m_table->columnCount(); ++i){
        m_table->setCellWidget(rowNum-1,i,new QSpinBox);
        qobject_cast<QSpinBox*>(m_table->cellWidget(rowNum-1, i))->setMaximum(999);
    }

}

void Window::removeRow()
{
    if (m_table->rowCount() > 0)
        m_table->removeRow( m_table->currentRow() != -1 ? m_table->currentRow() : m_table->rowCount() - 1 ); 
}

int Window::cellValue(int row, int col)
{
    return qobject_cast<QSpinBox*>(m_table->cellWidget(row,col))->value();
}




void Window::calculate()
{
    m_cbbx->clear();
    if (m_hle->text().isEmpty() && m_wle->text().isEmpty())
        return;

    m_height = m_hle->text().toInt();
    m_width = m_wle->text().toInt();

    if (m_table->rowCount() == 0)
        return;

    QList<QRect> rects;
    for(int row = 0; row < m_table->rowCount(); ++row)
        if(cellValue(row, 0)>0 && cellValue(row,1)>0 && cellValue(row,2)>0){ // empty table rows testing
            for(int count = 0; count < cellValue(row,2); ++count){
                QRect rect;
                if(cellValue(row,0)>cellValue(row,1) && cellValue(row,0)<m_width) //
                    rect.setRect(0,0,cellValue(row,0),cellValue(row,1));
                else
                    rect.setRect(0, 0,cellValue(row,1),cellValue(row,0));
                rects.push_back(rect);
        }
    }
    renderArea->setSTRIPH(m_height);
    renderArea->setSTRIPW(m_width);
    renderArea->setFixedSize(m_width+15,m_height+15);
    renderArea->fillArea(rects, m_height, m_width);
    renderArea->update();

    qvsb->setValue(qvsb->maximum());
    scrollArea->update();

// заполнение неуложившихся элементов
    for(QString str: renderArea->getUnList()) {
        m_cbbx->addItem(str);
    }

}

void Window::clearTable()
{
    m_table->setRowCount(0);
}

