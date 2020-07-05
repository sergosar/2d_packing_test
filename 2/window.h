#ifndef WINDOW_H
#define WINDOW_H
#include "renderarea.h"

#include <QComboBox>
#include <QScrollArea>
#include <QWidget>
#include <QScrollBar>
#include <QTableWidget>
#include <QLabel>



class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:

    void addRow();
    void removeRow();
    int cellValue(int row, int col);
    void calculate();
    void clearTable();

private:
    RenderArea *renderArea;
    QLabel *parLabel;
    QSlider *paramSlider;
    QTableWidget *m_table;
    QLineEdit *m_wle;
    QLineEdit *m_hle;
    QComboBox *m_cbbx;
    QScrollArea *scrollArea;
    QScrollBar *qvsb;


    int m_width;
    int m_height;

};

#endif // WINDOW_H
