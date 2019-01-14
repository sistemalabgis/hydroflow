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

#ifndef PARAMETROS_H
#define PARAMETROS_H

#include <QString>
#include <QWidget>

using namespace std;

class Parametros
{
private:
    QString nomeArquivoBacia;
    QString nomeArquivoLimite;
    QString nomeNovoArquivo;
    double ToleranciaXY;
    char tipoOrdemStrahler; //0-não classifica; 1-classifica Strahler clássica; 2-classifica Strahler modificada.
    bool eOrdemShreve;
    QWidget *origem;

public:
    Parametros();
    Parametros(QString bacia, QString limite, double tolerancia, char tipoStrahler, bool Shreve, QWidget *origem);

    QString getNomeArquivoBacia() const;
    QString getNomeArquivoLimite() const;
    double getToleranciaXY();
    char getTipoOrdemStrahler() const;
    void setTipoOrdemStrahler(const char &valor);
    bool getEOrdemShreve();
    QWidget *getOrigem() const;
    QString getNomeNovoArquivo() const;
    void setNomeNovoArquivo(const QString &value);

};

#endif // PARAMETROS_H
