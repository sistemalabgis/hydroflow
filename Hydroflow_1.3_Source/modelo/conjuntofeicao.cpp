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

#include "conjuntofeicao.h"

ConjuntoFeicao::ConjuntoFeicao()
{
    this->idConjunto = 0;
    this->nomeArquivo = "";
    this->codigoTipo = 0;
    this->quantidadeFeicoes = 0;
    this->quantidadeFeicoesNovas = 0;
    this->quantidadeAtributos = 0;
}

ConjuntoFeicao::ConjuntoFeicao(int idFeicao, QString nomeArquivo, int codigoTipo)
{
    this->idConjunto = idFeicao;
    this->nomeArquivo = nomeArquivo;
    this->codigoTipo = codigoTipo;
    this->quantidadeFeicoes = 0;
    this->feicoes = 0;
    this->quantidadeFeicoesNovas = 0;
    this->quantidadeAtributos = 0;
}

void ConjuntoFeicao::LimparConjuntoFeicao()
{

    //Limpando as feiçoes.
    Feicao feic;
    for (int i = 0; i < this->quantidadeFeicoes; ++i) {
        feic = this->feicoes[i];
        feic.limparFeicao();
    }
    delete[] this->feicoes;
    this->feicoes = 0;
    this->quantidadeFeicoes = 0;

    //Limpando as feiçoes novas.
    Feicao *feicao = 0;
    for (unsigned int j = 0; j < this->feicoesNovas.size(); ++j) {
        feicao = this->feicoesNovas.at(j);
        feicao->limparFeicao();
        delete feicao;
        feicao = 0;
    }
    this->feicoesNovas.clear();
    vector<Feicao*> fn;
    this->feicoesNovas.swap(fn);

    //Limpando os atributos das feições novas.
    AtributoFeicaoNova *item;
    for (unsigned int k = 0; k < this->atributosFeicoesNovas.size(); ++k) {
        item = this->atributosFeicoesNovas.at(k);
        item->limparAtributos();
        delete item;
        item = 0;
    }
    this->atributosFeicoesNovas.clear();
    vector <AtributoFeicaoNova*> afn;
    this->atributosFeicoesNovas.swap(afn);

    //Limpando as observações.
    this->obs->limparObservacoes();
    delete this->obs;
    this->obs = 0;
}

QString ConjuntoFeicao::getNomeArquivo()
{
    return this->nomeArquivo;
}

int ConjuntoFeicao::getIdConjunto()
{
    return this->idConjunto;
}
Feicao* ConjuntoFeicao::getFeicoes()
{
    return this->feicoes;
}

void ConjuntoFeicao::setFeicoes(Feicao *feicoes, int nfeicoes)
{
    this->feicoes = feicoes;
    this->quantidadeFeicoes = nfeicoes;
}

Feicao* ConjuntoFeicao::getFeicao(int idFeicao)
{
    if ((idFeicao >= 0) && (idFeicao < this->quantidadeFeicoes)) {
        return &this->feicoes[idFeicao];
    } else if ((idFeicao >= this->quantidadeFeicoes) && (idFeicao < (this->quantidadeFeicoes + this->quantidadeFeicoesNovas))) {
        return this->getFeicaoNova(idFeicao - this->quantidadeFeicoes);
    } else {
        Feicao *resposta = 0;
        return resposta;
    }
}

int ConjuntoFeicao::getCodigoTipo()
{
    return this->codigoTipo;
}

int ConjuntoFeicao::getQuantidadeFeicoes()
{
    return this->quantidadeFeicoes;
}

bool ConjuntoFeicao::getCriarObservacao()
{
    return this->obs->temObservacao();
}

void ConjuntoFeicao::setClassificacaoFeicao(int idFeicao, int fluxo, int strahler, int shreve)
{
    Feicao *feicao;
    if (idFeicao < this->quantidadeFeicoes) {
        feicoes[idFeicao].setClassificacao(fluxo, strahler, shreve);
    } else {
        feicao = feicoesNovas.at(idFeicao - this->quantidadeFeicoes);
        feicao->setClassificacao(fluxo, strahler, shreve);
    }
}

Observacao* ConjuntoFeicao::getObservacoes()
{
    return this->obs;
}

void ConjuntoFeicao::setObservacoes(Observacao *obs)
{
    this->obs = obs;
}

void ConjuntoFeicao:: setObservacao(int idFeicao, QString texto)
{
    this->obs->setObservacao(idFeicao, texto);
}

int ConjuntoFeicao::getQuantidadeFeicoesNovas()
{
    return this->feicoesNovas.size();
}

Feicao* ConjuntoFeicao::getFeicaoNova(int indice)
{
    if ((indice >= 0) && (indice < this->quantidadeFeicoesNovas)) {
        return this->feicoesNovas.at(indice);
    } else {
        Feicao *resposta = 0;
        return resposta;
    }
}

void ConjuntoFeicao::setFeicaoNova(Feicao *feicao)
{
    this->feicoesNovas.push_back(feicao);
    this->quantidadeFeicoesNovas = this->quantidadeFeicoesNovas + 1;
}

const vector<Feicao*>& ConjuntoFeicao::getFeicoesNovas()
{
    return this->feicoesNovas;
}

void ConjuntoFeicao::setAtributosFeicaoNova(AtributoFeicaoNova *atributosFeicoesNovas)
{
    this->atributosFeicoesNovas.push_back(atributosFeicoesNovas);
}

Atributo* ConjuntoFeicao::getAtributosFeicaoNova(int idFeicao)
{
    Atributo *atributos = 0;
    int indice = this->localizarAtributos(0, (this->quantidadeFeicoesNovas - 1), idFeicao);
    if (indice != -1) {
        AtributoFeicaoNova *reg = this->atributosFeicoesNovas.at(indice);
        atributos = reg->getAtributos(); //(Atributo*)
    }
    return atributos;
}

int ConjuntoFeicao::getQuantidadeAtributos()
{
    return this->quantidadeAtributos;
}

void ConjuntoFeicao::setQuantidadeAtributos(int valor)
{
    this->quantidadeAtributos = valor;
}

int ConjuntoFeicao::localizarAtributos(int inicio, int fim, int idFeicao)
{
    int resposta = -1;
    if (inicio <= fim) {

        // Calculando o meio (indice).
        int meio = (int) (inicio + fim)/2;

        // Lendo o registro do meio.
        AtributoFeicaoNova *reg = this->atributosFeicoesNovas[meio];

        // Analisando.
        if (idFeicao == reg->getIdFeicao()) {
            resposta = meio;
        } else if (idFeicao < reg->getIdFeicao()) {
            if (meio > inicio) {
                resposta = this->localizarAtributos(inicio, meio - 1, idFeicao);
            }
        } else { // (idElemento > reg.getIdElemento)
            if (meio < fim) {
                resposta = this->localizarAtributos(meio + 1, fim, idFeicao);
            }
        }
    }
    return resposta;
}

int ConjuntoFeicao::getTotalFeicoes()
{
    return (this->quantidadeFeicoes + this->quantidadeFeicoesNovas);
}
