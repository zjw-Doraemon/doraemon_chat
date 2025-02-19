#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QDialog>
#include "global.h"
#include "statewidget.h"
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
    void AddLBGroup(StateWidget* lb);
    void ShowSearch(bool);
    Ui::chat_dialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*> _lb_list;
    QWidget* _last_widget;
private slots:
    void ClearLabelState(StateWidget *lb);
    void slot_side_chat();
    void slot_loading_chat_user();
    void slot_side_contact();
    void slot_text_changed(const QString &str);
};

#endif // CHAT_DIALOG_H
