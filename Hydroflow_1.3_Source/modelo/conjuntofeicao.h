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

#ifndef CONJUNTOFEICAO_H
#define CONJUNTOFEICAO_H

#include "modelo/feicao.h"
#include <vector>
#include "atributofeicaonova.h"
#include "modelo/observacao.h"
#include "atributo.h"
#include <QString>

using namespace std;

class ConjuntoFeicao
{
private:
    QString nomeArquivo;
    int idConjunto; // 0: bacia; 1: limite.
    int codigoTipo; // De acordo com a especificação Shapefile.
    int quantidadeAtributos; // Quantidade de atributos originais do conjunto de feições.
    int quantidadeFeicoes;
    int quantidadeFeicoesNovas;
    Feicao *feicoes; // Lista de registros do shapefile.
    vector <Feicao*> feicoesNovas;
    vector <AtributoFeicaoNova*> atributosFeicoesNovas;
    Observacao *obs;

    int localizarAtributos(int inicio, int fim, int idFeicao);
public:
    ConjuntoFeicao();
    ConjuntoFeicao(int idFeicao, QString nomeArquivo, int codigoTipo);
    void LimparConjuntoFeicao();
    QString getNomeArquivo();
    int getIdConjunto();
    Feicao* getFeicoes();
    void setFeicoes(Feicao *feicoes, int nfeicoes);
    Feicao* getFeicao(int idFeicao);
    int getCodigoTipo();
    int getQuantidadeFeicoes();
    bool getCriarObservacao();
    void setClassificacaoFeicao(int idFeicao, int fluxo, int strahler, int shreve);
    Observacao* getObservacoes();
    void setObservacoes(Observacao *obs);
    void setObservacao(int idFeicao, QString texto);
    int getQuantidadeFeicoesNovas();
    Feicao* getFeicaoNova(int indice);
    void setFeicaoNova(Feicao *feicaoNova);
    const vector<Feicao*>& getFeicoesNovas();
    void setAtributosFeicaoNova(AtributoFeicaoNova *atributosFeicaoNova);
    Atributo* getAtributosFeicaoNova(int idFeicao);
    int getQuantidadeAtributos();
    void setQuantidadeAtributos(int valor);
    int getTotalFeicoes();
};

#endif // CONJUNTOFEICAO_H
