/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QQuickView>
#include <QSslSocket>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QGeoCoordinate>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QQuickStyle>
#include <QTranslator>
#include <QLibraryInfo>
#include "plantmodel.h"
#include "speciesmodel.h"

int main(int argc, char **argv)
{
	QGuiApplication::setApplicationName("BarcelonaTrees");
	QGuiApplication::setOrganizationName("BarcelonaTrees");
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc,argv);
	app.setWindowIcon(QIcon("qrc:/barnatrees_icon64.png"));

	//qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
	//qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionString();
	//qDebug()<<QCoreApplication::libraryPaths();

	QSettings settings;
	QString style = QQuickStyle::name();
	if (style.isEmpty())
		QQuickStyle::setStyle(settings.value("style").toString());
	else
		settings.setValue("style", style);

	QString defLang = QLocale::system().name().left(2);
	QString configuredLanguage = settings.value("language", defLang).toString();
	QTranslator trq;
	QTranslator trp;
	QLocale locale(configuredLanguage);
	qDebug() << "locale:" << locale;
	if (trq.load(locale, QLatin1String("qt"), QLatin1String("_"), QLatin1String(":/"))) {
		QCoreApplication::installTranslator(&trq);
	} else {
		qWarning() << "Failure loading Qt5 translations for" << configuredLanguage;
	}
	if (trp.load(locale, QLatin1String("barnatrees"), QLatin1String("_"), QLatin1String(":/"))) {
		QCoreApplication::installTranslator(&trp);
	} else {
		qWarning() << "Failure loading program translations for" << configuredLanguage;
	}

    QGeoCoordinate locationBarna = QGeoCoordinate( 41.403216, 2.186674 );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setConnectOptions(QLatin1String("QSQLITE_OPEN_READONLY"));
#ifdef Q_OS_ANDROID
    QFileInfo orig("assets:/barnatrees.db");
	QDir destDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QFileInfo destFile(destDir, "barnatrees.db");
	if (!destFile.exists()) {
		QFile::copy(orig.filePath(), destFile.absoluteFilePath());
	}
	db.setDatabaseName(destFile.absoluteFilePath());
#else
    db.setDatabaseName("barnatrees.db");
#endif
    if (!db.open()) {
        qCritical() << db.lastError();
        return -1;
    }

	SpeciesModel speciesModel;
	qDebug() << "species.columns:" << speciesModel.columnCount();

	PlantModel plantModel;
	plantModel.setCenter(locationBarna);
	qDebug() << "plants.columns:" << plantModel.columnCount();

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("availableStyles", QQuickStyle::availableStyles());
	engine.rootContext()->setContextProperty("speciesModel", &speciesModel);
	engine.rootContext()->setContextProperty("plantModel", &plantModel);
	engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;
    return app.exec();
}
