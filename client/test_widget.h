#ifndef TEST_WIDGET_H
#define TEST_WIDGET_H

#include <QWidget>

namespace Ui {
class test_widget;
}

class test_widget : public QWidget
{
    Q_OBJECT

public:
    explicit test_widget(QWidget *parent = nullptr);
    ~test_widget();

private:
    Ui::test_widget *ui;
};

#endif // TEST_WIDGET_H
