#ifndef MYCOIN_H
#define MYCOIN_H

#include <QWidget>
#include<QPushButton>
#include<QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyCoin(QWidget *parent = nullptr);

    //传入金币的路径
    MyCoin(QString btnImg);

    //金币的属性
    int posX;//坐标x
    int posY;//坐标y
    bool flag;//正反标志

    //改变标志的方法
    void changeFlag();

    QTimer *timer1;//正面翻反面
    QTimer *timer2;//反面翻正面

    int min=1;
    int max=8;

    //执行动画的标志
    bool isAnimation=false;

    //重写 按下
    void mousePressEvent(QMouseEvent *e);

    //是否胜利的标志
    bool isWin=false;



signals:

public slots:
};

#endif // MYCOIN_H
