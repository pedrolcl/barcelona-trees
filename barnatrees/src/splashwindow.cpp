/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2022 Pedro Lopez-Cabanillas <plcl@users.sf.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <QGuiApplication>
#include <QBackingStore>
#include <QResizeEvent>
#include <QPainter>
#include <QGradient>
#include <QFont>
#include <QScreen>
#include "splashwindow.h"

SplashWindow::SplashWindow(QWindow *parent)
    : QWindow(parent)
    , m_backingStore(new QBackingStore(this))
    , m_image(":/splash.png")
    , m_message(qApp->applicationName() + " " + qApp->applicationVersion())
{
    setFlags(Qt::SplashScreen | Qt::FramelessWindowHint);
    QRect r(QPoint(), m_image.size() / m_image.devicePixelRatio());
    QPoint c = screen()->geometry().center() - r.center();
    setGeometry(c.x(), c.y(), r.width(), r.height());
}

bool SplashWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

void SplashWindow::renderLater()
{
    requestUpdate();
}

void SplashWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    m_backingStore->resize(resizeEvent->size());
}

void SplashWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed())
        renderNow();
}

QString SplashWindow::message() const
{
    return m_message;
}

void SplashWindow::setMessage(const QString &message)
{
    m_message = message;
}

void SplashWindow::finish(QWindow* w)
{
    connect(w, &QWindow::visibleChanged, this, [=](bool visible){ if(visible) this->close(); });
}

void SplashWindow::centerInScreen(QScreen *s)
{
    setFlag(Qt::SplashScreen, false);
    QPoint c = s->availableGeometry().center() - geometry().center();
    setPosition(c);
}

void SplashWindow::renderNow()
{
    if (!isExposed())
        return;

    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    painter.fillRect(rect, QGradient::SaintPetersburg);
    painter.drawImage(rect, m_image);
    painter.setFont(QFont("Arial", 16, QFont::ExtraBold));
    render(&painter);
    painter.end();

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

void SplashWindow::render(QPainter *painter)
{
    QRectF rect(0, 0, width(), height());
    if (!m_message.isEmpty()) {
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignBottom, m_message);
    }
}
