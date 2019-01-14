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

#include "no.h"


No::No(int idFeicao)
{
    this->idFeicao = idFeicao;
    this->fluxo = 0;
    this->strahler = 0;
    this->shreve = 0;
}

No::~No()
{
    //Destruindo os filhosNo.
    int nFilhos = getQuantidadeFilhosNo();
    if (nFilhos > 0) {
        No *filho;
        for (int i = 0; i < nFilhos; ++i) {
            filho = filhosNo.at(i);
            delete filho;
            filho = 0;
        }
        vector<No*> tmp;
        filhosNo.clear();
        filhosNo.swap(tmp);
    }
    this->idFeicao = 0;
    this->fluxo = 0;
    this->strahler = 0;
    this->shreve = 0;
    this->valoresStrahler.clear();
}

int No::getIdFeicao()
{
    return this->idFeicao;
}

void No::setIdElemento(const int &idFeicao)
{
    this->idFeicao = idFeicao;
}

int No::getFluxo()
{
    return this->fluxo;
}

void No::setFluxo(const int &fluxo)
{
    this->fluxo = fluxo;
}

int No::calcularStrahler()
{
    int a = 0;
    int b = 0;
    int reg;
    int valor = 0;

    //Selecionando os dois maiores valores.
    for (unsigned int i = 0; i < valoresStrahler.size(); ++i) {
        reg = valoresStrahler.at(i);
        if (reg > a) {
            if (a > b) {
                b = a;
            }
            a = reg;
        } else if (reg > b) {
            b = reg;
        }
    }

    //Calculando Strahler.
    if (valoresStrahler.size() > 0) {
        if (a == b) {
            valor = a + 1;
        } else if (a > b) {
            valor = a;
        } else {
            valor = b;
        }
    }
    return valor;
}

int No::getStrahler()
{
    int valor;
    if (getQuantidadeFilhosNo() > 0) {
        valor = calcularStrahler();
    } else { //getQuantidadeFilhosNo() = 0;
        valor = strahler;
    }
    return valor;
}

void No::setStrahler(const int &valor)
{
    strahler = valor;
}

int No::getShreve()
{
    return this->shreve;
}

void No::setShreve(const int &valor)
{
    shreve = valor;
}

void No::inserirFilhoNo(No *no)
{
    if (no != 0) {
        filhosNo.push_back(no);

        //Incluindo valor de Strahler.
        valoresStrahler.push_back(no->getStrahler());

        //Calculando o valor de Shreve.
        shreve = shreve + no->getShreve();
    }
}

int No::getQuantidadeFilhosNo()
{
    return (int) filhosNo.size();
}
