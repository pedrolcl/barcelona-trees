#ifndef ARVIARDATA_H
#define ARVIARDATA_H
/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2023 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
#include <QtGlobal>
#include <QString>

class TreeData
{
public:
    TreeData();

    QString codi() const;
    void setCodi(const QString &codi);

    qreal latitud() const;
    void setLatitud(const qreal &latitud);

    qreal longitud() const;
    void setLongitud(const qreal &longitud);

    QString adreca() const;
    void setAdreca(const QString &adreca);

    int catEspecieId() const;
    void setCatEspecieId(const int catEspecieId);

    QString nomCientific() const;
    void setNomCientific(const QString &nomCientific);

    QString nomEsp() const;
    void setNomEsp(const QString &nomEsp);

    QString nomCat() const;
    void setNomCat(const QString &nomCat);

    void clear();

private:
    QString m_codi;
    qreal m_latitud;
    qreal m_longitud;
    QString m_adreca;
    int m_catEspecieId;
    QString m_nomCientific;
    QString m_nomEsp;
    QString m_nomCat;
};

#endif // ARVIARDATA_H
