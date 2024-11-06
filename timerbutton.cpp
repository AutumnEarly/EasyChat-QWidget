#include "timerbutton.h"

TimerButton::TimerButton(QWidget *parent) : QPushButton(parent),
    m_btn_text(""), m_timerID(2001), m_interval(10), m_counter(m_interval)
{

}

TimerButton::~TimerButton()
{

}

void TimerButton::mouseReleaseEvent(QMouseEvent *e)
{
    this->setEnabled(false);
    m_counter = m_interval;
    m_btn_text = this->text();
    this->setText(QString::number(m_counter));
    m_timerID = startTimer(std::chrono::seconds(1));
    emit clicked();
    QPushButton::mouseReleaseEvent(e);
}

void TimerButton::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerID) {
        if(--m_counter > 0) {
            m_timerID = startTimer(std::chrono::seconds(1));
            this->setText(QString::number(m_counter));
//            qDebug() << "timerId " << event->timerId() << " " << m_counter;
        } else if(m_counter <= 0) {
            this->setText(m_btn_text);
            this->setEnabled(true);
        }
    }
}
