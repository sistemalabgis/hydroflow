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

#ifndef POSICAO_H
#define POSICAO_H

#include <vector>
#include "modelo/segmento.h"
#include "utils/geometria.h"
#include "utils/mensagem.h"

using namespace std;

typedef	struct
{
    Segmento *a;
    Segmento *b;
} Item_troca;

class Posicao
{
private:
    vector<Segmento*> lista;
    Geometria *geo;

    Mensagem *saida; // Remover após fase de testes.

    int buscaBinaria(unsigned int inicio, unsigned int fim, double linhaVarredura, Segmento *segmento);
    int compararPosicao(double linhaVarredura, Segmento *primeiro, Segmento *segundo);
public:
    Posicao(Geometria *geo, Mensagem *saida);
    void limparPosicao();
    int localizar(double linhaVarredura, Segmento *segmento);
    int inserir(Segmento *segmento);
    void excluir(unsigned int indice);
    Segmento* acima(unsigned int indice);
    Segmento* abaixo(unsigned int indice);
    void trocar(unsigned int primeiro, unsigned int segundo);

};

#endif // POSICAO_H
