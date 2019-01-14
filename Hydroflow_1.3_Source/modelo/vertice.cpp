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

#include "vertice.h"
#include <math.h>

Vertice::Vertice()
{
    this->id = -1;
    this->x = 0;
    this->y = 0;
    this->eUltimo = false;
}

Vertice::Vertice(double x, double y)
{
    this->id = -1;
    this->x = x;
    this->y = y;
    this->eUltimo = false;
}

Vertice::Vertice(int id, double x, double y)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->eUltimo = false;
}

Vertice::Vertice(int id, double x, double y, bool eUltimo)
{
    this->id = id;
    this->x = x;
    this->y = y;
    this->eUltimo = eUltimo;
}

int Vertice::getId()
{
    return this->id;
}

double Vertice::getX()
{
    return (double) this->x;
}

double Vertice::getY()
{
    return (double) this->y;
}

void Vertice::setX(double x)
{
    this->x = x;
}

void Vertice::setY(double y)
{
    this->y = y;
}

bool Vertice::eIgual(Vertice *p)
{
    return ((this->getX() == p->getX()) && (this->getY() == p->getY()));
}

bool Vertice::naLinhaDeVarredura(double x, double tolerancia)
{
    return (fabs(x - this->getX()) <= tolerancia);
}

bool Vertice::eExtremo()
{
    return ((this->id == 0) || this->eUltimo);
}
