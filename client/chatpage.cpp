#include "chatpage.h"
#include "ui_chatpage.h"
#include <QStyleOption>
#include <QPainter>

#include <QJsonArray>
#include <QJsonObject>

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
{
    ui->setupUi(this);
}

ChatPage::~ChatPage()
{
    delete ui;
}

// void ChatPage::paintEvent(QPaintEvent *event)
// {
//     QStyleOption opt;
//     opt.initFrom(this);
//     QPainter p(this);
//     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
// }
