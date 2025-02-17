#ifndef CHATPAGE1_H
#define CHATPAGE1_H

#include <QWidget>

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit chat_page(QWidget *parent = nullptr);
    ~chat_page();

private:
    Ui::chat_page *ui;
};

#endif // CHATPAGE1_H
