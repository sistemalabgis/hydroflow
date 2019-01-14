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

#include "pontovarredura.h"

PontoVarredura::PontoVarredura(Vertice *vertice, Segmento *segmento)
{
    this->ponto = vertice;
    this->lista.push_back(segmento);
}

void PontoVarredura::limparPontoVarredura()
{
    delete ponto;
    ponto = 0;
    this->lista.clear();
}

Vertice* PontoVarredura::getPonto()
{
    return this->ponto;
}

void PontoVarredura::inserirSegmento(Segmento *segmento)
{
    this->inserirSegmento(0, (this->lista.size() - 1), segmento);
}

void PontoVarredura::inserirSegmento(int inicio, int fim, Segmento *segmento)
{
    if (inicio <= fim) {

        // Obtendo o registro central.
        int meio = (int) (inicio + fim) / 2;
        Segmento *item = this->lista.at(meio);

        // Comparando os segmentos.
        int comp = segmento->compararSegmentos(item);

        // Segmento menor que item central.
        if (comp < 0) {
            if ((meio == 0) || (inicio == fim)) {
                this->lista.insert(this->lista.begin() + meio, segmento);
            } else {
                this->inserirSegmento(0, (meio - 1), segmento);
            }

        // Segmento maior que item central.
        } else if (comp > 0) {
            if (inicio < fim) {
                this->inserirSegmento((meio + 1), fim, segmento);
            } else {
                this->lista.push_back(segmento);
            }
        } //comp = 0; segmento já existe. Nada fazer!
    }
}

Segmento* PontoVarredura::getSegmento(int indice)
{
    return this->lista.at(indice);
}

int PontoVarredura::getQuantidadeSegmentos()
{
    return this->lista.size();
}
