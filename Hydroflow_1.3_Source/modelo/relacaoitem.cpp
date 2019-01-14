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

#include "relacaoitem.h"

RelacaoItem::RelacaoItem()
{
    this->origem = 0;
    this->destino = 0;
    this->tipo = 0;
}

RelacaoItem::RelacaoItem(Segmento *origem, Segmento *destino, int tipo)
{
    this->origem = origem;
    this->destino = destino;
    this->tipo = tipo;
}

Segmento* RelacaoItem::getOrigem()
{
    return this->origem;
}

void RelacaoItem::setOrigem(Segmento *origem)
{
    this->origem = origem;
}

Segmento* RelacaoItem::getDestino()
{
    return this->destino;
}

void RelacaoItem::setDestino(Segmento *destino)
{
    this->destino = destino;
}

int RelacaoItem::getTipo()
{
    return this->tipo;
}
