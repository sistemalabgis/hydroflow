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

#ifndef PONTOVARREDURA_H
#define PONTOVARREDURA_H

#include <vector>
#include "modelo/pontovarredura.h"
#include "segmento.h"

using namespace std;

class PontoVarredura
{
private:
    Vertice *ponto;
    vector<Segmento*> lista; //Ordenação: idConjunto, idFeicao e id.
    void inserirSegmento(int inicio, int fim, Segmento *segmento);
public:
    PontoVarredura(Vertice *vertice, Segmento *segmento);
    void limparPontoVarredura();
    Vertice* getPonto();
    void inserirSegmento(Segmento *segmento);
    Segmento* getSegmento(int indice);
    int getQuantidadeSegmentos();
};

#endif // PONTOVARREDURA_H
