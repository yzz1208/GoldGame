#include "playgame.h"
#include<QDebug>
#include<QMenuBar>
#include<QPainter>
#include<QPixmap>
#include<mypushbutton.h>
#include<QTimer>
#include<QLabel>
#include"mycoin.h"
#include"dataconfig.h"
#include<QPropertyAnimation>
#include<QSound>

//PlayGame::PlayGame(QWidget *parent) : QMainWindow(parent)
//{

//}
PlayGame::PlayGame(int levelNum)
{
    QString str=QString("进入了第%1关").arg(levelNum);
    qDebug()<<str;
    this->levelIndex=levelNum;

    //初始化游戏场景
    //设定固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币场景");

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

    //添加音效
    QSound *backSound=new QSound(":/res/BackButtonSound.wav",this);

    QSound *flipSound=new QSound(":/res/ConFlipSound.wav",this);

    QSound *winSound=new QSound(":/res/LevelWinSound.wav",this);

    //返回
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
     backBtn->setParent(this);
     backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

     //点击返回按钮
     connect(backBtn,&MyPushButton::clicked,[=](){
         qDebug()<<"点击返回";

         backSound->play();

         QTimer::singleShot(300,this,[=](){
             emit this->chooseSceneBack();
         });

     });

     //显示当前关卡数
     QLabel *label=new QLabel;
     label->setParent(this);
     QFont font;

     font.setFamily("华文新魏");
     font.setPointSize(20);

     QString str1=QString("level:%1").arg(this->levelIndex);
     //将字体设置到标签控件中
     label->setFont(font);

     label->setText(str1);

     label->setGeometry(20,this->height()-50,200,50);


     dataConfig config;
     //初始化每个管卡的二维数组
     for(int i=0;i<4;i++)
     {
         for(int j=0;j<4;j++)
         {
             this->gameArray[i][j]=config.mData[this->levelIndex][i][j];
         }
     }

      //胜利图片显示
     QLabel *winLabel=new QLabel;
     QPixmap tmpPix;
     tmpPix.load(":/res/LevelCompletedDialogBg.png");
     winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
     winLabel->setPixmap(tmpPix);
     winLabel->setParent(this);
     winLabel->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());

     //显示金币背景图案
     //创建金币的背景图片
     for(int i=0;i<4;i++)
     {
         for(int j=0;j<4;j++)
         {
             //背景图片
             QLabel *label=new QLabel;
             label->setGeometry(0,0,50,50);
             label->setPixmap(QPixmap(":/res/BoardNode(1).png"));
             label->setParent(this);
             label->move(57+i*50,200+j*50);

             //创建金币
             if(this->gameArray[i][j]==1)
             {
                 //显示金币
                 str =":/res/Coin0001.png";

             }
             else
             {
                 //显示银币
                 str =":/res/Coin0008.png";
             }
             MyCoin *coin =new MyCoin(str);
             coin->setParent(this);
             coin->move(59+i*50,204+j*50);

             //给金币属性赋值
             coin->posX=i;
             coin->posY=j;
             coin->flag=this->gameArray[i][j];//1正面 2反面

             //将金币放入二维数组中 便于后期维护
             coinBtn[i][j]=coin;


             //点击金币 反转
             connect(coin,&MyCoin::clicked,[=]()
             {
                 flipSound->play();

                 //点击按钮 禁用所有按钮
                 for(int i=0;i<4;i++)
                 {
                     for(int j=0;j<4;j++)
                     {
                         this->coinBtn[i][j]->isWin=true;
                     }
                 }
                 coin->changeFlag();
                 this->gameArray[i][j]=this->gameArray[i][j]==0?1:0;

                 //翻转周围金币
                 QTimer::singleShot(300,this,[=](){
                     if(coin->posX+1<=3)//周围右侧金币反转的条件
                     {
                         coinBtn[coin->posX+1][coin->posY]->changeFlag();
                         this->gameArray[coin->posX+1][coin->posY]=this->gameArray[coin->posX+1][coin->posY]==0?1:0;
                     }
                     if(coin->posX-1>=0)//左侧
                     {
                         coinBtn[coin->posX-1][coin->posY]->changeFlag();
                         this->gameArray[coin->posX-1][coin->posY]=this->gameArray[coin->posX-1][coin->posY]==0?1:0;
                     }
                     //上侧
                     if(coin->posY-1>=0)
                     {
                         coinBtn[coin->posX][coin->posY-1]->changeFlag();
                         this->gameArray[coin->posX][coin->posY-1]=this->gameArray[coin->posX][coin->posY-1]==0?1:0;
                     }

                     //下侧
                     if(coin->posY+1<=3)
                     {
                         coinBtn[coin->posX][coin->posY+1]->changeFlag();
                         this->gameArray[coin->posX][coin->posY+1]=this->gameArray[coin->posX][coin->posY+1]==0?1:0;
                     }

                     //翻转完成后解开禁用
                     for(int i=0;i<4;i++)
                     {
                         for(int j=0;j<4;j++)
                         {
                             this->coinBtn[i][j]->isWin=false;
                         }
                     }

                     //判断是否胜利
                     this->isWin=true;
                     for(int i=0;i<4;i++)
                     {
                         for(int j=0;j<4;j++)
                         {
                             if(coinBtn[i][j]->flag==false)//只要有一个为反面则算失败
                             {
                                 this->isWin=false;
                                 break;
                             }
                         }
                     }

                     if(this->isWin==true)
                     {
                         //胜利

                         winSound->play();

                         qDebug()<<"胜利";
                         //将所有胜利标志改为true，如果再次点击按钮 直接return 不响应
                         for(int i=0;i<4;i++)
                         {
                             for(int j=0;j<4;j++)
                             {
                                 coinBtn[i][j]->isWin=true;
                             }
                         }

                         //将胜利的图片移动下来
                         QPropertyAnimation*animation=new QPropertyAnimation(winLabel,"geometry");

                         //设置时间间隔
                         animation->setDuration(1000);
                         //设置开始位置
                         animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                         //设置结束位置
                         animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                         //设置返回曲线
                         animation->setEasingCurve(QEasingCurve::OutBounce);
                         //开始动画
                         animation->start();
                     }
                 });

             });
         }
     }



}
void PlayGame:: paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");

    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
