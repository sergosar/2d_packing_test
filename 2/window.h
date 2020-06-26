#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>



class QLabel;
class QRadioButton;
class QGridLayout;
class QSlider;
class RenderArea;
class QLineEdit;
class QTableWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void algChanged();
    void inpChanged();
    void parChanged();

private:
    RenderArea *renderArea;
    QLabel *parLabel;
    QSlider *paramSlider;
    QLineEdit *m_hle;
    QLineEdit *m_wle;
    QTableWidget *m_table;
};

#endif // WINDOW_H
