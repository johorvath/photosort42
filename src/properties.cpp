#include "properties.h"
#include "ui_properties.h"

Properties::Properties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Properties)
{
    ui->setupUi(this);
}

Properties::~Properties()
{
    delete ui;
}

void Properties::on_pushButton_algorithm_clicked()
{
    if(ui->radioB_fisher->isChecked())
    {

    }
    else if(ui->radioB_eigen->isChecked())
    {

    }
    else if(ui->radioB_lbp->isChecked())
    {

    }
    else
    {
        //preset parameter
    }


}
