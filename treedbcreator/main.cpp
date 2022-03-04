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
#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include "jsonparser.h"
#include "downloadmanager.h"

//#define _STR(x) #x
//#define STRINGIFY(x) _STR(x)

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("treedbcreator");
    QCoreApplication::setOrganizationName("BarcelonaTrees");
    QCoreApplication::setApplicationVersion(QT_STRINGIFY(APPVER));
    QCoreApplication app(argc, argv);

    JsonParser parser;
    DownloadManager manager;
    QObject::connect(&manager, &DownloadManager::downloadReady, &parser, &JsonParser::addDataset);
    QObject::connect(&manager, &DownloadManager::done, &parser, &JsonParser::execute);
    QObject::connect(&parser, &JsonParser::done, &app, &QCoreApplication::quit);

    QTimer::singleShot(0, &manager, &DownloadManager::execute);
    parser.initDB();

    app.exec();
}
