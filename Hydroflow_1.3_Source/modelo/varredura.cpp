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

#include "varredura.h"
#include "utils/modulo.h"
#include "vertice.h"
#include <vector>
#include <algorithm>


Varredura::Varredura(Geometria *geo)
{
    this->geo = geo;
}

void Varredura::limparVarredura()
{
    //Limpando as linhas.
    VarreduraItem *item;
    for (unsigned int i = 0; i < this->linhas.size(); ++i) {
        item = this->linhas[i];
        item->limparVarreduraItem();
        delete item;
    }
    this->linhas.clear();
    vector<VarreduraItem*> lhs;
    this->linhas.swap(lhs);

    //Limpando os pontos de varredura.
    PontoVarredura *ponto;
    for (unsigned int i = 0; i < this->pontosVarredura.size(); ++i) {
        ponto = this->pontosVarredura[i];
        ponto->limparPontoVarredura();
        delete ponto;
    }
    this->pontosVarredura.clear();
    vector<PontoVarredura*> pv;
    this->pontosVarredura.swap(pv);

    this->geo = 0;
}

void Varredura::setLinhas(Feicao *feicoes, int nFeicoes)
{
    int nSegmentos;

    // Montando a linha de varredura.
    Segmento *segmento = 0;
    VarreduraItem *evento = 0;
    Vertice *ponto = 0;
    for (int j = 0; j < nFeicoes; ++j) {
        if (feicoes[j].getProcessar()) {
            nSegmentos = feicoes[j].getNSegmentos();
            for (int i = 0; i < nSegmentos; ++i) {
                segmento = feicoes[j].getSegmento(i);
                ponto = new Vertice(segmento->getA()->getX(), segmento->getA()->getY());
                evento = new VarreduraItem(ponto, segmento, 0);
                this->linhas.push_back(evento);
                ponto = new Vertice(segmento->getB()->getX(), segmento->getB()->getY());
                evento = new VarreduraItem(ponto, segmento, 1);
                this->linhas.push_back(evento);
            }
        }
    }
}

void Varredura::setLinhas(const vector<Feicao*>& listaFeicoesNovas)
{
    Feicao *feicao = 0;
    int nSegmentos;
    Segmento *segmento;
    VarreduraItem *evento = 0;
    Vertice *ponto = 0;

    for (unsigned int i = 0; i < listaFeicoesNovas.size(); ++i) {
        feicao = listaFeicoesNovas.at(i);
        if (feicao->getProcessar()) {
            nSegmentos = feicao->getNSegmentos();
            for (int j = 0; j < nSegmentos; ++j) {
                segmento = feicao->getSegmento(j);
                ponto = new Vertice(segmento->getA()->getX(), segmento->getA()->getY());
                evento = new VarreduraItem(ponto, segmento, 0);
                this->linhas.push_back(evento);
                ponto = new Vertice(segmento->getB()->getX(), segmento->getB()->getY());
                evento = new VarreduraItem(ponto, segmento, 1);
                this->linhas.push_back(evento);
            }
        }
    }
}

int Varredura::getQuantidadeLinhas()
{
    return this->linhas.size();
}

void Varredura::ordenarLinhas()
{
    sort(this->linhas.begin(), this->linhas.end(), compararItemVarreduraX);
}

// Alterado para pegar ultimo!!
VarreduraItem* Varredura::obterEvento()
{
    VarreduraItem *resposta = 0;
    if (!this->linhas.empty()) {
        resposta =  this->linhas.back();
        this->linhas.pop_back();
    }
    return resposta;
}

// Insere em ordem decrescente para melhorar desempenho na retirado.
void Varredura::inserir(unsigned int inicio, unsigned int fim, VarreduraItem *item)
{
    if (inicio <= fim) {

        // Calculando o meio (indice).
        unsigned int meio = (int) (inicio + fim)/2;

        // Lendo o registro do meio.
        VarreduraItem *varreI = this->linhas[meio];

        //Comparando o item informado com o registro central.
        int comp = compararItensVarredura2(item, varreI);
        if (comp > 0) { // item > varreI: gravar antes
            if (inicio == meio) {
                linhas.insert(linhas.begin() + meio, item);
            } else if (meio > 0) {
                inserir(inicio, (meio - 1), item);
            } else { // É inicio da lista (meio = 0).
                linhas.insert(linhas.begin(), item);
            }
        } else if (comp < 0) { // item < varreI
            if (meio == fim) {
                if (meio == (linhas.size() - 1)) { // É ultimo elemento.
                    linhas.push_back(item);
                } else {
                    linhas.insert(linhas.begin() + meio + 1, item);
                }
            } else { // inicio < Fim.
                inserir((meio + 1), fim, item);
            }
        } // comp = 0; item já existe. Nada fazer!
    }
}

void Varredura::inserir(VarreduraItem *item)
{
    if (this->linhas.empty()) {
      linhas.push_back(item);
    } else {
        this->inserir(0, (this->linhas.size() - 1), item);
    }
}

/*
 * Valores de retorno:
 * -1 - se o primeiro ítem é menor que o segundo.
 *  0 - se o primeiro ítem é igual ao segundo.
 *  1 - se o primeiro ítem é maior que o segundo.
 *
 */
int Varredura::compararItensVarredura2(VarreduraItem *primeiro, VarreduraItem *segundo)
{
/* Tabela verdade dos eventos
 * |tipoX|tipoY|V/F|Observação
 * |  0  |  0  | X |Analisar y e ângulo
 * |  0  |  1  |-1 |
 * |  0  |  2  |-1 |
 * |  1  |  0  | 1 |
 * |  1  |  1  | X |Analisar y e ângulo
 * |  1  |  2  | 1 |
 * |  2  |  0  | 1 |
 * |  2  |  1  |-1 |
 * |  2  |  2  | X |Analisar y e ângulo
 */
    int resposta = 0;
    if (geo->seMenor(primeiro->getX(), segundo->getX())) {
        resposta = -1;
    } else if (geo->seMenor(segundo->getX(), primeiro->getX())) {
        resposta = 1;
    } else if (geo->seIguais(primeiro->getX(), segundo->getX())) {
        if (primeiro->getTipoEvento() == segundo->getTipoEvento()) {

            // Verificando se são os mesmos segmentos.
            if ((primeiro->getSegmentoA()->getId() == segundo->getSegmentoA()->getId()) &&
                (primeiro->getSegmentoA()->getIdFeicao() == segundo->getSegmentoA()->getIdFeicao()) &&
                (primeiro->getSegmentoA()->getIdConjunto() == segundo->getSegmentoA()->getIdConjunto()) &&
                (primeiro->getSegmentoB()->getId() == segundo->getSegmentoB()->getId()) &&
                (primeiro->getSegmentoB()->getIdFeicao() == segundo->getSegmentoB()->getIdFeicao()) &&
                (primeiro->getSegmentoB()->getIdConjunto() == segundo->getSegmentoB()->getIdConjunto())) { //São iguais!

                resposta = 0;
            } else {

                // Verificando as alturas(eixo y).
                if (geo->seMenor(primeiro->getY(), segundo->getY())) {
                    resposta = -1;
                } else if (geo->seMenor(primeiro->getY(), segundo->getY())) {
                    resposta = 1;
                } else if (geo->seIguais(primeiro->getY(), segundo->getY())) {

                    // Verificando o ângulo de inclinação dos segmentos
                    if (geo->compararAngulos(primeiro->getSegmentoA(), segundo->getSegmentoA()) >= 0) {
                        resposta = 1;
                    } else { // yPrimeiro < ySegundo
                        resposta = -1;
                    }
                }
            }
        } else if (((primeiro->getTipoEvento() == 0) && (segundo->getTipoEvento() == 1)) ||
                   ((primeiro->getTipoEvento() == 0) && (segundo->getTipoEvento() == 2)) ||
                   ((primeiro->getTipoEvento() == 2) && (segundo->getTipoEvento() == 1))) {
            resposta = -1;
        } else if (((primeiro->getTipoEvento() == 1) && (segundo->getTipoEvento() == 0)) ||
                   ((primeiro->getTipoEvento() == 1) && (segundo->getTipoEvento() == 2)) ||
                   ((primeiro->getTipoEvento() == 2) && (segundo->getTipoEvento() == 0))) {
            resposta = 1;
        }
    }
    return resposta;
}

/*
 * Valores de retorno:
 * -1 - se o primeiro ítem é menor que o segundo.
 *  0 - se o primeiro ítem é igual ao segundo.
 *  1 - se o primeiro ítem é maior que o segundo.
 *
 * Critério de ordenação:
 * 1º - eixos x;
 * 2º - eixo y;
 */
int Varredura::compararPontosVarredura(Vertice *primeiro, Vertice *segundo)
{
    int resposta = 0;

    if (!geo->seIguais(primeiro, segundo)) {
        if (geo->seIguais(primeiro->getX(), segundo->getX())){
            if (geo->seMenor(primeiro->getY(), segundo->getY())){
                resposta = -1;
            } else {// y do primeiro > y do segundo.
                resposta = 1;
            }
        } else if (geo->seMenor(primeiro->getX(), segundo->getX())) {
            resposta = -1;
        }  else {// x do primeiro > x do segundo.
            resposta = 1;
        }
    }

    return resposta;
}

/*
 * Valores de retorno:
 * -1 - se a linha de varredura é menor que o ponto de varredura.
 *  0 - se a linha de varredura intercepta o ponto de varredura.
 *  1 - se a linha de varredura é maior que o ponto de varredura.
 *
 * Critério de ordenação:
 * 1º - eixos x;
 * 2º - eixo y;
 */
int Varredura::compararLinhasVarredura(double linhaVarredura, Vertice *ponto)
{
    int resposta = 0;

    if (!ponto->naLinhaDeVarredura(linhaVarredura, this->geo->getTolerancia())) {

        // Comparando as ordenadas.
        if (geo->seMenor(linhaVarredura, ponto->getX())) {
            resposta = -1;
        } else {// É maior!
            resposta = 1;
        }
    }

    return resposta;
}

PontoVarredura* Varredura::criarPontoVarredura(Vertice *ponto, Segmento *segmentoA, Segmento *segmentoB)
{
    Vertice *pontoV = new Vertice(ponto->getX(), ponto->getY());
    PontoVarredura *pontoVarredura = new PontoVarredura(pontoV, segmentoA);
    if (segmentoB != 0) {
        pontoVarredura->inserirSegmento(segmentoB);
    }
    return pontoVarredura;
}

void Varredura::inserirPontoVarredura(VarreduraItem *itemVarredura)
{
    if (this->pontosVarredura.empty()) {
        PontoVarredura *pontoVarredura = this->criarPontoVarredura(itemVarredura->getPonto(), itemVarredura->getSegmentoA(), itemVarredura->getSegmentoB());
        this->pontosVarredura.push_back(pontoVarredura);
    } else {
        this->inserirPontoVarredura(0, (this->pontosVarredura.size() - 1), itemVarredura);
    }
}

void Varredura::inserirPontoVarredura(unsigned int inicio, unsigned int fim, VarreduraItem *itemVarredura)
{
    if (inicio <= fim) {
        PontoVarredura *pontoVarredura = 0;

        //Calculando o meio (indice).
        unsigned int meio = (unsigned int) (inicio + fim)/2;

        //Lendo o registro do meio.
        PontoVarredura *item = this->pontosVarredura.at(meio);

        //Comparando o item informado com o registro do meio.
        int comp = this->compararPontosVarredura(itemVarredura->getPonto(), item->getPonto());

        if (comp == 0) { //São iguais.

            // Inserindo os segmentos no ponto de varredura existente.
            item->inserirSegmento(itemVarredura->getSegmentoA());
            if (itemVarredura->getSegmentoB() != 0) { //Interseção.
                item->inserirSegmento(itemVarredura->getSegmentoB());
            }
        } else if (comp < 0) {//Ponto < Ponto do meio (incluir antes).
            if ((meio == 0) || (inicio == fim)) {
                pontoVarredura = this->criarPontoVarredura(itemVarredura->getPonto(), itemVarredura->getSegmentoA(), itemVarredura->getSegmentoB());
                this->pontosVarredura.insert((this->pontosVarredura.begin() + meio), pontoVarredura);
            } else {
                this->inserirPontoVarredura(0, (meio - 1), itemVarredura);
            }
        } else if (comp > 0) { //Ponto > Ponto do meio (incluir depois).
            if (meio == (this->pontosVarredura.size() - 1)) { //Ultimo registro.
                pontoVarredura = this->criarPontoVarredura(itemVarredura->getPonto(), itemVarredura->getSegmentoA(), itemVarredura->getSegmentoB());
                this->pontosVarredura.push_back(pontoVarredura);
            } else if (inicio == fim){
                pontoVarredura = this->criarPontoVarredura(itemVarredura->getPonto(), itemVarredura->getSegmentoA(), itemVarredura->getSegmentoB());
                this->pontosVarredura.insert((this->pontosVarredura.begin() + meio + 1), pontoVarredura);
            } else {
                this->inserirPontoVarredura((meio + 1), fim, itemVarredura);
            }
        }
    }
}

PontoVarredura* Varredura::procurarPontoVarredura(unsigned int inicio, unsigned int fim, double linhaVarredura)
{
    PontoVarredura *item = 0;
    PontoVarredura *pontoVarredura = 0;
    if (inicio <= fim) {

        // Calculando o meio (indice).
        unsigned int meio = (unsigned int) (inicio + fim)/2;

        // Lendo o registro do meio.
        item = this->pontosVarredura.at(meio);

        // Analisando a linha de varredura do registro do meio.
        int comp = this->compararLinhasVarredura(linhaVarredura, item->getPonto());

        if (comp == 0) {// Encontrou!
            pontoVarredura = item;
            this->pontosVarredura.erase(this->pontosVarredura.begin() + meio);
        } else if (comp < 0) {// Linha de varredura é mmenor que a ordenada do ponto do meio.
            if (meio > 0) {
                pontoVarredura = this->procurarPontoVarredura(0, (meio - 1), linhaVarredura);
            }
        } else {// Linha de varredura é maior que a ordenada do ponto do meio.
            if (meio < (this->pontosVarredura.size() - 1)) {
                pontoVarredura = this->procurarPontoVarredura((meio + 1), (this->pontosVarredura.size() - 1), linhaVarredura);
            }
        }
    }

    return pontoVarredura;
}

PontoVarredura* Varredura::getPontoVarredura(double linhaVarredura)
{
    PontoVarredura *pontoVarredura = 0;
    if (!this->pontosVarredura.empty()) {

        //Implementação por busca binária.
        pontoVarredura = this->procurarPontoVarredura(0, (this->pontosVarredura.size() - 1), linhaVarredura);
    }
    return pontoVarredura;
}
