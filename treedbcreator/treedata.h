#ifndef ARVIARDATA_H
#define ARVIARDATA_H
#include <QtGlobal>
#include <QString>

class TreeData
{
public:
    TreeData();

    QString codi() const;
    void setCodi(const QString &codi);

    qreal posicioX_ETRS89() const;
    void setPosicioX_ETRS89(const qreal &posicioX_ETRS89);

    qreal posicioY_ETRS89() const;
    void setPosicioY_ETRS89(const qreal &posicioY_ETRS89);

    qreal latitud_WGS84() const;
    void setLatitud_WGS84(const qreal &latitud_WGS84);

    qreal longitud_WGS84() const;
    void setLongitud_WGS84(const qreal &longitud_WGS84);

    QString tipusElement() const;
    void setTipusElement(const QString &tipusElement);

    QString espaiVerd() const;
    void setEspaiVerd(const QString &espaiVerd);

    QString adreca() const;
    void setAdreca(const QString &adreca);

    QString alcada() const;
    void setAlcada(const QString &alcada);

    int catEspecieId() const;
    void setCatEspecieId(const int catEspecieId);

    QString nomCientific() const;
    void setNomCientific(const QString &nomCientific);

    QString nomEsp() const;
    void setNomEsp(const QString &nomEsp);

    QString nomCat() const;
    void setNomCat(const QString &nomCat);

    QString categoriaArbrat() const;
    void setCategoriaArbrat(const QString &categoriaArbrat);

    QString ampladaVorera() const;
    void setAmpladaVorera(const QString &ampladaVorera);

    QString plantacioDT() const;
    void setPlantacioDT(const QString &plantacioDT);

    QString tipAigua() const;
    void setTipAigua(const QString &tipAigua);

    QString tipReg() const;
    void setTipReg(const QString &tipReg);

    QString tipSuperf() const;
    void setTipSuperf(const QString &tipSuperf);

    QString tipSuport() const;
    void setTipSuport(const QString &tipSuport);

    QString cobertaEscocell() const;
    void setCobertaEscocell(const QString &cobertaEscocell);

    QString midaEscocell() const;
    void setMidaEscocell(const QString &midaEscocell);

    QString voraEscocell() const;
    void setVoraEscocell(const QString &voraEscocell);

    void clear();

private:
    QString m_codi;
    qreal m_posicioX_ETRS89;
    qreal m_posicioY_ETRS89;
    qreal m_latitud_WGS84;
    qreal m_longitud_WGS84;
    QString m_tipusElement;
    QString m_espaiVerd;
    QString m_adreca;
    QString m_alcada;
    int m_catEspecieId;
    QString m_nomCientific;
    QString m_nomEsp;
    QString m_nomCat;
    QString m_categoriaArbrat;
    QString m_ampladaVorera;
    QString m_plantacioDT;
    QString m_tipAigua;
    QString m_tipReg;
    QString m_tipSuperf;
    QString m_tipSuport;
    QString m_cobertaEscocell;
    QString m_midaEscocell;
    QString m_voraEscocell;
};

#endif // ARVIARDATA_H
