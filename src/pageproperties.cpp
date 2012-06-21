#include "pageproperties.h"
#include "ui_pageproperties.h"

PageProperties::PageProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageProperties)
{
    ui->setupUi(this);
}

PageProperties::~PageProperties()
{
    delete ui;
}
