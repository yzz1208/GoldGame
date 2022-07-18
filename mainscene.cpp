#include "mainscene.h"
#include "ui_mainscene.h"
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include"chooselevelscene.h"
#include<QSound>

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //配置主场景

    //设置固定大小
    setFixedSize(320,588);
    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币主场景");

    //退出实现
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    //开始按钮音效
    QSound *startSound=new QSound(":/res/TapButtonSound.wav",this);
//     QSound *winSound=new QSound(":/res/LevelWinSound.wav",this);
//     winSound->setLoops(-1);//设置循环播放
//     winSound->play();

    //开始按钮
    MyPushButton *startBtn=new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);

    //实例化选择关卡的场景
    chooseSence=new ChooseLevelScene;

    //监听选择管卡的返回按钮的信号
    connect(chooseSence,&ChooseLevelScene::chooseSceneBack,this,[=](){

        this->setGeometry(chooseSence->geometry());
        chooseSence->hide();

        this->show();//重新显示主场景

    });

    connect(startBtn,&MyPushButton::clicked,[=](){
        qDebug()<<"开始";

        //播放开始音效
        startSound->play();
        startBtn->zoom1();
        startBtn->zoom2();

        //延时进入到管卡场景
        QTimer::singleShot(300,this,[=](){
            //进入选择关卡的场景中

            //设置chooseSence位置
            chooseSence->setGeometry(this->geometry());
            //自身隐藏
            this->hide();
            //显示选择关卡的场景
            chooseSence->show();

        });
    });
}

//重写paintEvent事件 画背景图
void MainScene:: paintEvent(QPaintEvent*)
{
    //创建画家
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //背景图标
    pix.load(":/res/Title.png");

    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);//缩放

    painter.drawPixmap(10,30,pix);
}

MainScene::~MainScene()
{
    delete ui;
}
