#ifndef TIMERBUTTON_H
#define TIMERBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QTimerEvent>
#include <QDebug>

/*********************************************************************************
  * @Version:
  * @FileName:      timerbutton.h
  * @Brief:         定时按钮

  * @Author:        秋初
  * @Date:          2024-09-08
  * @History:
**********************************************************************************/
class TimerButton : public QPushButton
{
    Q_OBJECT
public:
    TimerButton(QWidget *parent = nullptr);
    ~TimerButton();
protected:
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *event) override;
private:
    QString m_btn_text;
    // 定时器ID
    int m_timerID;
    // 最终计数(秒)
    int m_interval;
    // 当前计数
    int m_counter;
};

#endif // TIMERBUTTON_H
