/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2020 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

qreal TreeData::posicioX_ETRS89() const
{
    return m_posicioX_ETRS89;
}

void TreeData::setPosicioX_ETRS89(const qreal &posicioX_ETRS89)
{
    m_posicioX_ETRS89 = posicioX_ETRS89;
}

qreal TreeData::posicioY_ETRS89() const
{
    return m_posicioY_ETRS89;
}

void TreeData::setPosicioY_ETRS89(const qreal &posicioY_ETRS89)
{
    m_posicioY_ETRS89 = posicioY_ETRS89;
}

qreal TreeData::latitud_WGS84() const
{
    return m_latitud_WGS84;
}

void TreeData::setLatitud_WGS84(const qreal &latitud_WGS84)
{
    m_latitud_WGS84 = latitud_WGS84;
}

qreal TreeData::longitud_WGS84() const
{
    return m_longitud_WGS84;
}

void TreeData::setLongitud_WGS84(const qreal &longitud_WGS84)
{
    m_longitud_WGS84 = longitud_WGS84;
}

QString TreeData::tipusElement() const
{
    return m_tipusElement;
}

void TreeData::setTipusElement(const QString &tipusElement)
{
    m_tipusElement = tipusElement;
}

QString TreeData::espaiVerd() const
{
    return m_espaiVerd;
}

void TreeData::setEspaiVerd(const QString &espaiVerd)
{
    m_espaiVerd = espaiVerd;
}

QString TreeData::adreca() const
{
    return m_adreca;
}

void TreeData::setAdreca(const QString &adreca)
{
    m_adreca = adreca;
}

QString TreeData::alcada() const
{
    return m_alcada;
}

void TreeData::setAlcada(const QString &alcada)
{
    m_alcada = alcada;
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

QString TreeData::categoriaArbrat() const
{
    return m_categoriaArbrat;
}

void TreeData::setCategoriaArbrat(const QString &categoriaArbrat)
{
    m_categoriaArbrat = categoriaArbrat;
}

QString TreeData::ampladaVorera() const
{
    return m_ampladaVorera;
}

void TreeData::setAmpladaVorera(const QString &ampladaVorera)
{
    m_ampladaVorera = ampladaVorera;
}

QString TreeData::plantacioDT() const
{
    return m_plantacioDT;
}

void TreeData::setPlantacioDT(const QString &plantacioDT)
{
    m_plantacioDT = plantacioDT;
}

QString TreeData::tipAigua() const
{
    return m_tipAigua;
}

void TreeData::setTipAigua(const QString &tipAigua)
{
    m_tipAigua = tipAigua;
}

QString TreeData::tipReg() const
{
    return m_tipReg;
}

void TreeData::setTipReg(const QString &tipReg)
{
    m_tipReg = tipReg;
}

QString TreeData::tipSuperf() const
{
    return m_tipSuperf;
}

void TreeData::setTipSuperf(const QString &tipSuperf)
{
    m_tipSuperf = tipSuperf;
}

QString TreeData::tipSuport() const
{
    return m_tipSuport;
}

void TreeData::setTipSuport(const QString &tipSuport)
{
    m_tipSuport = tipSuport;
}

QString TreeData::cobertaEscocell() const
{
    return m_cobertaEscocell;
}

void TreeData::setCobertaEscocell(const QString &cobertaEscocell)
{
    m_cobertaEscocell = cobertaEscocell;
}

QString TreeData::midaEscocell() const
{
    return m_midaEscocell;
}

void TreeData::setMidaEscocell(const QString &midaEscocell)
{
    m_midaEscocell = midaEscocell;
}

QString TreeData::voraEscocell() const
{
    return m_voraEscocell;
}

void TreeData::setVoraEscocell(const QString &voraEscocell)
{
    m_voraEscocell = voraEscocell;
}

void TreeData::clear()
{
    m_codi.clear();
    m_posicioX_ETRS89 = 0;
    m_posicioY_ETRS89 = 0;
    m_latitud_WGS84 = 0;
    m_longitud_WGS84 = 0;
    m_tipusElement.clear();
    m_espaiVerd.clear();
    m_adreca.clear();
    m_alcada.clear();
    m_catEspecieId = 0;
    m_nomCientific.clear();
    m_nomEsp.clear();
    m_nomCat.clear();
    m_categoriaArbrat.clear();
    m_ampladaVorera.clear();
    m_plantacioDT.clear();
    m_tipAigua.clear();
    m_tipReg.clear();
    m_tipSuperf.clear();
    m_tipSuport.clear();
    m_cobertaEscocell.clear();
    m_midaEscocell.clear();
    m_voraEscocell.clear();
}
