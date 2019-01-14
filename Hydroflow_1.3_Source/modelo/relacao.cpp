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

#include "relacao.h"
#include <QString>
#include "utils/modulo.h"
#include <algorithm>
#include <QObject>

#include <QDebug>

Relacao::Relacao(Mensagem *saida)
{
    this->saida = saida;
}

void Relacao::limparRelacao()
{
    // Limpando a lista de relações topológicas.
    this->lista.clear();
    vector<RelacaoItem> lst;
    this->lista.swap(lst);

    //Limpando a lista de relações inesperadas.
    vector<RelacaoItem> err;
    this->erros.clear();
    this->erros.swap(err);

    // Limpando a foz.
    vector<RelacaoItem> fz;
    this->fozes.clear();
    this->fozes.swap(fz);

    // Limpando os índices
    vector<IndiceItem*> ind;
    this->indice.clear();
    this->indice.swap(ind);
    vector<IndiceItem*> indp;
    this->indicePrimario.clear();
    this->indicePrimario.swap(indp);

    this->saida = 0;
}

void Relacao::inserir(unsigned int inicio, unsigned int fim, Segmento* origem, Segmento* destino, int tipo)
{
    if (inicio <= fim) {
        RelacaoItem item(origem, destino, tipo);

        // Calculando o meio (indice).
        unsigned int meio = (int) (inicio + fim)/2;

        // Lendo o item do meio.
        RelacaoItem itemMeio;
        if (tipo == 0) {
            itemMeio = this->lista[meio];
        } else {
            itemMeio = this->erros[meio];
        }

        // Comparando os itens.
        int comparacao = this->comparaPosicao_2(&item, &itemMeio);

        vector<RelacaoItem>::iterator it;

        // (item < itemMeio)
        if (comparacao == -1) {
            if ((inicio == fim) || (inicio == meio)) { // (inicio == fim) Não encontrou.

                //Inserir no ponteiro atual(meio).
                if (tipo == 0) {
                    it = this->lista.begin();
                    advance(it, meio);
                    this->lista.insert(it, item);
                } else {
                    it = this->erros.begin();
                    advance(it, meio);
                    this->erros.insert(it, item);
                }
            } else if (inicio < meio) { //(meio > 0)
                inserir(inicio, meio - 1 , origem, destino, tipo);
            } else { // É inicio da lista (meio = 0).
                if (tipo == 0) {
                    this->lista.insert(lista.begin(), item);
                } else {
                    this->erros.insert(erros.begin(), item);
                }
            }

        //(item > itemMeio)
        } else if (comparacao == 1) {
            if (tipo == 0) {
                if (meio == (this->lista.size() - 1)) { // Se meio é ultimo elemento.
                    this->lista.push_back(item);
                } else if (inicio == fim) {
                    it = this->lista.begin();
                    advance(it, meio); // advance(it, (meio + 1));
                    this->lista.insert(it, item);
                } else {
                    inserir((meio + 1), fim, origem, destino,tipo);
                }
            } else { //Cadastrar relações indesejadas.
                if (meio == (this->erros.size() -1)) { // Se meio é ultimo elemento.
                    this->erros.push_back(item);
                } else if (inicio == fim) {
                    it = this->erros.begin();
                    advance(it, meio); // advance(it, (meio + 1));
                    this->erros.insert(it, item);
                } else {
                    inserir((meio + 1), fim, origem, destino,tipo);
                }
            }
        } // Comparação igual a zero: relação já cadastrada. Nada fazer!
    } else { // Implementar mensagem de erro (inicio > fim)!?

    }
}

void Relacao::inserirFoz(Segmento *bacia, Segmento *limite)
{
    //Garantindo que o primeiro argumento é da bacia.
    RelacaoItem item;
    if (bacia->getIdConjunto() == 0) {
        bacia->setEFoz();
        item.setOrigem(bacia);
        item.setDestino(limite);
    } else {
        limite->setEFoz();
        item.setOrigem(limite);
        item.setDestino(bacia);
    }

    //Garantindo que a foz não foi incluida antes.
    bool incluida = false;
    for (unsigned int i = 0; i < fozes.size(); ++i) {
        if (item.getOrigem()->getIdFeicao() == fozes.at(i).getOrigem()->getIdFeicao()) {
            incluida = true;
            break;
        }
    }

    if (!incluida) {

        //Inserindo em fozes.
        fozes.push_back(item);
    }
}

void Relacao::inserirRelacao(Segmento *origem, Segmento *destino, int tipo)
{
    /* Tipos de relação topológica:
     * 0- Encosta;
     * 1- Toca;
     * 2- Intercepta.
     */

    // Garantindo que o FID do primeiro segmento seja menor que o FID do segundo.
    if(destino->getIdFeicao() < origem->getIdFeicao()) {
        Segmento *temp = origem;
        origem = destino;
        destino = temp;
    }

    /* Verificando se é relação entre foz e limite.
       Só aceita relação topológica do tipo encosta entre foz e limite da bacia!
    */
    if ((origem->getIdConjunto() != destino->getIdConjunto()) && (tipo == 0)) {
        this->inserirFoz(origem, destino); //Versão 1.3!

    //Verificando se os segmenos são de feições diferentes da bacia.
    } else if ((origem->getIdFeicao() != destino->getIdFeicao()) && ((origem->getIdConjunto() == 0) && (destino->getIdConjunto() == 0))) {
        if (tipo == 0) { // Escosta.
            if (this->lista.empty()) {
                RelacaoItem item(origem, destino, tipo);
                this->lista.push_back(item);
            } else {
                this->inserir(0, (this->lista.size() - 1), origem, destino, tipo);
            }
        } else { //Tipo de evento errado. Incluindo na lista de erros.
            if (this->erros.empty()) {
                RelacaoItem item(origem, destino, tipo);
                this->erros.push_back(item);
            } else {
                this->inserir(0, (this->erros.size() - 1), origem, destino, tipo);
            }
        }
    } //Os segmentos da orígem e destino são da mesma feição. Não gravar!
}

RelacaoItem* Relacao::getEvento(int indice)
{
    return &this->lista[indice];
}

vector<Segmento*> Relacao::getFilhosNo(int idFeicao, int idFeicaoPai, vector<Segmento*> filhosPai, Segmento *&novoPai)
{
    vector<Segmento*> resposta;

    // Obtendo o índice primário.
    int indicePrimario = getIndicePrimario(0, this->indicePrimario.size() - 1, idFeicao);
    if (indicePrimario >= 0) {

        bool fim = false;
        bool avaliar;
        bool eFilho;
        IndiceItem *itemPrimario;
        RelacaoItem itemEvento;
        Segmento *segmentoRelacionado;
        Segmento *segmentoFilho;
        int tamanhoIndice = this->indice.size();

        while ((!fim) && (indicePrimario < tamanhoIndice)) {

            // Obtendo o índice do evento.
            itemPrimario = this->indice[indicePrimario];

            if (itemPrimario->getFeicao() == idFeicao) {

                // Lendo evento.
                itemEvento = this->lista[itemPrimario->getValor()];
/*
                if ((itemEvento.getOrigem()->getIdFeicao() == idFeicao) && (itemEvento.getDestino()->getIdFeicao() != idFeicaoPai) && (itemEvento.getDestino()->getIdFeicao() != idFeicaoIrmao)) {
                    resposta.push_back(itemEvento.getDestino());
                    novoPai = itemEvento.getOrigem();
                } else if ((itemEvento.getDestino()->getIdFeicao() == idFeicao) && (itemEvento.getOrigem()->getIdFeicao() != idFeicaoPai) && (itemEvento.getOrigem()->getIdFeicao() != idFeicaoIrmao)) {
                    resposta.push_back(itemEvento.getOrigem());
                    novoPai = itemEvento.getDestino();
                }
*/

                avaliar = true;
                if ((itemEvento.getOrigem()->getIdFeicao() == idFeicao) && (itemEvento.getDestino()->getIdFeicao() != idFeicaoPai)) {
                    segmentoRelacionado = itemEvento.getDestino();
                    novoPai = itemEvento.getOrigem();
                } else if ((itemEvento.getDestino()->getIdFeicao() == idFeicao) && (itemEvento.getOrigem()->getIdFeicao() != idFeicaoPai)) {
                    segmentoRelacionado = itemEvento.getOrigem();
                    novoPai = itemEvento.getDestino();
                } else {
                    avaliar = false;
                }

                if (avaliar) {
                    eFilho = true;
                    for (unsigned int i = 0; i < filhosPai.size(); ++i) {
                        segmentoFilho = filhosPai.at(i);
                        if (segmentoRelacionado->getIdFeicao() == segmentoFilho->getIdFeicao()) {
                            eFilho = false;
                        }
                    }
                    if (eFilho) {
                        resposta.push_back(segmentoRelacionado);
                    }
                }

                ++indicePrimario;
            } else {
                fim = true;
            }
        } // Fim do loop.
    }

    return resposta;
}

/* Resultado:
 * -1: se primeiro < segundo;
 * 0: se primeiro = segundo;
 * 1: se primeiro > segundo.
 */
int Relacao::comparaPosicao_2(RelacaoItem *primeiro, RelacaoItem *segundo)
{
    int resposta = 0;
    if (((primeiro->getOrigem()->getIdFeicao() == segundo->getOrigem()->getIdFeicao()) &&
          (primeiro->getDestino()->getIdFeicao() == segundo->getDestino()->getIdFeicao())) &&
          (primeiro->getTipo() == segundo->getTipo())) {
        resposta = 0;
    } else if((primeiro->getOrigem()->getIdFeicao() < segundo->getOrigem()->getIdFeicao()) ||
       ((primeiro->getOrigem()->getIdFeicao() == segundo->getOrigem()->getIdFeicao()) && (primeiro->getDestino()->getIdFeicao() < segundo->getDestino()->getIdFeicao())) ||
       ((primeiro->getOrigem()->getIdFeicao() == segundo->getOrigem()->getIdFeicao()) && (primeiro->getDestino()->getIdFeicao() == segundo->getDestino()->getIdFeicao()) && (primeiro->getTipo() < segundo->getTipo()))) {
        resposta = -1;
    } else if((primeiro->getOrigem()->getIdFeicao() > segundo->getOrigem()->getIdFeicao()) ||
              ((primeiro->getOrigem()->getIdFeicao() == segundo->getOrigem()->getIdFeicao()) && (primeiro->getDestino()->getIdFeicao() > segundo->getDestino()->getIdFeicao())) ||
              ((primeiro->getOrigem()->getIdFeicao() == segundo->getOrigem()->getIdFeicao()) && (primeiro->getDestino()->getIdFeicao() == segundo->getDestino()->getIdFeicao()) && (primeiro->getTipo() > segundo->getTipo()))) {
        resposta = 1;
    }
    return resposta;
}

Segmento* Relacao::getLimite()
{
    return this->limite;
}

RelacaoItem Relacao::getFoz(int indice)
{
    return this->fozes.at(indice);
}

int Relacao::getQuantidadeFozes()
{
    return fozes.size();
}

// Índices
int Relacao::getIndicePrimario(int inicio, int fim, int idFeicao)
{
    int resposta = -1;
    if ((inicio <= fim) && (!lista.empty())) {
        int meio = (inicio + fim) / 2;
        IndiceItem *item = indicePrimario[meio];

        if (idFeicao == item->getFeicao()) {
            resposta = item->getValor();
        } else {
            if (idFeicao < item->getFeicao()) {
                resposta = getIndicePrimario(inicio, meio - 1, idFeicao);
            } else {
                resposta = getIndicePrimario(meio + 1, fim, idFeicao);
            }
        }
    }
    return resposta;
}

void Relacao::montarIndices()
{

    // Montando índice principal.
    this->indice.clear();
    RelacaoItem itemEvento;
    int tamanhoLista = this->lista.size();
    for (int i = 0; i < tamanhoLista; ++i) {
        itemEvento = this->lista[i];

        //Verificando se são ocorrências de bacia.
        this->indice.push_back(new IndiceItem(itemEvento.getOrigem()->getIdFeicao(), i));
        this->indice.push_back(new IndiceItem(itemEvento.getDestino()->getIdFeicao(), i));
    }

    // Ordenando o índice principal.
    sort(this->indice.begin(), this->indice.end(), compararItemIndice);

    //Montando o índice primário.
    this->indicePrimario.clear();
    int tamanho = this->indice.size();
    int idFeicao = -1;
    IndiceItem *itemIndice;
    for (int j = 0; j < tamanho; ++j) {
        itemIndice = this->indice[j];
        if ((itemIndice->getFeicao() != idFeicao)){ // && (itemIndice->getSegmento() != idSegmento)
            idFeicao = itemIndice->getFeicao();
            this->indicePrimario.push_back(new IndiceItem(idFeicao, j));
        }
    }
}

int Relacao::getQuantidadeRelacoes()
{
    return this->lista.size();
}

bool Relacao::haErros()
{
    return (this->erros.size() > 0);
}

void Relacao::listarEventosInesperados(Mensagem *saida)
{
    QString msg_1 = QObject::tr("Erro! Impossível continuar! Encontradas relações topológicas inesperadas.");
    QString msg_2 = QObject::tr("São esperadas somente relações de encosta, entretanto foram encontradas as relações:");
    QString msg_3 = QObject::tr("Consulte a documentação deste aplicativo para mais detalhes sobre as relações topológicas esperadas e inesperadas.");
    QString msg_4 = QObject::tr("Confira a topologia e execute o processamento novamente.");
    saida->append(msg_1 + "\n" + msg_2);

    if (!erros.empty()) {
        vector<RelacaoItem>::iterator it = erros.begin();
        RelacaoItem reg;
        QString tipoEvento;
        for (it; it != erros.end(); ++it) {
            reg = *it;
            switch (reg.getTipo()) {
            case 1:
                tipoEvento = QObject::tr(" toca em FID");
                break;
            case 2:
                tipoEvento = QObject::tr(" intercepta FID");
                break;
            default:
                tipoEvento = " - FID";
                break;
            }
            saida->append("   FID" + QString::number(reg.getOrigem()->getIdFeicao()) + tipoEvento + QString::number(reg.getDestino()->getIdFeicao()));
        }
        saida->append("   " + QObject::tr("Total de relações: ") + QString::number(this->erros.size()) + ".");
        saida->append("\n" + msg_3);
        saida->append(msg_4);
    }
}
