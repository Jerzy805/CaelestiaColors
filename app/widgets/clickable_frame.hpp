#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QString>
#include <utility> // for std::move
#include <QStyle>

class ClickableFrame final : public QFrame
{
    Q_OBJECT
    QString m_name;
    QString m_hex;

    public:
    explicit ClickableFrame(QString name, QString hex, QWidget* parent = nullptr)
        : QFrame(parent), m_name(std::move(name)), m_hex(std::move(hex))
    {
        setCursor(Qt::PointingHandCursor);
    }

    [[nodiscard]] const QString& name() const noexcept
    {
        return m_name;
    }

    [[nodiscard]] const QString& hex() const noexcept
    {
        return m_hex;
    }

    void setSelected(bool selected)
    {
        setProperty("selected", selected);
        style()->unpolish(this);
        style()->polish(this);
        update();
    }

    void setHex(QString hex)
    {
        m_hex = std::move(hex);
    }

    signals:
    void clicked();

    protected:
    
    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton && 
            rect().contains(event->position().toPoint()))
        {
            emit clicked();
        }

        QFrame::mouseReleaseEvent(event);
    }
};