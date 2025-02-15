#include "test_widget.h"
#include "ui_test_widget.h"

test_widget::test_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::test_widget)
{
    ui->setupUi(this);
}

test_widget::~test_widget()
{
    delete ui;
}
