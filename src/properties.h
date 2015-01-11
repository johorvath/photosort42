#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>

namespace Ui {
class Properties;
}

class Properties : public QDialog
{
    Q_OBJECT

public:
    explicit Properties(QWidget *parent = 0);
    ~Properties();

private slots:
    void on_pushButton_algorithm_clicked();

private:
    Ui::Properties *ui;
};

#endif // PROPERTIES_H
