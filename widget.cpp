#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QtMath>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    startAngle = 150;
    run();//启动仪表盘动画

}

Widget::~Widget()
{
    delete ui;
}

void Widget::initCanvas(QPainter& painter) //初始化画布
{
    painter.setRenderHint(QPainter::Antialiasing,true);

    //底色弄成黑色
    painter.setBrush(Qt::black);
    painter.drawRect(rect());

    //坐标系 平移到中心
    QPoint centerPoint(rect().width()/2, rect().height()/2 + 40);
    painter.translate(centerPoint);
}

void Widget::drawMiddleCircle(QPainter& painter, int radius) //画中心圆
{
    // 画小圆
    painter.setPen(QPen(Qt::white,3));
    painter.drawEllipse(QPoint(0,0),radius,radius);
}

void  Widget::drawCurrentSpeed(QPainter& painter) //画中心圆中间的速度
{
    //当前值
    painter.setPen(Qt::white);
    QFont font("Arial", 25);
    font.setBold(true);
    painter.setFont(font);
   // painter.drawText(0,0,QString::number(currentValue));
    painter.drawText(QRect(-60,-60,120,70),Qt::AlignCenter,QString::number(currentValue*4));
    QFont font2("Arial", 13);
    font2.setBold(true);
    painter.setFont(font2);
    painter.drawText(QRect(-60,-60,120,160),Qt::AlignCenter,"km/h");
}

void Widget::drawScale(QPainter &painter, int radius) //画刻度线
{
    //画刻度
    //1. 算出一个刻度需要旋转的角度
    angle = 240*1.0 / 60;//270*1.0的作用是扩大变量类型，把int型阔成double,保留小数
    //2.  设置第一个刻度的位置
    painter.setFont(QFont("Arial",15));
    painter.setPen(QPen(Qt::white, 8));
    painter.save();//保存当前坐标位置，此时此刻是在原点，x在3点钟方向
    painter.rotate(startAngle);
    for(int i=0;i<=60;i++){
        if(i >= 40)
        {
            painter.setPen(QPen(Qt::red, 4));
        }
        else
        {
            painter.setPen(QPen(Qt::white, 4));
        }
        if(i % 5 == 0)
        {
            //画长的刻度线
            painter.drawLine(radius - 20, 0, radius - 3 ,0);
        }
        else
        {
            //画短的刻度线
            painter.drawLine(radius - 8, 0, radius - 3 ,0);
        }
        //画完后旋转
        painter.rotate(angle);
    }
    painter.restore();
}

void Widget::drawScaleText(QPainter &painter, int radius) //画刻度线上的文字
{
    QFont font("Arial", 15);
    font.setBold(true);
    painter.setFont(font);
    //利用坐标轴平移写刻度文字
    int R = radius - 50;
    for(int i = 0; i <= 60; i++)
    {

        if(i % 5 == 0)
        {
            //保存坐标系
            painter.save();
            //算出平移点
            int deltaX = qCos(qDegreesToRadians(210-angle*i)) * R; //QCOS和QSIN认的都是弧度，弧度=角度/180 * 3.14
            int deltaY = qSin(qDegreesToRadians(210-angle*i)) * R;
            //平移坐标系
            painter.translate(QPoint(deltaX, -deltaY));
            //旋转坐标系
            painter.rotate(-120+angle*i);
            //写上文字
            painter.drawText(-25,-25,50,30,Qt::AlignCenter, QString::number(i*4));
            //恢复坐标系
            painter.restore();
           }
    }

}

void Widget::drawPointer(QPainter &painter, int radius) //画指针
{
    //画指针，线
    //坐标轴先回到原点
    painter.restore();
    painter.save();

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    static const QPointF points[4] = {
        QPointF(radius,-10.0),
        QPointF(200.0,-1.0),
        QPointF(200.0, 1.0),
        QPointF(radius,10.0)
    };

    painter.rotate(startAngle + angle * currentValue);
    painter.drawPolygon(points, 4);
    //painter.drawLine(60,0,length,0);
}

void Widget::drawSpeedPie(QPainter &painter, int radius) //画指针划过的扇形
{
    painter.restore();
    QRect rentangle(-radius,-radius,radius*2,radius*2);
    painter.setPen(Qt::NoPen);
    QRadialGradient rd(0,0,radius);
    rd.setColorAt(1.0, QColor(0,0,0,0));
    rd.setColorAt(0.5, QColor(235,50,50,100));
    painter.setBrush(rd);
    painter.drawPie(rentangle,(360-startAngle)*16,-angle*currentValue*16);//angle前面取 负数，为了让它顺时针方向画
}

void Widget::drawEllipseInnerBlack(QPainter &painter, int radius)
{
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(0,0), radius, radius);
}

void Widget::drawEllipseInnerShine(QPainter &painter, int radius)
{
    QRadialGradient radialGradient(0,0,radius);
    radialGradient.setColorAt(0.0, QColor(255, 140, 17, 255));
    radialGradient.setColorAt(1.0, QColor(0,0,0,0));
    painter.setBrush(radialGradient);
    painter.drawEllipse(QPoint(0,0), radius, radius);
}

void Widget::drawOutterShine(QPainter &painter, int radius)
{
    painter.restore();
    QRect rentangle(-radius,-radius,radius*2,radius*2);
    painter.setPen(Qt::NoPen);

    QRadialGradient radialGradient(0,0,radius);
    radialGradient.setColorAt(1.0, QColor(255,0,0,255));
    radialGradient.setColorAt(0.97, QColor(255,0,0,70));
    radialGradient.setColorAt(0.8, QColor(0,0,0,0));
    radialGradient.setColorAt(0,QColor(0,0,0,0));
    painter.setBrush(radialGradient);
    painter.drawPie(rentangle,(360-150)*16,-angle*61*16);//angle前面取 负数，为了让它顺时针方向画
}

void Widget::drawLogo(QPainter &painter, int radius)
{
    QRect rectangle(-80, radius/2, 130, 50);
    painter.drawPixmap(rectangle, QPixmap(":/logo.png"));
}

void Widget::run() //启动仪表盘动画
{
    timer = new QTimer(this);
    currentValue = 0;
    connect(timer, &QTimer::timeout,[=](){
        if(mark == 0){
            currentValue++;
            if(currentValue >= 61){
                mark = 1;
            }

        }
        if(mark == 1){
            currentValue--;
            if(currentValue == 0){
                mark = 0;
            }
        }

        update();
    });
    timer->start(50);
}



void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    initCanvas(painter); //初始化画布

    //画小圆
    drawMiddleCircle(painter, 60);

   //画刻度线
    drawScale(painter, height()/2);

    //写刻度文字
    drawScaleText(painter, height()/2);

    //画指针
    //drawPointer(painter, height()/2 - 20- 10 - 28);
    drawPointer(painter, 60);

    //画扇形
    drawSpeedPie(painter, height()/2);

    //画内圈渐变色
    drawEllipseInnerShine(painter, 120);

    //画黑色内圈
    drawEllipseInnerBlack(painter, 80);

    //中心处画当前速度
    drawCurrentSpeed(painter);

    //画外环发光圈
    drawOutterShine(painter, (height()/2)+15);

    //画一个logo
    drawLogo(painter, height()/2);

}

