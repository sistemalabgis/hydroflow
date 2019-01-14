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

#ifndef EVENTO_H
#define EVENTO_H

#include <vector>
#include "vertice.h"
#include "segmento.h"

using namespace std;

class VarreduraItem
{
private:
    Vertice *ponto;
    vector<Segmento*> segmentos;
    int tipoEvento; // 0: entrada; 1: saída; 2: interseção.

public:
    VarreduraItem();
    VarreduraItem(Vertice *ponto, Segmento *segmento, int tipo);
    VarreduraItem(Vertice *ponto, Segmento *segmentoA, Segmento *segmentoB, int tipo);
    void limparVarreduraItem();
    Vertice* getPonto();
    double getX();
    double getY();
    Segmento* getSegmentoA();
    Segmento* getSegmentoB();
    int getTipoEvento();
};

#endif // EVENTO_H
