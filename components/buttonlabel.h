#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStyle>
#include <QVariant>

class ButtonLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged FINAL)
public:
    ButtonLabel(QWidget* parent = nullptr);
    QString state() const;
    void setState(const QString &newState);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked(QMouseEvent *event);
    void stateChanged();

private:
    QString m_state;
    QVector<QString> m_states;


};

#endif // BUTTONLABEL_H
