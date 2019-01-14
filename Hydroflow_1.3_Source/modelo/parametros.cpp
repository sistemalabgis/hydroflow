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

#include "parametros.h"


Parametros::Parametros()
{
    this->nomeArquivoBacia = "";
    this->nomeArquivoLimite = "";
    this->ToleranciaXY = 0;
    this->tipoOrdemStrahler = 0;
    this->eOrdemShreve = false;
    this->origem = 0;
}

Parametros::Parametros(QString bacia, QString limite, double tolerancia, char tipoStrahler, bool Shreve, QWidget *origem)
{
    this->nomeArquivoBacia = bacia;
    this->nomeArquivoLimite = limite;
    this->ToleranciaXY = tolerancia;
    this->tipoOrdemStrahler = tipoStrahler;
    this->eOrdemShreve = Shreve;
    this->origem = origem;
}

QString Parametros::getNomeArquivoBacia() const
{
    return this->nomeArquivoBacia;
}

QString Parametros::getNomeArquivoLimite() const
{
    return this->nomeArquivoLimite;
}

double Parametros::getToleranciaXY()
{
    return this->ToleranciaXY;
}

char Parametros::getTipoOrdemStrahler() const
{
    return tipoOrdemStrahler;
}

void Parametros::setTipoOrdemStrahler(const char &valor)
{
    this->tipoOrdemStrahler = valor;
}

bool Parametros::getEOrdemShreve()
{
    return this->eOrdemShreve;
}

QWidget *Parametros::getOrigem() const
{
    return origem;
}

QString Parametros::getNomeNovoArquivo() const
{
    return nomeNovoArquivo;
}

void Parametros::setNomeNovoArquivo(const QString &value)
{
    nomeNovoArquivo = value;
}
