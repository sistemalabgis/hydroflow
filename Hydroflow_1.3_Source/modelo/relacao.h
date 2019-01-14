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

#ifndef RELACAO_H
#define RELACAO_H

#include <vector>
#include "relacaoitem.h"
#include "segmento.h"
#include "indiceitem.h"
#include "utils/mensagem.h"

using namespace std;

class Relacao
{
private:
    vector<RelacaoItem> lista;
    vector<RelacaoItem> erros;
    Segmento *limite;
    vector<Segmento*> foz; //Segmento das feições da bacia que encostam no limite da bacia.
    vector<RelacaoItem> fozes;

    vector<IndiceItem*> indicePrimario;
    vector<IndiceItem*> indice;

    Mensagem *saida;

    int getIndicePrimario(int inicio, int fim, int idFeicao); // Busca binária no índice primário.
    void inserir(unsigned int inicio, unsigned int fim, Segmento *origem, Segmento *destino, int tipo);
    void inserirFoz(Segmento *bacia, Segmento *limite); //Versão 1.3!
    int comparaPosicao_2(RelacaoItem *primeiro, RelacaoItem *segundo);

public:
    Relacao(Mensagem *saida);
    void limparRelacao();

    // Relações
    void inserirRelacao(Segmento *origem, Segmento *destino, int tipo);
    RelacaoItem* getEvento(int indice);
    vector<Segmento*> getFilhosNo(int idFeicao, int idFeicaoPai, vector<Segmento*> filhosPai, Segmento *&novoPai);
    int getQuantidadeRelacoes();
    bool haErros();
    void listarEventosInesperados(Mensagem *saida);
    Segmento* getLimite();

    // Foz
    RelacaoItem getFoz(int indice);
    int getQuantidadeFozes(); //Versão 1.3!

    // Índices
    void montarIndices();
};

#endif // RELACAO_H
