#include "buttonlabel.h"

#include <QPainter>
#include <QPainterPath>
#include <QRectF>

ButtonLabel::ButtonLabel(QWidget* parent) : QLabel(parent)
{
    m_states.push_back("normal");
    m_states.push_back("active");
    m_states.push_back("hoverd");

    this->setCursor(Qt::PointingHandCursor);
    this->setProperty("state", m_state);
    this->setState("normal");
    update();
}

void ButtonLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked(event);
    QLabel::mouseReleaseEvent(event);
}


QString ButtonLabel::state() const
{
    return m_state;
}

void ButtonLabel::setState(const QString &newState)
{
    if (m_state == newState)
        return;
    m_state = newState;

    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
    emit stateChanged();
}
