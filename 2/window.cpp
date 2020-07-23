#include <QGridLayout>
#include <QHBoxLayout>

#include <QLineEdit>
#include <QPushButton>
#include <QtGui>
#include <QSpinBox>
#include <QLabel>
#include <QScrollArea>
#include <QTableWidget>
#include <QComboBox>
#include <QScrollBar>
#include <QHeaderView>
#include <QFileDialog>
#include <QDir>



#include "renderarea.h"
#include "packager2.h"
#include "window.h"
using namespace std;
Window::Window()
{
    renderArea = new RenderArea;
    scrollArea = new QScrollArea;
    scrollArea->setWidget(renderArea);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    qvsb = scrollArea->verticalScrollBar();

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(scrollArea, 0, 0, 1, 4);

    ///////////////////////////////////////

    QLabel *sizeLbl = new QLabel("<b>Размер полотна</b>");
    QLabel *tableLbl = new QLabel("<b>Перечень изделий</b>");

    QLabel *hLbl = new QLabel("Высота, см: ");
    QLabel *wLbl = new QLabel("Ширина, см: ");

    QLabel *resLbl = new QLabel("Нераскроенные изделия:");

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
    colNames << "Высота" << "Ширина" << "Количество";
    m_table->setHorizontalHeaderLabels(colNames);


    QPushButton *calcBtn = new QPushButton("Рассчитать");
    QPushButton *clearBtn = new QPushButton("Очистить таблицу");
    QPushButton *saveBtn = new QPushButton("Сохранить раскорой");

    m_cbbx = new QComboBox;

    mainLayout->addWidget(sizeLbl, 2, 0, Qt::AlignLeft );
    mainLayout->addWidget(tableLbl, 2, 2, Qt::AlignRight);
    mainLayout->addLayout(hhbLt, 3, 0, Qt::AlignLeft);
    mainLayout->addLayout(whbLt, 4, 0, Qt::AlignLeft);
    mainLayout->addLayout(bLt, 5, 0, Qt::AlignCenter);
    mainLayout->addWidget(saveBtn, 6,0);
    mainLayout->addWidget(m_table, 3, 1, 3, 2);
    mainLayout->setColumnStretch(2, 2);
    mainLayout->setColumnStretch(1, 2);
    mainLayout->addWidget(resLbl, 7, 0, Qt::AlignCenter);
    mainLayout->addWidget(calcBtn, 7, 1, 1, 1);
    mainLayout->addWidget(clearBtn, 7, 2, 1, 1);
    mainLayout->addWidget(m_cbbx, 8, 0, 2, 1);



    setLayout(mainLayout);
    setWindowTitle("Тестовое задание по двумерной упаковке");
    this->resize(600,900);
    m_table->horizontalHeader()->setStretchLastSection(true);

    connect(addBtn, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(remBtn, SIGNAL(clicked()), this, SLOT(removeRow()) );
    connect(calcBtn, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(clearBtn, SIGNAL(clicked()), this, SLOT(clearTable()));
    connect(saveBtn, SIGNAL(clicked()),this,SLOT(saveRslt()));

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

int Window::cV(int row, int col) // cellValue of tableWшdget
{
    return qobject_cast<QSpinBox*>(m_table->cellWidget(row,col))->value();
}

void Window::pushRects(QList<QRect> &rects,int w,int h,int n){
    for(int i = 0; i<n; i++){
        QRect rect;
        rect.setRect(0, 0,w,h);
        rects.push_back(rect);
    }
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

   // QList<QRect> rects;
    QList<QSize> rects;
    for(int row = 0; row < m_table->rowCount(); ++row)
        if(cV(row, 0)>0 && cV(row,1)>0 && cV(row,2)>0){// empty table rows testing
            int w =cV(row,0);
            int h = cV(row,1);
            for(int count = 0; count < cV(row,2); ++count){
            rects.push_back(QSize(min(w,h), max(w,h)));
    }
    }
    renderArea->setSTRIPH(m_height);
    renderArea->setSTRIPW(m_width);
    renderArea->setFixedSize(m_width+15,m_height+15);
    renderArea->fillArea(rects, m_height, m_width);

    // заполнение неуложившихся элементов
    for(QString str: renderArea->getUnList()) {
        m_cbbx->addItem(str);
    }
    renderArea->update();

    qvsb->setValue(qvsb->maximum());
 //   scrollArea->update();


}

void Window::clearTable()
{
    m_table->setRowCount(0);
}

void Window::saveRslt()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("Images (*.png)" ));
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)){
        QPixmap image= renderArea->grab();
        image.save(&file, "PNG");
    };
}

