#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPushButton>
#include <QString>
#include <QGridLayout>
#include <QLabel>>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QRightClickButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QRightClickButton(QWidget *parent = 0);

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void rightClicked();

public slots:

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void def_but();

    void clear();
    void set_flag();
private:
    Ui::MainWindow *ui;
    int number_of_mines;
    int  number_of_flags;
    int rows;
    int columns;
    QVector<QVector<QPair<QRightClickButton*, int>>> field;
    QGridLayout* grid;
    QWidget* wid;

    QGridLayout* grid_end;
    QWidget* wid_end;
    QLabel* lay_end;
      QLabel* flags;
    void printEnd(const QString& str) const;
    void createField();
    void setRandomMine();
    void coolField();
    void openUnits();
    bool isWin();
    void realPrint();
};
#endif // MAINWINDOW_H
