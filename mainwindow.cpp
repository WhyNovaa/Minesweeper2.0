#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
QWidget* temp;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //srand(time(NULL));
    ui->setupUi(this);
    this->setWindowTitle("MineSweeper");
    grid = new QGridLayout;
    wid = new QWidget;
    wid_end = new QWidget;
    lay_end = new QLabel;
    grid_end = new QGridLayout;
    flags = new QLabel;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int s1, s2, mines;
    bool flag = true;
    try{
        s2 = ui->size1->text().toInt();
        s1 = ui->size2->text().toInt();
        mines = ui->number_of_mines->text().toInt();
    }
    catch(...) {
        ui->status_->setText("Wrong size or number of mines");
        flag = false;
    }
    if(flag) {
        if(mines < 1 || mines > s1 * s2 || s1 < 1 || s2 < 1) {
            ui->status_->setText("Wrong size or number of mines");
        } else {
            this->close();
            rows = s1;
            columns = s2;
            number_of_mines = mines;
            number_of_flags = mines;
            createField();
        }
    }
}

void MainWindow::createField() {
    QVector<QVector<QPair<QRightClickButton*, int>>> v(rows, QVector<QPair<QRightClickButton*, int>>(columns));
    field = v;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            QRightClickButton* but = new QRightClickButton;
            but->setStyleSheet("background-color: AliceBlue;  border: 1px solid black");
            connect(but, SIGNAL( clicked() ), this, SLOT( def_but() ));
            connect(but, SIGNAL( rightClicked() ), this, SLOT( set_flag()));
            but->setFixedSize(25, 25);
            but->setText("");
            field[i][j] = {but, 0};
            grid->addWidget(but, i, j, Qt::AlignCenter);
        }
    }

    QPushButton* clear = new QPushButton();
    clear->setText("Clear");

    connect(clear, SIGNAL( clicked() ), this, SLOT( clear() ));
    QVBoxLayout *h1 = new QVBoxLayout;
    flags->setText("Number of flags: " + QString::number(number_of_flags));
    h1->addLayout(grid);
    h1->addWidget(flags);
    h1->addWidget(clear);

    wid->setLayout(h1);
    setCentralWidget(wid);
    grid->setVerticalSpacing(0);
    grid->setHorizontalSpacing(0);
    this->setFixedSize(25 * columns + 10, 25 * (rows + 1) + 10);

    //заполняю поле минами
    for(int i = 0; i < number_of_mines; i++) {
        setRandomMine();
    }

    //расставляю в каждую ячейку количчество мин вокруг
    coolField();

    this->show();
}

void MainWindow::setRandomMine()
{
    int x, y;
    //srand(time(NULL));
    while (true)
    {
        x = rand() % rows;
        y = rand() % columns;
        if (field[x][y].second != -1) {
            field[x][y].second = -1;
            return;
        }
    }
}

void MainWindow::coolField()
{
    int row_, column_, res;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (field[i][j].second == -1){
                continue;
            }
            res = 0;
            for (int a = -1; a < 2; a++) {
                for (int b = -1; b < 2; b++) {
                    if (a == 0 && b == 0) {
                        continue;
                    }
                    row_ = i + a;
                    column_ = j + b;
                    if (row_ >= 0 && row_ < rows && column_ >= 0 && column_ < columns) {
                        if (field[row_][column_].second == -1) {
                            res++;
                        }
                    }
                }
            }
            field[i][j].second = res;
        }
    }
}


void MainWindow::def_but() {
    QRightClickButton* button = (QRightClickButton*) sender();
    if(button->text() != "F") {
    int _row, _column, col_span, row_span;
    int index = grid->indexOf(button);
    grid->getItemPosition(index, &_row, &_column, &col_span, &row_span);
    button->setEnabled(false);

    if(field[_row][_column].second == -1) {
        button->setText("M");
        realPrint();
        //ЗДЕСЬ НАДО ПРОИГРЫШ СДЕЛАТЬ
        QMessageBox::StandardButton reply = QMessageBox::question(this, "End of the game", "You Lost :( Would you like to start a new game?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No) {
            QApplication::quit();
        }
        else {
            emit clear();
        }

    }
    else {
        if(field[_row][_column].second == 0) {
            button->setText("");
            openUnits();
        } else {
            button->setText(QString::number(field[_row][_column].second));
        }
        if(isWin()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "End of the game", "You Win! Would you like to start a new game?", QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::No) {
                QApplication::quit();
            }
            else {
                emit clear();
            }
        }
    }
    }
}

void MainWindow::openUnits()
{
    int row_, column_;
    bool flag = true;
    while (flag) {
        flag = false;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (field[i][j].second == 0 && !field[i][j].first->isEnabled()) {
                    for (int a = -1; a < 2; a++) {
                        for (int b = -1; b < 2; b++) {
                            if (a == 0 && b == 0) {
                                continue;
                            }
                            row_ = i + a;
                            column_ = j + b;
                            if (row_ >= 0 && row_ < rows && column_ >= 0 && column_ < columns) {
                                if (field[row_][column_].second == 0 && field[row_][column_].first->isEnabled()) {
                                    flag = true;
                                }
                                field[row_][column_].first->setEnabled(false);
                                if(field[row_][column_].second == 0) {
                                    field[row_][column_].first->setText("");
                                     field[row_][column_].first->setStyleSheet("background-color: Azure; border: 2px solid black");
                                }
                                else {
                                    field[row_][column_].first->setText(QString::number(field[row_][column_].second));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

bool MainWindow::isWin()
{
    for (long long i = 0; i < rows; i++) {
        for (long long j = 0; j < columns; j++) {
            if (field[i][j].first->isEnabled() && field[i][j].second != -1) {
                return false;
            }
        }
    }
    return true;
}
void MainWindow::set_flag() {
    QRightClickButton *b1 = (QRightClickButton *)sender();
    if(b1->text() != "F" ) {
        if(number_of_flags > 0) {
        b1->setText("F");
        number_of_flags--;
         flags->setText("Number of flags: " + QString::number(number_of_flags));
    }
    }
    else {
        b1->setText("");
        b1->setEnabled(true);
        number_of_flags++;
         flags->setText("Number of flags: " + QString::number(number_of_flags));
    }
}
void MainWindow::realPrint() {
    for(auto& i : field) {
        for(auto& j : i) {
            if(j.second == -1) {
                j.first->setText("М");
            }
            else if(j.second == 0) {
                j.first->setText("");
            }
            else {
                j.first->setText(QString::number(j.second));
            }
            j.first->setEnabled(false);
        }
    }
}

void MainWindow::clear() {
    number_of_flags = number_of_mines;
    flags->setText("Number of flags: " + QString::number(number_of_flags));
    for(auto& i : field) {
        for(auto& j : i) {
            j.first->setEnabled(true);
            j.second = 0;
            j.first->setText("");
            j.first->setStyleSheet("background-color: AliceBlue;  border: 1px solid black");
        }
    }
    for(int i = 0; i < number_of_mines; i++) {
        setRandomMine();
    }
    coolField();
    wid_end->close();
}

void MainWindow::printEnd(const QString& str) const {
    lay_end->setText(str);
    grid_end->addWidget(lay_end, 0, 0, Qt::AlignCenter);
    wid_end->setLayout(grid_end);
    wid_end->resize(400, 200);
    wid_end->show();
}
