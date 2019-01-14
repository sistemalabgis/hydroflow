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

#include "hydroflow.h"
#include <QObject>
#include <vector>
#include <string>
#include <QString>
#include <time.h>
#include "feicao.h"
#include "pontovarredura.h"
#include "utils/modulo.h"
#include "windows.h"

#include <QDebug>


Hydroflow::Hydroflow(ConjuntoFeicao *drenagem, ConjuntoFeicao *limite, Parametros *parametros, Mensagem *saida)
{
    this->drenagem = drenagem;
    this->limite = limite;
    this->params = parametros;
    this->geo = new Geometria(parametros->getToleranciaXY());
    this->varredura = new Varredura(this->geo);
    this->relacoesTopologicas = new Relacao(saida);
    this->posicao = new Posicao(geo, saida);
    this->saida = saida;
    moduloSetGeometria(geo);
}

void Hydroflow::limparHydroflow()
{
    this->drenagem = 0;
    this->limite = 0;
    this->params = 0;
    delete this->geo;
    this->geo = 0;
    this->varredura->limparVarredura();
    delete this->varredura;
    this->varredura = 0;
    this->relacoesTopologicas->limparRelacao();
    delete this->relacoesTopologicas;
    this->relacoesTopologicas = 0;
    this->posicao->limparPosicao();
    delete this->posicao;
    this->posicao = 0;
    saida = 0;
}

void Hydroflow::montarLinhaVarredura()
{
    varredura->setLinhas(drenagem->getFeicoes(), drenagem->getQuantidadeFeicoes());
    varredura->setLinhas(drenagem->getFeicoesNovas());
    varredura->setLinhas(limite->getFeicoes(), limite->getQuantidadeFeicoes());
    varredura->ordenarLinhas();
}

void Hydroflow::avaliarSegmentos(Vertice *ponto, Segmento *acima, Segmento *abaixo)
{
    Vertice *pIntersecao = 0;

    // Verificando se há interseção.
    if (geo->pontoIntersecao(acima, abaixo, pIntersecao)) {

        // Selecionando a área de interesse para o ponto de interseção.
        if (((pIntersecao->getX() >= (ponto->getX() - geo->getTolerancia())) && (pIntersecao->getY() >= (ponto->getY() - geo->getTolerancia()))) ||
            ((pIntersecao->getX() > (ponto->getX() + geo->getTolerancia())) && (pIntersecao->getY() < (ponto->getY() - geo->getTolerancia())))) {

            // Separando as interseções do tipo encosta.
            if (!((geo->seIguais(pIntersecao, acima->getA()) && geo->seIguais(pIntersecao, abaixo->getB())) || //Casos 1, 2 e 3.
                    (geo->seIguais(pIntersecao, acima->getB()) && geo->seIguais(pIntersecao, abaixo->getA())) ||
                    (geo->seIguais(pIntersecao, acima->getA()) && geo->seIguais(pIntersecao, abaixo->getA())) || //Caso 5.
                    (geo->seIguais(pIntersecao, acima->getB()) && geo->seIguais(pIntersecao, abaixo->getB())))) { //Caso6.

                // Inserindo a interseção na lista de varredura.
                varredura->inserir(new VarreduraItem(pIntersecao, acima, abaixo, 2));
            }
        }
    }
}

void Hydroflow::varrerPlano()
{
    Segmento *registro = 0; // Segmento em análise.
    Segmento *acima = 0; // Segmento acima do segmento em análise.
    Segmento *abaixo = 0; // Segmento abaixo do segmento em análise.
    double linhaVarredura;
    double varreduraAnterior;
    int indice_A, indice_B;
    VarreduraItem *lv = 0;

    // Iniciando a varredura.
    lv = varredura->obterEvento();
    varreduraAnterior = lv->getX();

    while (lv != 0) {
        linhaVarredura = lv->getX();
        registro = lv->getSegmentoA();

        // Verificando se mudou a linha de varredura.
        if (geo->seMenor(varreduraAnterior, linhaVarredura)) {

            // Processando a linha de varredura anterior.
            this->processarPontosVarredura(varreduraAnterior);
            varreduraAnterior = linhaVarredura;
        }

        // Inserir segmento(s) no ponto de varredura.
        varredura->inserirPontoVarredura(lv);

        // Processando o evento da linha de varredura.
        switch (lv->getTipoEvento()) {
        case 0: // Extremo esquerdo. Segmento entrando.

            // Inserindo em posição.
            indice_A = posicao->inserir(registro);

            // Verificando segmento imediatamente acima.
            acima = posicao->acima(indice_A);
            if (acima != 0 ) { // Se há segmento acima:
                this->avaliarSegmentos(lv->getPonto(), acima, registro);
            }

            // Verificando segmento imediatamente abaixo.
            abaixo = posicao->abaixo(indice_A);
            if (abaixo != 0) { // Se há segmento abaixo:
                this->avaliarSegmentos(lv->getPonto(), registro, abaixo);
            }
            break;
        case 1: // Extremo direito. Segmento saindo.

            // Localizando o segmento em Posicao.
            indice_A = posicao->localizar(linhaVarredura, registro);

            // Verificando segmento imediatamente acima.
            acima = posicao->acima(indice_A);
            if (acima != 0 ) { // Se há segmento acima:

                // Verificando segmento imediatamente abaixo.
                abaixo = posicao->abaixo(indice_A);
                if (abaixo != 0) { // Se há segmento abaixo:
                    this->avaliarSegmentos(lv->getPonto(), acima, abaixo);
                }
            }

            // Excluindo de posição.
            posicao->excluir(indice_A);
            break;
        case 2: // Interseção.

            // Separando interseção por toque.
            if (!geo->seIguais(lv->getPonto(), lv->getSegmentoA()->getA()) &&
                !geo->seIguais(lv->getPonto(), lv->getSegmentoA()->getB()) &&
                !geo->seIguais(lv->getPonto(), lv->getSegmentoB()->getA()) &&
                !geo->seIguais(lv->getPonto(), lv->getSegmentoB()->getB())) {

                // Localizando os segmentos.
                indice_A = posicao->localizar(linhaVarredura, lv->getSegmentoA());
                indice_B = posicao->localizar(linhaVarredura, lv->getSegmentoB());

                // Trocando segmentos de posição .
                posicao->trocar(indice_A, indice_B);

                // Verificando segmento imediatamente acima.
                acima = posicao->acima(indice_A);
                if (acima != 0 ) { // Se há segmento acima:
                    this->avaliarSegmentos(lv->getPonto(), acima, lv->getSegmentoB());
                }

                // Verificando segmento imediatamente abaixo.
                abaixo = posicao->abaixo(indice_B);
                if (abaixo != 0) { // Se há segmento abaixo:
                    this->avaliarSegmentos(lv->getPonto(), lv->getSegmentoA(), abaixo);
                }
            }
            break;
        }

        lv->limparVarreduraItem();
        delete lv;
        lv = varredura->obterEvento();

    } // Fim da varredura

    // Processando os pontos da ultima linha de varredura.
    this->processarPontosVarredura(varreduraAnterior);

    posicao->limparPosicao();
}

/*
 * Valores para resultado:
 * 0- processamento sem erros;
 * 4- nó com mais de dois filhos;
 * 5- nó em loop;
 * 6- bacias interconectadas.
*/
No* Hydroflow::criarNos(Segmento *segmento, Segmento *pai, const vector<Segmento*> &irmaos, int &resultado)
{ //criarNo(segmento atual, segmento pai ou raiz, segmento irmão do atual, resultado da criação do nó)

    No *no = 0;

    if (resultado == 0) {

        //Verificando a condição de interconexão entre árvores.
        bool loopBacia = false;
        bool conexaoBacias = false;
        Feicao *feicaoCorrente = drenagem->getFeicao(segmento->getIdFeicao());
        if (segmento->getEFoz()) {
            /* A feição corrente é a feição da foz e o segmento pai é do limite da bacia.
             * É o início do processamento de uma bacia!
             */

            //Verificando se a feição da foz já foi processada.
            if (feicaoCorrente->getIdFeicaoFoz() < 0) {
                //Árvore ainda não processada.
                feicaoCorrente->setIdFeicaoFoz(feicaoCorrente->getId());
            } else {
                //Árvore já processada. Existe interconexão entre bacias!
                conexaoBacias = true;
            }
        } else {
            if (feicaoCorrente->getIdFeicaoFoz() < 0) {
                feicaoCorrente->setIdFeicaoFoz(drenagem->getFeicao(pai->getIdFeicao())->getIdFeicaoFoz());
            } else {
                //Feição já processada. Existe um loop na árvore!
                loopBacia = true;
            }
        }

        //Verificando a condição de loop na árvore ou interconexão entre árvores.
        if (!loopBacia && !conexaoBacias) {

            // Criando o nó.
            no = new No(segmento->getIdFeicao());

            //Processando o fluxo.
            if (feicaoCorrente->getNSegmentos() > 1) { //Vários segmentos.
                if (segmento->getId() == 0) {
                    no->setFluxo(2); //Inverter!
                } else {
                    no->setFluxo(1); //Manter.
                }
            } else { //Segmento único.

                // Verificando se o vértice "a" do segmento encosta no pai.
                if (segmento->getA()->eIgual(pai->getA()) || segmento->getA()->eIgual(pai->getB())) {
                    if (segmento->getA()->getId() == 0) {
                        no->setFluxo(2); //Inverter!
                    } else {
                        no->setFluxo(1); //Manter.
                    }
                } else { // Então é o vértice "b" que encosta no pai.
                    if (segmento->getB()->getId() == 0) {
                        no->setFluxo(2); //Inverter!
                    } else {
                        no->setFluxo(1); //Manter.
                    }
                }
            }

            // Obtendo os filhos.
            int paiID;
            if (pai->getIdConjunto() == 1) { //Limite da bacia.
                paiID = -1;
            } else {
                paiID = pai->getIdFeicao(); //Rede de drenagem.
            }
            Segmento *novoPai = 0; //O novo pai é o segmento atual!
            vector<Segmento*> filhosSegmento = relacoesTopologicas->getFilhosNo(segmento->getIdFeicao(), paiID, irmaos, novoPai);

            // Classificando o nó.
            No *filhoNo = 0;

            /*
             * Instanciar o nó com valor inicial 1 para
             * Strahler e Shreve para eliminar a necessidade
             * de usar os métodos No::setStrahler() e No::setShreve().
             * Não será necessário avaliar (filhosSegmento.size() == 0).
             *
             */
            if (filhosSegmento.size() == 0) { //Nó sem filhos. É folha!

                //Classificando por Strahler
                if (params->getTipoOrdemStrahler() > 0) {
                    no->setStrahler(1);
                }

                //Classificando por Shreve.
                if (params->getEOrdemShreve()) {
                    no->setShreve(1);
                }

            } else if (filhosSegmento.size() == 1) { //Fazer classificação do pai igual a do filho.

                // Processando o filho único.
                filhoNo = criarNos(filhosSegmento[0], segmento, filhosSegmento, resultado);

                if (resultado == 0) {
                    no->inserirFilhoNo(filhoNo);

                    //Classificando por Strahler. Passar essa lógica para No::inserirFilhoNo()!
                    if (params->getTipoOrdemStrahler() > 0) {
                        no->setStrahler(filhoNo->getStrahler());
                    }

                    //Classificando por Shreve. Passar essa lógica para No::inserirFilhoNo()!
                    if (params->getEOrdemShreve()) {
                        no->setShreve(filhoNo->getShreve());
                    }
                }
            } else { //Nó com dois ou mais filhos.
                if ((filhosSegmento.size() > 2) && (params->getTipoOrdemStrahler() == 1)) {

                    //Gravando a mensagem de mais de três afluentes no log.
                    QString msg_1 = QObject::tr("Aviso! Estrutura topológica não esperada para hierarquização Strahler.");
                    QString msg_2 = QObject::tr(" A Feição FID");
                    QString msg_3 = QObject::tr(" recebe mais de dois afluentes em um mesmo ponto.");
                    QString msg_4 = QObject::tr("Os identificadores desses afluentes são:");
                    QString msg_5 = QObject::tr("O resultado final apresentará a classificação por Strahler relaxado.");
                    saida->append(msg_1 + msg_2 + QString::number(segmento->getIdFeicao()) + msg_3 + "\n" + msg_4);

                    //Listando os filhos.
                    Segmento *filho = 0;
                    for (unsigned int i = 0; i < filhosSegmento.size(); ++i) {
                        filho = filhosSegmento[i];
                        saida->append("   " + QString::number(i + 1) + " - FID" + QString::number(filho->getIdFeicao()));
                    }
                    saida->append("\n" + msg_5);

                    //Configurando a classificação Strahler leve.
                    params->setTipoOrdemStrahler(2);
                }

                //Cadastrando os nós filhos.
                for (unsigned int i = 0; i < filhosSegmento.size(); ++i) {
                    filhoNo = criarNos(filhosSegmento.at(i), segmento, filhosSegmento, resultado);

                    if (resultado == 0) {
                        /*
                         * As regras para a classificação por Strahler e Shreve estão no método
                         * No::inserirFilhoNo().
                         */
                        no->inserirFilhoNo(filhoNo);
                    } else {
                        break;
                    }
                }
            }

            //Gravando classificação.
            if (resultado == 0) {
                this->drenagem->setClassificacaoFeicao(no->getIdFeicao(), no->getFluxo(), no->getStrahler(), no->getShreve());
            }
        } else { //Feição já processada!

            QString msg_1 = QObject::tr("Erro! impossível continuar! Estrutura topológica não esperada para hierarquização.");
            QString msg_2;
            QString msg_3;
            QString msg_4;
            QString msg_5 = QObject::tr("Confira a topologia e execute o processamento novamente.");
            if (loopBacia) {
                resultado = 5; //
                msg_2 = QObject::tr(" Existem feições em anel (loop).");
                msg_4 = QObject::tr("Verifique as feições: FID") + QString::number(segmento->getIdFeicao())
                                + QObject::tr(" e FID") + QString::number(pai->getIdFeicao()) + ".";

                //Gravando o erro no log.
                saida->append(msg_1 + msg_2 + "\n" + msg_4 + "\n\n" + msg_5);

            } else { //Conexão entre bacias.
                resultado = 6;
                msg_2 = QObject::tr(" Os sistemas de drenagem associados a foz FID");
                msg_3 = QObject::tr(" e a foz FID");
                msg_4 = QObject::tr(" estão conectados.");

                //Gravando o erro no log.
                saida->append(msg_1 + msg_2 + QString::number(feicaoCorrente->getId())
                              + msg_3 + QString::number(feicaoCorrente->getIdFeicaoFoz())
                              + msg_4 + "\n\n" + msg_5);
            }
        }
    }
    return no;
}

/*
 * Valores de retorno:
 * 0- processamento correto;
 * 2- foz não identificada;
 * 3- mais de uma foz identificada; //Não utilizado no processamento de várias fozes!
 * 4- nó com mais de dois filhos;
 * 5- nó em loop;
 * 6- bacias interconectadas;
 */
int Hydroflow::montarArvore()
{
    int resultado = 0;

    if(relacoesTopologicas->getQuantidadeFozes() > 0) {

        // Montando os índices dos eventos.
        relacoesTopologicas->montarIndices();

        //Construindo a árvore que representa cada bacia.
        RelacaoItem relacaoFoz;
        No *raiz = 0;
        vector<Segmento*> filhos;
        for (int i = 0; i < relacoesTopologicas->getQuantidadeFozes(); ++i) {
            relacaoFoz = relacoesTopologicas->getFoz(i);

            //origem: segmento da foz; destino: segmento do limite.
            raiz = criarNos(relacaoFoz.getOrigem(), relacaoFoz.getDestino(), filhos, resultado);
            delete raiz;
            raiz = 0;
        }
    } else {
        resultado = 2; // Foz não identificada!!!
    }

    return resultado;
}

/*Códigos de retorno:
0 - Processamento concluído com sucesso!
1 - Processamento concluído com alertas!
2 - Foz não identificada.
3 - Mais de uma foz identificada.
4 - Feição com mais de dois afluentes.
5 - Relações topológicas inesperadas! (listado no log)
 */
int Hydroflow::classificarBacia()
{
    int resultado = 0;

    // Montando a linha de varredura.
    montarLinhaVarredura();

    // Varrendo o plano;
    varrerPlano();

    // Verificando a existência de relações topológicas inesperadas (toque e interseção).
    if (relacoesTopologicas->haErros()) {

        // Listando os eventos estranhos.
        relacoesTopologicas->listarEventosInesperados(saida);
        resultado = 5;
    } else {

        // Classificando a bacia hidrográfica.
        resultado = montarArvore();

        // Validando o processamento das feições.
        if (resultado == 0) {
            resultado = avaliarProcessamento();
        }
    }
    return resultado;
}

void Hydroflow::processarPontosVarredura(double linhaVarredura)
{
    Segmento *segmento = 0;
    Vertice *ponto = 0;
    vector<boolean> teste;

    // Obtendo o primiro ponto de varradura.
    PontoVarredura *pontoVarredura = varredura->getPontoVarredura(linhaVarredura);
    while (pontoVarredura != 0) {
        if (pontoVarredura->getQuantidadeSegmentos() > 1) { // Há relações topológicas.
            ponto = pontoVarredura->getPonto();

            // Testando os segmentos.
            for (int k = 0; k < pontoVarredura->getQuantidadeSegmentos(); ++k) {
                segmento = pontoVarredura->getSegmento(k);
                if ((geo->seIguais(ponto, segmento->getA()) && (segmento->getA()->eExtremo() || (segmento->getIdConjunto() == 1)))
                        || (geo->seIguais(ponto, segmento->getB()) && (segmento->getB()->eExtremo() || (segmento->getIdConjunto() == 1)))) {
                    teste.push_back(true);
                } else {
                    teste.push_back(false);
                }
            }

            // Avaliando as relações topológicas.
            int j;
            for (int i = 0; i < (pontoVarredura->getQuantidadeSegmentos() - 1); ++i) {
                j = i + 1;
                for (j; j < pontoVarredura->getQuantidadeSegmentos(); ++j) {
                    if (teste.at(i) && teste.at(j)) { // Encosta.
                        this->relacoesTopologicas->inserirRelacao(pontoVarredura->getSegmento(i), pontoVarredura->getSegmento(j), 0);
                    } else if (teste.at(i) || teste.at(j)) { // Toca.
                        this->relacoesTopologicas->inserirRelacao(pontoVarredura->getSegmento(i), pontoVarredura->getSegmento(j), 1);
                    } else { // Intercepta.
                        this->relacoesTopologicas->inserirRelacao(pontoVarredura->getSegmento(i), pontoVarredura->getSegmento(j), 2);
                    }
                }
            } // Fim da iteração na listaSegmentos.
            teste.clear();
        } // Fim da avaliação das relações topológicas.

        // Limpando o ponto de varedura corrente.
        pontoVarredura->limparPontoVarredura();
        delete pontoVarredura;
        pontoVarredura = 0;

        // Lendo o próximo ponto de varredura.
        pontoVarredura = varredura->getPontoVarredura(linhaVarredura);
    } // Fim do processamento dos pontos da linha de varredura anterior.
}

int Hydroflow::avaliarProcessamento()
{
    int resultado = 0;
    QString msg_0;
    QString msg_1 = QObject::tr("Aviso: a feição FID");
    QString msg_2 = QObject::tr(" não foi processada. Confira a topologia da rede de drenagem.");
    QString msg_3 = QObject::tr("Feicao nao processada.");
    Feicao *feicao;
    for (int i = 0; i < drenagem->getTotalFeicoes(); ++i) {
        feicao = drenagem->getFeicao(i);
        if ((feicao->getFluxo() == 0) || ((params->getTipoOrdemStrahler() > 0) && (feicao->getStrahler() == 0)) || (params->getEOrdemShreve() && (feicao->getShreve() == 0))) {

            // Incluir mensagem no log.
            msg_0 = msg_1 + QString::number(feicao->getId()) + msg_2;
            saida->append(msg_0);

            // Incluir mensagem na observação.
            drenagem->setObservacao(feicao->getId(), msg_3);
            feicao->setTemObservacao(true);
            resultado = 1;
        }
    }
    return resultado;
}
