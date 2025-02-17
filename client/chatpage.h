#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();

private:
    Ui::ChatPage *ui;

protected:
    //void paintEvent(QPaintEvent *event);
private slots:
    void on_send_btn_clicked();
};

#endif // CHATPAGE_H
