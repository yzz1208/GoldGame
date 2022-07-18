#include "chooselevelscene.h"
#include<QMenuBar>
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include<QLabel>
#include"playgame.h"
#include<QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{

    //配置选择管卡场景
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择管卡场景");

    //创建菜单栏
    QMenuBar *bar=menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu* startMenu=bar->addMenu("开始");

    //创建退出菜单项
    QAction *quitAction=startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //选择关卡音效
    QSound*chooseSound=new QSound(":/res/TapButtonSound.wav",this);
    //返回按钮音效
    QSound*backSound=new QSound(":/res/BackButtonSound.wav",this);

    //返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
     backBtn->setParent(this);
     backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

     //点击返回按钮
     connect(backBtn,&MyPushButton::clicked,[=](){
         qDebug()<<"点击返回";

         //播放返回按钮音效
         backSound->play();
         //告诉主场景 我返回了 主场景监听ChooseLevelScene的返回按钮
         //延时
         QTimer::singleShot(300,this,[=](){
             emit this->chooseSceneBack();
         });

     });

     //创建选择关卡的按钮
     for(int i=0;i<20;i++)
     {
         MyPushButton*menuBtn=new MyPushButton(":/res/LevelIcon.png");
         menuBtn->setParent(this);
         menuBtn->move(25+i%4*70,130+i/4*70);

         connect(menuBtn,&MyPushButton::clicked,[=](){
             QString str=QString("您选择的是第%1 关").arg(i+1);
             qDebug()<<str;


             //播放选择关卡音效
             chooseSound->play();

             //进入到游戏场景
             this->hide();//隐藏选关场景

             play=new PlayGame(i+1);

             //设置游戏场景位置
             play->setGeometry(this->geometry());

             play->show();

             connect(play,&PlayGame::chooseSceneBack,[=](){

                 this->setGeometry(play->geometry());
                 this->show();

                 delete play;
                 play=NULL;
             });
         });

         QLabel *label=new QLabel;
         label->setParent(this);
         label->setFixedSize(menuBtn->width(),menuBtn->height());
         label->setText(QString::number(i+1));
         label->move(25+i%4*70,130+i/4*70);

         //设置对齐方式 水平居中 垂直居中
         label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

         //设置让鼠标穿透
         label->setAttribute(Qt::WA_TransparentForMouseEvents);
     }
}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");

    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //图标
    pix.load(":/res/Title.png");

    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix);

}
