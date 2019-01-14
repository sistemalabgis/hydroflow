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

#include "observacao.h"

Observacao::Observacao()
{
}

void Observacao::limparObservacoes()
{
    item_observacao *reg = 0;
    for (unsigned int i = 0; i < this->lista.size(); ++i) {
        reg = this->lista.at(i);
        delete reg;
        reg = 0;
    }
    this->lista.clear();
    vector<item_observacao*> lst;
    this->lista.swap(lst);
}

void Observacao::setObservacao(int idFeicao, QString texto)
{
    item_observacao *reg, *item;
    bool encontrou = false;

    if (!this->lista.empty()) {
        for (unsigned int pos = 0; pos < this->lista.size(); ++pos) {
            item = this->lista[pos];
            if (item->iDFeicao == idFeicao) {
                item->texto = texto;
                encontrou = true;
                break;
            }
        }
    }
    if (!encontrou) {
        reg = new item_observacao();
        reg->iDFeicao = idFeicao;
        reg->texto = texto;
        this->lista.push_back(reg);
    }
}

QString Observacao::getObservacao(int idFeicao)
{
    QString texto = "";
    item_observacao *reg;

    if (!this->lista.empty()) {
        for (unsigned int pos = 0; pos < this->lista.size(); ++pos) {
            reg = this->lista[pos];
            if (reg->iDFeicao == idFeicao) {
                texto = reg->texto;
                break;
            }
        }
    }
    return texto;
}

int Observacao::getQuantidadeObservacoes()
{
    return this->lista.size();
}

bool Observacao::temObservacao()
{
    return !this->lista.empty();
}
