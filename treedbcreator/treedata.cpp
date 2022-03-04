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

#include "treedata.h"

TreeData::TreeData()
{
    clear();
}

QString TreeData::codi() const
{
    return m_codi;
}

void TreeData::setCodi(const QString &codi)
{
    m_codi = codi;
}

qreal TreeData::latitud() const
{
    return m_latitud;
}

void TreeData::setLatitud(const qreal &latitud)
{
    m_latitud = latitud;
}

qreal TreeData::longitud() const
{
    return m_longitud;
}

void TreeData::setLongitud(const qreal &longitud)
{
    m_longitud = longitud;
}

QString TreeData::adreca() const
{
    return m_adreca;
}

void TreeData::setAdreca(const QString &adreca)
{
    m_adreca = adreca;
}

int TreeData::catEspecieId() const
{
    return m_catEspecieId;
}

void TreeData::setCatEspecieId(const int catEspecieId)
{
    m_catEspecieId = catEspecieId;
}

QString TreeData::nomCientific() const
{
    return m_nomCientific;
}

void TreeData::setNomCientific(const QString &nomCientific)
{
    m_nomCientific = nomCientific;
}

QString TreeData::nomEsp() const
{
    return m_nomEsp;
}

void TreeData::setNomEsp(const QString &nomEsp)
{
    m_nomEsp = nomEsp;
}

QString TreeData::nomCat() const
{
    return m_nomCat;
}

void TreeData::setNomCat(const QString &nomCat)
{
    m_nomCat = nomCat;
}

void TreeData::clear()
{
    m_codi.clear();
    m_latitud = 0;
    m_longitud = 0;
    m_adreca.clear();
    m_catEspecieId = 0;
    m_nomCientific.clear();
    m_nomEsp.clear();
    m_nomCat.clear();
}
