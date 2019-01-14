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

#ifndef VETOR_H
#define VETOR_H

#include "vertice.h"

class Segmento
{
private:
    int id;
    int idFeicao;
    int idConjunto;
    Vertice *a;
    Vertice *b;
    bool eFoz;

public:
    Segmento();
    Segmento(int id, int idFeicao, int idConjunto, Vertice *a, Vertice *b);
    int getId();
    void setId(int valor);
    int getIdFeicao();
    int getIdConjunto();
    Vertice* getA();
    void setA(Vertice *valor);
    Vertice* getB();
    void setB(Vertice *valor);
    double getMenorX(double tolerancia);
    double getMaiorX(double tolerancia);
    double getX(double y, double tolerancia);
    double getY(double x, double tolerancia);
    double getMenorY(double tolerancia);
    double getMaiorY(double tolerancia);
    double getDx();
    double getDy();
    bool ePonto(double tolerancia);
    bool eVertical(double tolerancia);
    bool eHorizontal(double tolerancia);
    Vertice* getVertice(double eixo, char tipoEixo, double tolerancia);
    int compararSegmentos(Segmento *segmento);
    void setOrdem(bool ordem);
    bool getOrdem();
    bool getEFoz() const;
    void setEFoz();
};

#endif // VETOR_H
