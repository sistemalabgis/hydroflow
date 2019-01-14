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

#include "modulo.h"
#include "modelo/posicao.h"

Geometria *geometria;

void moduloSetGeometria(Geometria *geo)
{
    geometria = geo;
}

// Retorna true se primeiro > segundo.
bool compararItemVarreduraX(VarreduraItem *primeiro, VarreduraItem *segundo)
{
    bool resposta = false; // Padrão: primeiro é menor que o segundo!
    double iPrimeiro = primeiro->getX();
    double iSegundo = segundo->getX();

    //Avaliando o indice do item de varredura.
    if (geometria->seMenor(iSegundo, iPrimeiro)) { // Índice menor primeiro
        resposta = true;
    } else if (geometria->seIguais(iPrimeiro, iSegundo)) {

        //Avaliando o tipo dos eventos
        int tipoPrimeiro = primeiro->getTipoEvento();
        int tipoSegundo = segundo->getTipoEvento();

        if ((tipoPrimeiro == 0) && (tipoSegundo == 0)) { // São do mesmo tipo: entrada!

            //Avaliando altura na linha de varredura (y).
            if (geometria->seMenor(segundo->getY(), primeiro->getY())) {
                resposta = true;
            }
        } else if ((tipoPrimeiro == 1) && (tipoSegundo == 1)) { // São do mesmo tipo: saída!

            double xPrimeiro = primeiro->getSegmentoA()->getMenorX(geometria->getTolerancia());
            double xSegundo = segundo->getSegmentoA()->getMenorX(geometria->getTolerancia());
            if (geometria->seMenor(xSegundo, xPrimeiro)) {
                resposta = true; // Entra depois, sai depois!
            }

        // São de tipos diferentes!
        } else if (((tipoPrimeiro == 1) && (tipoSegundo == 0)) ||
                   ((tipoPrimeiro == 2) && (tipoSegundo == 1)) ||
                   ((tipoPrimeiro == 0) && (tipoSegundo == 2))) {
            resposta = true;
        }
    }
    return resposta;
}

bool compararItemIndice(IndiceItem *primeiro, IndiceItem *segundo)
{
    bool resposta = false;
    if(primeiro->getFeicao() == segundo->getFeicao()) {
        if(primeiro->getValor() < segundo->getValor()) {
            resposta = true;
        }
    } else if (primeiro->getFeicao() < segundo->getFeicao()) {
        resposta = true;
    }
    return resposta;
}
