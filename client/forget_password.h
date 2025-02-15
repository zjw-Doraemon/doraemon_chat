#ifndef FORGET_PASSWORD_H
#define FORGET_PASSWORD_H

#include <QWidget>
#include <global.h>
namespace Ui {
class forget_password;
}

class forget_password : public QWidget
{
    Q_OBJECT

public:
    explicit forget_password(QWidget *parent = nullptr);
    ~forget_password();
signals:
    void switch_login_dia();
private slots:
    void on_pushButton_4_clicked();
    void on_get_varefy_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void inithandle_respond();
    void on_yes_clicked();

private:
    Ui::forget_password *ui;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
};

#endif // FORGET_PASSWORD_H
