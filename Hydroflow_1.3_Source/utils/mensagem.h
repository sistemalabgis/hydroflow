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

#ifndef MENSAGEM_H
#define MENSAGEM_H

#include <vector>
#include "modelo/parametros.h"
#include <QString>
#include <QDateTime>

using namespace std;

class Mensagem
{
private:
    Parametros *params;
    QString resultado;
    QDateTime tempo;
    vector<QString> lista;
public:
    Mensagem(Parametros *params);
    ~Mensagem();
    void append(QString texto);
    void setResultado(QString resultado);
    QString getCabecalho();
    QString getRodape();
    bool haMensagem();
    int getTamanho();
    QString getMensagem(unsigned int indice);
    QString getDataHora();
};

#endif // MENSAGEM_H
