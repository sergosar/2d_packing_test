#ifndef WINDOW_H
#define WINDOW_H
#include <QComboBox>
#include <QScrollArea>
#include <QWidget>
#include <QScrollBar>

class QLabel;
class QRadioButton;
class QGridLayout;
class QSlider;
class RenderArea;
class QTableWidget;
class QLineEdit;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void algChanged();
    void inpChanged();
    void parChanged();

    void addRow();
    void removeRow();
    void calculate();

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
