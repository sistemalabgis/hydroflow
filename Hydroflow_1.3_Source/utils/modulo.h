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

#ifndef MODULO_H
#define MODULO_H

#include "geometria.h"
#include "modelo/varredura_item.h"
#include "modelo/indiceitem.h"

using namespace std;

void moduloSetGeometria(Geometria *geo);

// Funções de comparação do qsort.
bool compararItemVarreduraX(VarreduraItem *primeiro, VarreduraItem *segundo);
bool compararItemIndice(IndiceItem *primeiro, IndiceItem *segundo);

#endif // MODULO_H
