/*  Copyright (C) 2015 Sistema Labgis of Instituto Multidisciplinar de
*   Formação Humana com Tecnologias of Universidade do Estado do Rio de Janeiro,
*   Alexandre Monteiro Reis and José Augusto Sapienza Ramos.
*
*   This file is part of HydroFlow.
*
*   HydroFlow is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "utils.h"
#include <QApplication>
#include <QFile>
#include <QXmlStreamReader>

Utils::Utils()
{

}

QString Utils::getIdioma()
{
    QString resposta = "";
    QFile arquivo(qApp->applicationDirPath() + "/hydroflow.xml");
    if (arquivo.open(QIODevice::ReadOnly)) {
        QXmlStreamReader xml;
        xml.setDevice(&arquivo);
        xml.readNext();
        while (!xml.atEnd()) {
            if (xml.isStartElement()) {
                if (xml.name() == "language") {
                    resposta = xml.readElementText();
                    break;
                }
            }
            xml.readNext();
        }
        arquivo.close();
    }
    return resposta;
}

char* Utils::converterString(QString texto)
{
    string textoS = texto.toLocal8Bit().constData();
    char *resposta = new char [textoS.length()+1];
    strcpy (resposta, textoS.c_str());
    return resposta;
}
