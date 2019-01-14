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

#include "segmento.h"
#include <cmath>

Segmento::Segmento()
{
    this->id = -1;
    this->idFeicao = -1;
    this->idConjunto = -1;
    this->a = 0;
    this->b = 0;
    this->eFoz = false;
}

Segmento::Segmento(int id, int idFeicao, int idConjunto, Vertice *a, Vertice *b)
{
    this->id = id;
    this->idFeicao = idFeicao;
    this->idConjunto = idConjunto;
    this->a = a;
    this->b = b;
    this->eFoz = false;
}

int Segmento::getId()
{
    return this->id;
}

void Segmento::setId(int valor)
{
    this->id = valor;
}

int Segmento::getIdFeicao()
{
    return this->idFeicao;
}

int Segmento::getIdConjunto()
{
    return this->idConjunto;
}

Vertice* Segmento::getA()
{
    return this->a;
}

void Segmento::setA(Vertice *valor)
{
    this->a = valor;
}

Vertice* Segmento::getB()
{
    return this->b;
}

void Segmento::setB(Vertice *valor)
{
    this->b = valor;
}

/* Retorna o valor de "x" do vértice com o menor valor.
 */
double Segmento::getMenorX(double tolerancia)
{
    if ((this->getA()->getX() + tolerancia) < getB()->getX()) {
        return this->getA()->getX();
    } else {
        return this->getB()->getX();
    }
}

/* Retorna o valor de "x" do vértice com maior valor.
 */
double Segmento::getMaiorX(double tolerancia)
{
    if ((this->getA()->getX() - tolerancia) > this->getB()->getX()) {
        return this->getA()->getX();
    } else {
        return this->getB()->getX();
    }
}

/* Retorna o valor de 'x' do vértice com o valor de 'y' informado.
 * Se o segmento é horizontal, retorna o menor valor de 'x'.
 */
double Segmento::getX(double y, double tolerancia)
{
    double resposta = 0;

    //Verificando se segmento é horizontal.
    if (this->eHorizontal(tolerancia)) {
        if ((this->getA()->getX() + tolerancia) < this->getB()->getX()) {
            resposta = this->getA()->getX();
        } else {
            resposta = this->getB()->getX();
        }
    } else if (fabs(y - this->getA()->getY()) <= tolerancia) {
        resposta = this->getA()->getX();
    } else if (fabs(y - this->getB()->getY()) <= tolerancia) {
        resposta = this->getB()->getX();
    }
    return resposta;
}

/* Retorna o valor de 'y' do vértice com o valor de 'x' informado.
 * Se o segmento é vertical, retorna o menor valor de 'y'.
 */
double Segmento::getY(double x, double tolerancia)
{
    double resposta = 0;

    //Verificando se segmento é vertical.
    if (eVertical(tolerancia)) {
        if ((this->getA()->getY() + tolerancia) < this->getB()->getY()) {
            resposta = this->getA()->getY();
        } else {
            resposta = this->getB()->getY();
        }
    } else if (fabs(x - this->getA()->getX()) <= tolerancia) {
        resposta = this->getA()->getY();
    } else if (fabs(x - this->getB()->getX()) <= tolerancia) {
        resposta = this->getB()->getY();
    }
    return resposta;
}

/* Retorna o valor de "y" do vértice com o menor valor.
 */
double Segmento::getMenorY(double tolerancia)
{
    if ((this->getA()->getY() + tolerancia) < this->getB()->getY()) {
        return this->getA()->getY();
    } else {
        return this->getB()->getY();
    }
}

/* Retorna o valor de "y" do vértice com meior valor.
 */
double Segmento::getMaiorY(double tolerancia)
{
    if ((this->getB()->getY() - tolerancia) > this->getA()->getY()) {
        return this->getB()->getY();
    } else {
        return this->getA()->getY();
    }
}

Vertice* Segmento::getVertice(double eixo, char tipoEixo, double tolerancia)
{
    Vertice *ponto = 0;
    if (tipoEixo == 'x') {
        if (fabs(this->getA()->getX() - eixo) <= tolerancia) {
            ponto = this->getA();
        }else if (fabs(this->getB()->getX() - eixo) <= tolerancia) {
            ponto = this->getB();
        }
    } else { // É eixo 'y'.
        if (fabs(this->getA()->getY() - eixo) <= tolerancia) {
            ponto = this->getA();
        }else if (fabs(this->getB()->getY() - eixo) <= tolerancia) {
            ponto = this->getB();
        }
    }
    return ponto;
}

double Segmento::getDx()
{
    return this->b->getX() - this->a->getX();
}

double Segmento::getDy()
{
    return this->b->getY() - this->a->getY();
}

/*
 * Retornas verdadeiro se o segmento é degenerado.
 */
bool Segmento::ePonto(double tolerancia)
{
    if ((fabs(this->a->getX() - this->b->getX()) <= tolerancia) && (fabs(this->a->getY() - this->b->getY()) <= tolerancia)) {
        return true;
    }
    return false;
}

bool Segmento::eVertical(double tolerancia)
{
    bool resposta = false;
    if (!this->ePonto(tolerancia)) {
        if (fabs(this->a->getX() - this->b->getX()) <= tolerancia) {
            resposta =  true;
        }
    }
    return resposta;
}

bool Segmento::eHorizontal(double tolerancia)
{
    bool resposta = false;
    if (!this->ePonto(tolerancia)) {
        if (fabs(this->a->getY() - this->b->getY()) <= tolerancia) {
            resposta = true;
        }
    }
    return resposta;
}

/* Compara dois segmentos
 * Valores de retorno:
 * -1 - se este objeto é menor que o informado.
 *  0 - se este objeto é igual ao informado.
 *  1 - se este objeto é maior que o informado.
 *
 * Critério de ordenação:
 * 1º - idConjunto;
 * 2º - idFeicao;
 * 3° - id.
 */
int Segmento::compararSegmentos(Segmento *segmento)
{
    int resposta = 0;
    if (this->getIdConjunto() < segmento->getIdConjunto()) {
        resposta = -1;
    } else if (this->getIdConjunto() > segmento->getIdConjunto()) {
        resposta = 1;
    } else { //this->getIdConjunto() = segmento->getIdConjunto()
        if (this->getIdFeicao() < segmento->getIdFeicao()) {
            resposta = -1;
        } else if (this->getIdFeicao() > segmento->getIdFeicao()) {
            resposta = 1;
        } else { //this->getIdFeicao() = segmento->getIdFeicao()
            if (this->getId() < segmento->getId()) {
                resposta = -1;
            } else if  (this->getId() > segmento->getId()) {
                resposta = 1;
            } //this->getId() > segmento->getId()
        }
    }
    return resposta;
}

bool Segmento::getEFoz() const
{
    return this->eFoz;
}

void Segmento::setEFoz()
{
    this->eFoz = true;
}
