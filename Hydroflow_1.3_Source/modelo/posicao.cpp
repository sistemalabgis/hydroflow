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

#include "posicao.h"

Posicao::Posicao(Geometria *geo, Mensagem *saida)
{
    this->geo = geo;
    this->saida = saida;
}

void Posicao::limparPosicao()
{
    this->lista.clear();
    vector<Segmento*> seg;
    this->lista.swap(seg);
    this->geo = 0;
    this->saida = 0;
}

int Posicao::buscaBinaria(unsigned int inicio, unsigned int fim, double linhaVarredura, Segmento *segmento)
{
    int resposta = -1; //Não encontrou.

    if ((fim < lista.size()) && (inicio <= fim)) {
        unsigned int meio = (int) (inicio + fim)/2;
        Segmento *segItem = this->lista.at(meio);

        // Comparando segmento com item central.
        if (segmento->compararSegmentos(segItem) == 0) {
            resposta = meio;
        } else {

            // Avaliando as posições.
            int comparacao = this->compararPosicao(linhaVarredura, segmento, segItem);

            if (comparacao == -1) { // segmento está abaixo.
				resposta = this->buscaBinaria(meio + 1, fim, linhaVarredura, segmento);
            } else { // comparacao = 1; segmento está acima.
                resposta = this->buscaBinaria(inicio, meio - 1, linhaVarredura, segmento);
            }
        }
    }
    return resposta;
}

// Retorna -1 se não encontrou.
int Posicao::localizar(double linhaVarredura, Segmento *segmento)
{
    int indice = this->buscaBinaria(0, (this->lista.size() - 1), linhaVarredura, segmento);
    return indice;
}

/* Compara os segmentos avaliando os pontos relativos do primeiro e segundo segmentos
 * com a linha de varredura.
 *
 * Resultado:
 *  0: os segmentos tem a mesma altura relativa;
 * -1: o primeiro está abaixo do segundo e
 *  1: o primeiro está acima do segundo.
 */
int Posicao::compararPosicao(double linhaVarredura, Segmento *primeiro, Segmento *segundo)
{
    int resultado = 0;

    //Calculando os pontos relatívos.
    Vertice *pPrimeiro = geo->pontoRelativo(linhaVarredura, primeiro);
    Vertice *pSegundo = geo->pontoRelativo(linhaVarredura, segundo);

    if (geo->seIguais(pPrimeiro, pSegundo)) { //Mesma altura relativa.

        //Avaliando os vértices.
        if (primeiro->ePonto(geo->getTolerancia()) || segundo->ePonto(geo->getTolerancia())) {
            resultado = primeiro->compararSegmentos(segundo);
        } else if ((geo->seIguais(pPrimeiro, primeiro->getA()) && geo->seIguais(primeiro->getA(), segundo->getB())) || //Primeiro entrando.
            (geo->seIguais(pSegundo, segundo->getA()) && geo->seIguais(primeiro->getB(), segundo->getA()))) { //Primeiro saindo.

            //Tratando os casos 1 e 2.
            if (geo->seIguais(primeiro->getA()->getY(), segundo->getA()->getY())) {
                resultado = geo->compararAngulos(primeiro, segundo);
                if (resultado == 0) {

                    //Tratando o caso 1 (segmentos horizontais). Arbitrei!
                    resultado = geo->compararValores(primeiro->getA()->getX(), segundo->getA()->getX());
                } //É o caso 2!

            //Tratando o caso 3.
            } else {
                resultado = geo->compararValores(primeiro->getA()->getY(), segundo->getA()->getY());
            }

        // Testando casos 6 e 7.
        } else if ((geo->seIguais(pPrimeiro, primeiro->getB()) && geo->seIguais(primeiro->getB(), segundo->getB())) || //Caso 6.
                   (geo->seIguais(pPrimeiro, primeiro->getB()) && !geo->seIguais(pSegundo, segundo->getA()) &&
                    !geo->seIguais(pSegundo, segundo->getB()))) {

            // Determinando a linha de varredura.
            if (geo->seMaior(primeiro->getA()->getX(), segundo->getA()->getX())) {
                linhaVarredura = primeiro->getA()->getX();
            } else {
                linhaVarredura = segundo->getA()->getX();
            }

            // Comparando os segmentos.
            resultado = this->compararPosicao(linhaVarredura, primeiro, segundo);

        // Tratando o caso 8.
        } else if (!geo->seIguais(pPrimeiro, primeiro->getA()) && !geo->seIguais(pPrimeiro, primeiro->getB()) &&
                   !geo->seIguais(pSegundo, segundo->getA()) && !geo->seIguais(pSegundo, segundo->getB())) {

            // Comparando invertido por ser interseção.
            resultado = geo->compararAngulos(segundo, primeiro);

        //Tratando os casos 4 e 5.
        } else {
            resultado = geo->compararAngulos(primeiro, segundo);
        }
    } else  { //Alturas relativas diferentes.
        resultado = geo->compararValores(pPrimeiro->getY(), pSegundo->getY());
    }

    //Se mesmo ângulo, desempatar!!!
/*
    if (resultado == 0) {
         saida->append("F" + QString::number(primeiro->getIdFeicao()) + ":" + QString::number(primeiro->getId()) + "-" + QString::number(primeiro->getIdConjunto())
                       + " = " + QString::number(segundo->getIdFeicao()) + ":" + QString::number(segundo->getId()) + "-" + QString::number(segundo->getIdConjunto()));
    }
*/
    delete pPrimeiro;
    delete pSegundo;

    return resultado;
}

int Posicao::inserir(Segmento *segmento)
{
    int indice = -1;
    if (this->lista.empty()) {
        this->lista.push_back(segmento);
        indice = 0;
    } else {
        bool sair = false;
        unsigned int inicio = 0;
        unsigned int meio;
        unsigned int fim = this->lista.size() - 1;
        Segmento *segMeio;
        double linhaVarredura = segmento->getA()->getX();;
        int comparacao;
        vector<Segmento*>::iterator it;

        while (!sair) {

            // Calculando o meio (indice).
            meio = (int) (inicio + fim)/2;

            // Lendo o registro do meio.
            segMeio = this->lista.at(meio);

            // Comparando o registro com o segmento do resistro do meio.
            comparacao = this->compararPosicao(linhaVarredura, segmento, segMeio);

            // registro < segMeio. Iserir após o meio.
            if (comparacao < 0) {
                if (meio == fim) {
                    if (meio == (this->lista.size() - 1)) {
                        this->lista.push_back(segmento);
                        indice = this->lista.size() - 1;
                        sair = true;
                    } else {
                        it = this->lista.begin();
                        advance(it, meio + 1);
                        this->lista.insert(it, segmento);
                        indice = meio + 1;
                        sair = true;
                    }
                } else {
                    inicio = meio + 1;
                }

                // registro > segMeio. Iserir antes o meio.
            } else if (comparacao > 0) {
                if (inicio == meio) {
                    it = this->lista.begin();
                    advance(it, meio);
                    lista.insert(it, segmento);
                    indice = meio;
                    sair = true;
                } else {
                    fim = meio - 1;
                }

            } else { //Já está incluido.
                indice = meio;
                sair = true;
            }
        }
    }
    return indice;
}

void Posicao::excluir(unsigned int indice)
{
    if ((!lista.empty()) && (indice < this->lista.size())) {
        lista.erase(lista.begin() + indice);
    }
}

Segmento* Posicao::acima(unsigned int indice)
{
    Segmento *resposta = 0;
    if ((!lista.empty()) && (indice > 0) && (indice < this->lista.size())) {
        --indice;
        resposta = lista[indice];
    }
    return resposta;
}

Segmento* Posicao::abaixo(unsigned int indice)
{
    Segmento *resposta = 0;
    if ((!lista.empty()) && (indice < (lista.size() - 1))) {
        ++indice;
        resposta = lista[indice];
    }
    return resposta;
}

void Posicao::trocar(unsigned int primeiro, unsigned int segundo)
{
    Segmento *aux = lista.at(primeiro);
    lista.at(primeiro) = lista.at(segundo);
    lista.at(segundo) = aux;
}
