#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QDialog>
#include "global.h"
namespace Ui {
class chat_dialog;
}

class chat_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit chat_dialog(QWidget *parent = nullptr);
    ~chat_dialog();
    void addChatUserList();
private:
    void ShowSearch(bool);
    Ui::chat_dialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
private slots:
    void slot_loading_chat_user();
};

#endif // CHAT_DIALOG_H
