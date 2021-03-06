#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QLabel;
class QGridLayout;
class RenderArea;
class QTableWidget;
class QLineEdit;
class QComboBox;
class QScrollArea;
class QScrollBar;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:

    void addRow();
    void removeRow();

    void calculate();
    void clearTable();
    void saveRslt();

private:
    RenderArea *renderArea;
    QTableWidget *m_table;
    QLineEdit *m_wle;
    QLineEdit *m_hle;
    QComboBox *m_cbbx;
    QComboBox *m_changeAlgCB;
    QScrollArea *scrollArea;
    QScrollBar *qvsb;

    int cV(int row, int col);
    int m_width;
    int m_height;

    void pushRects(QList<QRect> &rects, int w, int h, int n);
};

#endif // WINDOW_H
