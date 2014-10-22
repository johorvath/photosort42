#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_sort_clicked();    

    void on_pushButton_compareInput_clicked();

    void on_pushButton_photoInput_clicked();

private:
    Ui::MainWindow *ui;
    std::vector < std::string > input_files_;
};

#endif // MAINWINDOW_H
