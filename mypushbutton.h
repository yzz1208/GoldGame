#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include<QPushButton>
#include<QDebug>

#include <QWidget>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = nullptr);

    //构造函数 参数1 正常显示的图片路径 参数2 按下后显示的图片路径
    MyPushButton(QString normalImg,QString pressImg="");

    //成员属性 保存用户传入的默认显示路径 以及按下后路径
    QString normalImgPath;
    QString pressImgPath;

    //弹跳特效
    void zoom1();//向下
    void zoom2();//向上

    //重写按钮 按下和释放
    void mousePressEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
