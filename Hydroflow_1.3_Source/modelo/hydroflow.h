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

#ifndef HYDROFLOW_H
#define HYDROFLOW_H

#include <vector>
#include "modelo/conjuntofeicao.h"
#include "modelo/segmento.h"
#include "utils/geometria.h"
#include "modelo/parametros.h"
#include "modelo/varredura.h"
#include "modelo/relacao.h"
#include "modelo/posicao.h"
#include "modelo/no.h"
#include "utils/mensagem.h"

using namespace std;

class Hydroflow
{
private:
    QObject *origem;
    ConjuntoFeicao *drenagem; // Rede de drenagem.
    ConjuntoFeicao *limite; // Limite da rede de drenagem.
    Geometria *geo; // Classe com funções geométricas.
    Parametros *params;
    Varredura *varredura; // Lista de varredura (abscissas)
    Relacao *relacoesTopologicas; // Lista de relações topológicas identificados no processo de varredura do plano.
    Posicao *posicao; // Posição dos segmentos na linha de varredura (ordenadas)
    No *raizArvore; // Nó que representa a foz da bacia hidrográgica.
    Mensagem *saida;

    void montarLinhaVarredura();
    void avaliarSegmentos(Vertice *ponto, Segmento *acima, Segmento *abaixo);
    void varrerPlano();
    No* criarNos(Segmento *segmento, Segmento *pai, const vector<Segmento*> &irmaos, int &resultado);
    int montarArvore();
    void processarPontosVarredura(double linhaVarredura);
    int avaliarProcessamento();
public:
    Hydroflow(ConjuntoFeicao *drenagem, ConjuntoFeicao *limite, Parametros *parametros, Mensagem *saida);
    void limparHydroflow();
    int classificarBacia();
};

#endif // HYDROFLOW_H
