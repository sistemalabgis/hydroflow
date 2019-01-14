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

#include "geometria.h"
#include <cmath>


Geometria::Geometria()
{
    this->tolerancia = 0;
}

Geometria::Geometria(double tolerancia)
{
    this->tolerancia = tolerancia;
}

void Geometria::setTolerancia(double tolerancia)
{
    this->tolerancia = tolerancia;
}

double Geometria::getTolerancia()
{
    return this->tolerancia;
}

/*
 * Compara os valores de a e b, retornando o menor.
 */
double Geometria::getMenor(double a, double b)
{
    if(this->seMenor(a, b)) {
        return a;
    } else {
        return b;
    }
}

/*
 * Compara os valores de a e b, retornando o maior.
 */
double Geometria::getMaior(double a, double b)
{
    if(this->seMaior(a, b)) {
        return a;
    } else {
        return b;
    }
}

/*
 * Aplicação da equação reduzida da circunferência.
 * Considerções:
 *      Vertice "a" é o centro da circunferência;
 *      Vertice "b" é o elemento em análise.
 * Resposta:
 *      Verdadeiro se o vertice "b" está contido na circunferência com centro no vertice "a" e raio igual a tolerância.
 */
bool Geometria::seIguais(Vertice *a, Vertice *b)
{
    return (sqrt(pow((b->getX() - a->getX()), 2) + pow((b->getY() - a->getY()), 2)) <= getTolerancia());
}

bool Geometria::seIguais(double a, double b)
{
    return (fabs(a - b) <= this->tolerancia);
}

bool Geometria:: seMenor(double a, double b)
{
    return ((a + this->tolerancia) < b);
}

bool Geometria:: seMaior(double a, double b)
{
    return ((a - this->tolerancia) > b);
}

/*
 * Retorna valor booleano, indicando se existe ou não interseção entre
 * os segmentos. Caso exista, o ponto de interseção e passado em "I".
 */
bool Geometria::pontoIntersecao(Segmento *primeiro, Segmento *segundo, Vertice *&I)
{
    delete I;
    I = 0;
    bool resposta = false;
    Vertice *a = primeiro->getA();
    Vertice *b = primeiro->getB();
    Vertice *c = segundo->getA();
    Vertice *d = segundo->getB();

    // Calculando s e t;
    double denon = (d->getX() - c->getX()) * (b->getY() - a->getY()) - (d->getY() - c->getY()) * (b->getX() - a->getX());
    if (fabs(denon) > this->tolerancia) {
        double s = ((d->getX() - c->getX()) * (c->getY() - a->getY()) - (d->getY() - c->getY()) * (c->getX() - a->getX())) / denon;
        double t = ((b->getX() - a->getX()) * (c->getY() - a->getY()) - (b->getY() - a->getY()) * (c->getX() - a->getX())) / denon;

        // Verificando se há interseção.
        if ((s > -this->tolerancia && s < (1 + this->tolerancia)) && (t > -this->tolerancia && t < (1 + this->tolerancia))) {
            double x = a->getX() + (s * (b->getX() - a->getX()));
            double y = a->getY() + (s * (b->getY() - a->getY()));
            I = new Vertice(x, y);
            resposta = true;
        }
    }
    return resposta;
}

/*
 *
 */
Vertice* Geometria::pontoRelativo(double x, Segmento *segmento)
{
    Vertice *p = 0;
    Vertice *a = segmento->getA();
    Vertice *b = segmento->getB();

    if (segmento->eVertical(this->tolerancia) || (a->naLinhaDeVarredura(x, this->tolerancia))) { // o segmento é vertical. Retorna o vértice com o menor "y"!
        p = new Vertice(a->getX(), a->getY());
    } else { // o segmento não é vertical.
        if (segmento->getB()->naLinhaDeVarredura(x, this->tolerancia)) { // A linha de varredura "x" intercepta o vértice "b".
            p = new Vertice(segmento->getB()->getX(), segmento->getB()->getY());
        } else {
            /* Calculando o ponto relativo.
             * Para a->getX() < b->getX()
             * double dx = b->getX() - a->getX();
             * double dy = b->getY() - a->getY();
             * double  t = (x - a->getX()) / dx;
             * y = (t * dy) + p.getX();
             * ou
             * y = (((x - a->getX()) / (b->getX() - a->getX())) * (b->getY() - a->getY())) + a->getY();
             */

            double y;
            y = (((x - a->getX()) / (b->getX() - a->getX())) * (b->getY() - a->getY())) + a->getY();
            p = new Vertice(x, y);
        }
    }
    return p;

}

int Geometria::compararValores(double a, double b)
{
    if (this->seMenor(a, b)) {
        return -1;
    } else if (this->seMaior(a, b)) {
        return 1;
    } else {
        return 0;
    }
}

/* Compara o ângulo inclinação dos segmentos.
 * Resposta:
 *          -1 o ângulo do primeiro segmento é menor que o ângulo do segundo segmento;
 *           0 o ângulo do primeiro segmento é igual ao ângulo do segundo segmento;
 *           1 o ângulo do primeiro segmento é maior que o ângulo do segundo segmento;
 */
int Geometria::compararAngulos(Segmento *primeiro, Segmento *segundo)
{
    double comp = 0;
    Vertice *a; //Ponto de análise do primeiro segmento.
    Vertice *b; //Ponto de análise do segundo segmento.

    a = this->subtrair(primeiro->getB(), primeiro->getA());
    b = this->subtrair(segundo->getB(), segundo->getA());

    // Comparando.
    comp = (a->getX() * b->getY()) - (b->getX() * a->getY());

    if (comp < 0) {
        return 1;
    } else if (comp > 0) {
        return -1;
    } else  { // Estão alinhados.
        return 0;
    }
    delete a;
    delete b;

    return comp;
}

/*
 * Subtrai das coordenadas de "a" as coordenadas de "b".
 */
Vertice* Geometria::subtrair(Vertice *a, Vertice *b)
{
    Vertice *p = new Vertice((a->getX() - b->getX()), (a->getY() - b->getY()));
    return p;
}
