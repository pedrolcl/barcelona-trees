#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H
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
#include <QWindow>

class SplashWindow : public QWindow
{
    Q_OBJECT
public:
    explicit SplashWindow(QWindow *parent = 0);
    virtual void render(QPainter *painter);

    QString message() const;
    void setMessage(const QString &message);
    void finish(QWindow* w);
    void centerInScreen(QScreen* s);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;

private:
    QBackingStore *m_backingStore;
    QImage  m_image;
    QString m_message;
};

#endif // SPLASHWINDOW_H
