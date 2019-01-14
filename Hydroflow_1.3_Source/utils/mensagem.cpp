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

#include "mensagem.h"
#include <QObject>

Mensagem::Mensagem(Parametros *params)
{
    this->params = params;
    this->tempo = QDateTime::currentDateTime();
    this->resultado = "";
}

Mensagem::~Mensagem()
{
    vector<QString> lst;
    this->lista.swap(lst);
}

void Mensagem::append(QString texto)
{
    this->lista.push_back(texto);
}

void Mensagem::setResultado(QString resultado)
{
    this->resultado = resultado;
}

QString Mensagem::getCabecalho()
{
    QString temp_1 = "";
    QString comp = "";
    QString temp_2 = "";
    QString temp_3 = "";
    QString temp_res = "";
    char classificacao = 0;

    // Definindo a inferência.
    switch (params->getTipoOrdemStrahler()) {
    case 1:
        temp_2 = QObject::tr("Ordens por Strahler");
        classificacao = classificacao + 1;
        break;
    case 2:
        temp_2 = QObject::tr("Ordens por Strahler relaxado");
        classificacao = classificacao + 1;
        break;
    }
    if (params->getEOrdemShreve()) {
        classificacao = classificacao + 2;
    }
    switch (classificacao) {
    case 0:
        comp = "";
        temp_3 = QObject::tr("Somente Fluxo.");
        break;
    case 1:
        comp = ".";
        break;
    case 2:
        comp = "";
        temp_3 = QObject::tr("Ordens por Shreve.");
        break;
    case 3:
        comp = ";\n   ";
        temp_3 = QObject::tr("Ordens por Shreve.");
        break;
    }
    temp_1 = "   " + temp_2 + comp + temp_3;

    // Definindo se há arquivo de resultado.
    if (this->resultado != "") {
        temp_res = "\n" + QObject::tr("Resultado: ") + this->resultado;
    }

    QString texto = "HydroFlow 1.3\n=============\n"
            + QObject::tr("Processamento: ")
            + this->getDataHora()
            + "\n" + QObject::tr("Tolerância: ") + QString::number(params->getToleranciaXY(), 'f', 12)
            + "\n" + QObject::tr("Rede de drenagem: ") + params->getNomeArquivoBacia()
            + "\n" + QObject::tr("Limite da área: ") + params->getNomeArquivoLimite()
            + temp_res
            + "\n" + QObject::tr("Inferir: ")
            + "\n" + temp_1
            + "\n-------------------------------------------------------------------------------------------------------------------";
    return texto;
}

QString Mensagem::getRodape()
{
    return "-------------------------------------------------------------------------------------------------------------------\n" + QObject::tr("Fim do log.");
}

bool Mensagem::haMensagem()
{
    return (this->lista.size() > 0);
}

int Mensagem::getTamanho()
{
    return this->lista.size();
}

QString Mensagem::getMensagem(unsigned int indice)
{
    if ((indice < this->lista.size()) && (!this->lista.empty())) {
        return this->lista.at(indice);
    } else {
        return "";
    }
}

QString Mensagem::getDataHora()
{
    return this->tempo.toString("dd/MM/yyyy  hh:mm:ss");
}
