#ifndef PLAYGAME_H
#define PLAYGAME_H

#include <QMainWindow>
#include"mycoin.h"

class PlayGame : public QMainWindow
{
    Q_OBJECT
public:
//    explicit PlayGame(QWidget *parent = nullptr);

    PlayGame(int levelNum);

    int levelIndex; //成员属性记录管卡

    //重写paintEvent事件

    void paintEvent(QPaintEvent*);

    int gameArray[4][4];//维护每个管卡属性

    MyCoin *coinBtn[4][4];

    //是否胜利的标志
    bool isWin;

signals:
        void chooseSceneBack();
public slots:
};

#endif // PLAYGAME_H
