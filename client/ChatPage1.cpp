#include "ChatPage1.h"
#include "ui_ChatPage1.h"

chat_page::chat_page(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chat_page)
{
    ui->setupUi(this);
}

chat_page::~chat_page()
{
    delete ui;
}
