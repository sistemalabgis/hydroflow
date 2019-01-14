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

#ifndef NO_H
#define NO_H

#include <vector>

using namespace std;

class No
{
private:
    int idFeicao;
    int fluxo;
    int strahler; //Valor da classificação Strahler.
    int shreve;  //Valor da classificação Shreve.
    vector<No*> filhosNo;
    vector<int> valoresStrahler;
    int calcularStrahler();
public:
    No(int idFeicao);
    ~No();
    int getIdFeicao();
    void setIdElemento(const int &idFeicao);
    int getFluxo();
    void setFluxo(const int &fluxo);
    int getStrahler();
    void setStrahler(const int &valor);
    int getShreve();
    void setShreve(const int &valor);
    void inserirFilhoNo(No *no);
    int getQuantidadeFilhosNo();

};

#endif // NO_H
