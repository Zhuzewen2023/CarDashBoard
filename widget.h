#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Widget *ui;
    QTimer *timer;
    int currentValue;
    int mark = 0;
    double angle;
    double startAngle;
    void initCanvas(QPainter& painter);
    void drawMiddleCircle(QPainter& painter, int radius);
    void drawCurrentSpeed(QPainter& painter);
    void drawScale(QPainter& painter, int radius);
    void drawScaleText(QPainter& painter, int radius);
    void drawPointer(QPainter& painter, int length);
    void drawSpeedPie(QPainter& painter, int radius);
    void drawEllipseInnerBlack(QPainter& painter, int radius);
    void drawEllipseInnerShine(QPainter& painter, int radius);
    void drawOutterShine(QPainter& painter, int radius);
    void drawLogo(QPainter& painter, int radius);
    void run();
};
#endif // WIDGET_H
