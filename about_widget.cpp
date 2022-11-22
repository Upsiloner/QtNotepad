#include "about_widget.h"
#include "ui_about_widget.h"

about_widget::about_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::about_widget)
{
    ui->setupUi(this);
    setFixedSize(550,400);
    setWindowTitle("关于此软件");
}

about_widget::~about_widget()
{
    delete ui;
}
