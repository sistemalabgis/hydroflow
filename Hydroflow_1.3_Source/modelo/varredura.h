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

#ifndef VARREDURA_H
#define VARREDURA_H

#include "varredura_item.h"
#include "modelo/pontovarredura.h"
#include "utils/geometria.h"
#include "modelo/segmento.h"
#include "modelo/feicao.h"

using namespace std;

class Varredura
{
private:
    vector<VarreduraItem*> linhas;
    vector<PontoVarredura*> pontosVarredura;
    Geometria *geo;

    int compararItensVarredura2(VarreduraItem *primeiro, VarreduraItem *segundo);
    int compararPontosVarredura(Vertice *primeiro, Vertice *segundo);
    int compararLinhasVarredura(double linhaVarredura, Vertice *ponto);
    void inserir(unsigned int inicio, unsigned int fim, VarreduraItem *item);
    PontoVarredura* criarPontoVarredura(Vertice *ponto, Segmento *segmentoA, Segmento *segmentoB);
    void inserirPontoVarredura(unsigned int inicio, unsigned int fim, VarreduraItem *itemVarredura); // Inserção binária.
    PontoVarredura* procurarPontoVarredura(unsigned int inicio, unsigned int fim, double linhaVarredura);
public:
    Varredura(Geometria *geo);
    void limparVarredura();
    void setLinhas(Feicao *feicoes, int nFeicoes);
    void setLinhas(const vector<Feicao*>& listaFeicoesNovas);
    VarreduraItem* obterEvento();
    void inserir(VarreduraItem *item);
    int getQuantidadeLinhas();
    void ordenarLinhas();

    //PontoVarredura
    void inserirPontoVarredura(VarreduraItem *itemVarredura);
    PontoVarredura* getPontoVarredura(double linhaVarredura);
};

#endif // VARREDURA_H
