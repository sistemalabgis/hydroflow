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

#include "varredura_item.h"

VarreduraItem::VarreduraItem()
{
    this->ponto = 0;
    this->tipoEvento = -1;
}

VarreduraItem::VarreduraItem(Vertice *ponto, Segmento *segmento, int tipo)
{
    this->ponto = ponto;
    segmentos.push_back(segmento);
    this->tipoEvento = tipo;
}

VarreduraItem::VarreduraItem(Vertice *ponto, Segmento *segmentoA, Segmento *segmentoB, int tipo)
{
    this->ponto = ponto;
    segmentos.push_back(segmentoA);
    segmentos.push_back(segmentoB);
    this->tipoEvento = tipo;
}

void VarreduraItem::limparVarreduraItem()
{
    delete this->ponto;
    this->ponto = 0;
}

Vertice* VarreduraItem::getPonto()
{
    return this->ponto;
}

double VarreduraItem::getX()
{
    return this->ponto->getX();
}

double VarreduraItem::getY()
{
    return this->ponto->getY();
}

Segmento* VarreduraItem::getSegmentoA()
{
    return this->segmentos.at(0);
}

Segmento* VarreduraItem::getSegmentoB()
{
    if (this->segmentos.size() == 2) {
        return this->segmentos.at(1);
    } else {
        Segmento *resposta = 0;
        return resposta;
    }
}

int VarreduraItem::getTipoEvento()
{
    return this->tipoEvento;
}

