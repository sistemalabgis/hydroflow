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

#ifndef FEICAO_H
#define FEICAO_H

#include "vertice.h"
#include "segmento.h"

class Feicao
{
private:
    int id;
    int idConjunto;
    int idFeicaoFoz;
    int tipo; // De acordo com a especificação Shapefile.
    int nPartes;
    int fluxo;
    int strahler;
    int shreve;
    int nVertices;
    Vertice *vertices;
    int nSegmentos;
    Segmento *segmentos;
    bool processar;
    bool temObservacao;

public:
    Feicao();
    Feicao(int id, int idConjunto, int tipo, int nPartes, int nSegmentos, Segmento *segmentos);
    Feicao(int id, int idConjunto, int tipo, int nPartes, int nVertices, Vertice *vertices, int nSegmentos, Segmento *segmentos);
    void limparFeicao();
    int getId();
    int getIdConjunto();
    int getIdFeicaoFoz() const;
    void setIdFeicaoFoz(int valor);
    int getFluxo();
    int getStrahler();
    int getShreve();
    int getNVertices();
    int getNSegmentos();

    Vertice* getVertice(int indice);
    void setVertice(int indice, Vertice *vertice);
    Segmento* getSegmento(int indice);
    void setSegmento(int indice, Segmento segmento);
    Segmento* getSegmentos();

    int getTipo();
    int getNPartes();
    void setProcessar(bool valor);
    bool getProcessar();
    bool getTemObservacao();
    void setTemObservacao(bool valor);
    void setClassificacao(int &fluxo, int &strahler, int &shreve);

};

#endif // FEICAO_H
