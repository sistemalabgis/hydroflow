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

#include "feicao.h"


Feicao::Feicao()
{
    this->id = -1;
    this->idConjunto = -1;
    this->idFeicaoFoz = -1;
    this->tipo = 0;
    this->nPartes = 0;
    this->fluxo = 0;
    this->strahler = 0;
    this->shreve = 0;
    this->nVertices = 0;
    this->vertices = 0;
    this->nSegmentos = 0;
    this->segmentos = 0;
    this->processar = true;
    this->temObservacao = false;
}

Feicao::Feicao(int id, int idConjunto, int tipo, int nPartes, int nSegmentos, Segmento *segmentos)
{
    this->id = id;
    this->idConjunto = idConjunto;
    this->idFeicaoFoz = -1;
    this->tipo = tipo;
    this->nPartes = nPartes;
    this->fluxo = 0;
    this->strahler = 0;
    this->shreve = 0;
    this->nVertices = 0;
    this->vertices = 0;
    this->nSegmentos = nSegmentos;
    this->segmentos = segmentos;
    this->processar = true;
    this->temObservacao = false;
}

Feicao::Feicao(int id, int idConjunto, int tipo, int nPartes, int nVertices, Vertice *vertices, int nSegmentos, Segmento *segmentos)
{
    this->id = id;
    this->idConjunto = idConjunto;
    this->idFeicaoFoz = -1;
    this->tipo = tipo;
    this->nPartes = nPartes;
    this->fluxo = 0;
    this->strahler = 0;
    this->shreve = 0;
    this->nVertices = nVertices;
    this->vertices = vertices;
    this->nSegmentos = nSegmentos;
    this->segmentos = segmentos;
    this->processar = true;
    this->temObservacao = false;
}

void Feicao::limparFeicao()
{

    //Limpando os vértices.
    delete[] this->vertices;
    this->vertices = 0;
    this->nVertices = 0;

    //Limpando os segmentos.
    delete[] this->segmentos;
    this->segmentos = 0;
    this->nSegmentos = 0;
}

int Feicao::getId()
{
   return this->id;
}

int Feicao::getIdConjunto()
{
    return this->idConjunto;
}

int Feicao::getIdFeicaoFoz() const
{
    return idFeicaoFoz;
}

void Feicao::setIdFeicaoFoz(int valor)
{
    idFeicaoFoz = valor;
}

int Feicao::getTipo()
{
    return this->tipo;
}

int Feicao::getNPartes()
{
    return this->nPartes;
}

int Feicao::getFluxo()
{
    return this->fluxo;
}

int Feicao::getStrahler()
{
    return this->strahler;
}

int Feicao::getShreve()
{
    return this->shreve;
}

int Feicao::getNVertices()
{
    return this->nVertices;
}

Vertice* Feicao::getVertice(int indice)
{
    return &this->vertices[indice];
}

void Feicao::setVertice(int indice, Vertice *vertice)
{
    if ((indice >= 0) && (indice < nVertices)) {
        this->vertices[indice] = *vertice;
    }
}

int Feicao::getNSegmentos()
{
    return this->nSegmentos;
}

Segmento* Feicao::getSegmento(int indice)
{
     return &this->segmentos[indice];
}

void Feicao::setSegmento(int indice, Segmento segmento)
{
    if ((indice >= 0) && (indice < nSegmentos)) {
        this->segmentos[indice] = segmento;
    }
}

Segmento* Feicao::getSegmentos()
{
    return this->segmentos;
}

void Feicao::setProcessar(bool valor)
{
    this->processar = valor;
}

bool Feicao::getProcessar()
{
    return this->processar;
}

bool Feicao::getTemObservacao()
{
    return this->temObservacao;
}

void Feicao::setTemObservacao(bool valor)
{
    this->temObservacao = valor;
}

void Feicao::setClassificacao(int &fluxo, int &strahler, int &shreve)
{
 this->fluxo = fluxo;
 this->strahler = strahler;
 this->shreve = shreve;
}
