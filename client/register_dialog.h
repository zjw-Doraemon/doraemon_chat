#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <QDialog>
#include "global.h"
namespace Ui {
class register_dialog;
}

class register_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit register_dialog(QWidget *parent = nullptr);
    ~register_dialog();
    void get_verify_code();
private:
    Ui::register_dialog *ui;
    void initHttpHandlers();
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
    void showTip(QString , bool);

private slots:
    void slot_reg_mod_finish(ReqId id, QString res,ErrorCodes err);
    void on_sure_btu_clicked();


    void on_cancel_btn_clicked();

private:
    QAction* eye ;
    QAction* eye_2;
signals:
    void show_mainwindows();

};

#endif // REGISTER_DIALOG_H
