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

#include "shpconjuntofeicaodao.h"
#include <QObject>
#include "utils/utils.h"
#include <iostream>
#include "sstream"
#include <QFileInfo>

#include <QDebug>

SHPConjuntoFeicaoDAO::SHPConjuntoFeicaoDAO()
{
    this->tolerancia = 0;
}

SHPConjuntoFeicaoDAO::SHPConjuntoFeicaoDAO(double tolerancia)
{
    this->tolerancia = tolerancia;
}

SHPHandle SHPConjuntoFeicaoDAO::criarConjuntoFeicao(QString nomeArquivo, int tipo)
{
    SHPHandle SHPfigura = 0;
    if ((tipo == SHPT_POINT)||(tipo == SHPT_ARC)||(tipo == SHPT_POLYGON)||(tipo == SHPT_MULTIPOINT)) {
        SHPfigura = SHPCreate(Utils::converterString(nomeArquivo), tipo);
    }
    return SHPfigura;
}

SHPHandle SHPConjuntoFeicaoDAO::abrirConjuntoFeicao(QString nomeArquivo, int modo)
{
    if (modo == 1){
        return SHPOpen(Utils::converterString(nomeArquivo), "rb+");
    } else {
        return SHPOpen(Utils::converterString(nomeArquivo), "rb");
    }
}

void SHPConjuntoFeicaoDAO::fecharConjuntoFeicao(SHPHandle handle)
{
    SHPClose(handle);
}

DBFHandle SHPConjuntoFeicaoDAO::criarDados(QString nomeArquivo)
{
    return DBFCreate(Utils::converterString(nomeArquivo));
}

DBFHandle SHPConjuntoFeicaoDAO::abrirDados(QString nomeArquivo, int modo)
{
    if (modo == 1){
        return DBFOpen(Utils::converterString(nomeArquivo), "rb+");
    } else {
        return DBFOpen(Utils::converterString(nomeArquivo), "rb");
    }
}

void SHPConjuntoFeicaoDAO::fecharDados(DBFHandle handle)
{
    DBFClose(handle);
}

// Tipos: 0- bacia; 1- limite.
ConjuntoFeicao* SHPConjuntoFeicaoDAO::lerConjuntoFeicao(QString nomeArquivo, int tipoFigura)
{
    ConjuntoFeicao *figura = 0;

    SHPHandle SHPfigura = abrirConjuntoFeicao(nomeArquivo, LEITURA);
    if (SHPfigura) {
        DBFHandle DBFfigura = abrirDados(nomeArquivo, LEITURA);
        if (DBFfigura) {

            SHPObject *registroSHP = 0;
            int nSegmentos = 0;
            int novoRegistro = SHPfigura->nRecords;
            bool eNovoElemento;
            int idFeicao;
            int idSegmento;
            int idVertice;
            int nAtributos = 0; //Quantidade de atributos do DBF da figura.
            int nVertices = 0; //Quantidade de vértices na feição.
            int fimParte;
            int elementoInicial = 0;
            Vertice *vertices = 0;
            Feicao *feicoes = new Feicao[SHPfigura->nRecords];
            Vertice *verticeA = 0;
            Vertice *verticeB = 0;
            bool eUltimo;
            Observacao *obs = new Observacao();
            QString temp;
            QString msg_1 = QObject::tr("Feição com partes multiplas. Veja: ");
            QString msg_2 = QObject::tr("Parte da feição FID");
            QString msg_3 = QObject::tr("Feição não processada.");

            // Instanciando a figura.
            figura = new ConjuntoFeicao(tipoFigura, nomeArquivo, SHPfigura->nShapeType);

            // Montando as feições
            for (int i = 0; i < SHPfigura->nRecords; ++i) {

                // Lendo o registro
                registroSHP = SHPReadObject(SHPfigura, i);
                if (registroSHP->nVertices > 1) { //Montando os segmentos (modelo Canônico)

                    // Lendo as partes.
                    for (int parte = 0; parte < registroSHP->nParts; ++parte) {
                        idVertice = registroSHP->panPartStart[parte];

                        // Determinando o ultimo vetor da parte.
                        if ((parte + 1) < registroSHP->nParts) {
                            fimParte = registroSHP->panPartStart[parte + 1] - 1;
                        } else {
                            fimParte = registroSHP->nVertices - 1;
                        }

                        // Dimensionando o vetor de segmentos da feição.
                        nVertices = fimParte - idVertice + 1;
                        vertices = new Vertice[nVertices];
                        nSegmentos = fimParte - idVertice;
                        Segmento *segmentos = new Segmento[nSegmentos];

                        if (parte == 0) { //Feição original.
                            idFeicao = i;
                            elementoInicial = i;
                            eNovoElemento = false;
                        } else {// Adicionar novo registro no ShapeFile.
                            idFeicao = novoRegistro;
                            ++novoRegistro;
                            eNovoElemento = true;
                        }

                        // Lendo os vértices da parte
                        int p = 0;
                        eUltimo = false;
                        for (int j = idVertice; j <= fimParte; ++j) {
                            if (j == fimParte) {
                                eUltimo = true;
                            }
                            vertices[p] = Vertice(p, registroSHP->padfX[j], registroSHP->padfY[j], eUltimo);
                            ++p;
                        }

                        //Montando os segmentos.
                        idSegmento = 0;
                        for (p = 0; p < (nVertices - 1); ++p) {
                            verticeA = &vertices[p];
                            verticeB = &vertices[p + 1];

                            if ((verticeA->getX() + this->tolerancia) < verticeB->getX()) {
                                segmentos[idSegmento] = Segmento(idSegmento, idFeicao, tipoFigura, verticeA, verticeB);
                            } else if ((verticeB->getX() + this->tolerancia) < verticeA->getX()) {
                                segmentos[idSegmento] = Segmento(idSegmento, idFeicao, tipoFigura, verticeB, verticeA);
                            } else { // É vertical!
                                if ((verticeA->getY() + this->tolerancia) < verticeB->getY()) {
                                    segmentos[idSegmento] = Segmento(idSegmento, idFeicao, tipoFigura, verticeA, verticeB);
                                } else {
                                    segmentos[idSegmento] = Segmento(idSegmento, idFeicao, tipoFigura, verticeB, verticeA);
                                }
                            }
                            ++idSegmento; //OK!
                        }

                        // Montando a feição da parte.
                        if (!eNovoElemento) {
                            feicoes[idFeicao] = Feicao(idFeicao, tipoFigura, registroSHP->nSHPType, 1, nVertices, vertices, nSegmentos, segmentos);
                            if (registroSHP->nParts > 1) {
                                feicoes[idFeicao].setTemObservacao(true);
                                obs->setObservacao(idFeicao, msg_1);
                            }
                        } else {

                            // Guardando a feição na lista de feições novas.
                            Feicao *nova = new Feicao(idFeicao, tipoFigura, registroSHP->nSHPType, 1, nVertices, vertices, nSegmentos, segmentos);
                            nova->setTemObservacao(true);
                            figura->setFeicaoNova(nova);

                            // Copiando os atributos do elemento inicial para o atributo novo.
                            Atributo *atributos = this->lerAtributos(DBFfigura, elementoInicial, nAtributos);
                            AtributoFeicaoNova *atributosNovos = new AtributoFeicaoNova(idFeicao, atributos);
                            figura->setAtributosFeicaoNova(atributosNovos);

                            // Editando as observações.
                            temp = obs->getObservacao(elementoInicial)+ " FID" + QString::number(idFeicao) + ";";
                            obs->setObservacao(elementoInicial, temp);
                            temp = msg_2 + QString::number(elementoInicial) + ".";
                            obs->setObservacao(idFeicao, temp);
                        }
                    } // Fim da montagem da parte.

                    // Fim da montagem dos segmentos.
                } else if (registroSHP->nVertices == 1) {
                    nVertices = 1;
                    vertices = new Vertice(0, registroSHP->padfX[0], registroSHP->padfY[0]);
                    nSegmentos = 1;
                    Segmento *segmentos = new Segmento[nSegmentos];

                    segmentos[0] = Segmento(0, i, tipoFigura, vertices, vertices); //Montando um segmento degenerado para respeitar o modelo de dados!
                    feicoes[i] = Feicao(i, tipoFigura, registroSHP->nSHPType, 1, nVertices, vertices, nSegmentos, segmentos);
                    if (tipoFigura == 0) { //É bacia. Não processar o elemento!
                        feicoes[i].setProcessar(false);
                        feicoes[i].setTemObservacao(true);
                        obs->setObservacao(i, msg_3);
                    }
                } else { //Não tem vertices! Situação de erro do ShapeFile.
                    nSegmentos = 0;
                    Segmento *segmentos = 0; // = new Segmento[nSegmentos];
                    feicoes[i] = Feicao(i, tipoFigura, registroSHP->nSHPType, registroSHP->nParts, nSegmentos, segmentos);
                    feicoes[i].setProcessar(false);
                    feicoes[i].setTemObservacao(true);
                    obs->setObservacao(i, msg_3);
                }
                SHPDestroyObject(registroSHP);
            } // Fim da montagem das feições.

            // Cadastrando demais atributos da figura.
            figura->setFeicoes(feicoes, SHPfigura->nRecords);
            figura->setObservacoes(obs);

            fecharDados(DBFfigura);
        }
        fecharConjuntoFeicao(SHPfigura);
    }
    return figura;
}

Atributo* SHPConjuntoFeicaoDAO::lerAtributos(DBFHandle DBFtabela, int idFeicao, int &nAtributos)
{
    Atributo *atributos = 0 ;
    if (DBFtabela && (idFeicao <= DBFGetRecordCount(DBFtabela))) {

        // Lendo os atributos do DBF.
        nAtributos = DBFGetFieldCount(DBFtabela);
        if (nAtributos > 0) {
            atributos = new Atributo[nAtributos];
            char nomeCampo[12];
            DBFFieldType tipoCampo;
            int tipo = 0;
            int tamanho; //Tamanho do atributo
            int decimal; //Quantidade de casas decimais;
            const char *valorChar;
            int valorInt;
            double valorDbl;
            stringstream valorSts;
            ostringstream valorDbs;
            string valor = "";

            for (int i = 0; i < nAtributos; ++i) {
                tipoCampo = DBFGetFieldInfo(DBFtabela, i, nomeCampo, &tamanho, &decimal);
                if (tipoCampo == FTString) {
                    tipo = 1;
                    valorChar = DBFReadStringAttribute(DBFtabela, idFeicao, i);
                    valor = string(valorChar);
                } else if (tipoCampo == FTInteger) {
                    tipo = 2;
                    valorInt = DBFReadIntegerAttribute(DBFtabela, idFeicao, i);
                    valorSts << valorInt;
                    valor = valorSts.str();
                } else if (tipoCampo == FTDouble){
                    tipo = 3;
                    valorDbl = DBFReadDoubleAttribute(DBFtabela, idFeicao, i);
                    valorDbs << valorDbl;
                    valor = valorDbs.str();
                } else if (tipoCampo == FTLogical) {
                    tipo = 4;
                    const char valorConsChr = DBFReadLogicalAttribute(DBFtabela, idFeicao, i)[0];
                    valor = valorConsChr;
                } else if (tipoCampo == FTDate) {
                    tipo = 5;
                    valorInt = DBFReadDateAttribute(DBFtabela, idFeicao, i);
                    valorSts << valorInt;
                    valor = valorSts.str();
                }
                atributos[i] = Atributo(nomeCampo, tipo, valor, tamanho, decimal);
            }
        } //if(atributos)
    }

    return atributos;
}

bool SHPConjuntoFeicaoDAO::criarNovaTabela(QString nomeArquivo, DBFHandle *DBFtabelaOrig, Parametros *params, bool temObservacao)
{
    bool resultado = false;

    //Criando a nova tabela.
    DBFHandle DBFtabela = criarDados(nomeArquivo);

    if (DBFtabela && DBFtabelaOrig) {
        resultado = true;

        //Criando os campos da tabela original.
        int nCampos = DBFGetFieldCount(*DBFtabelaOrig);
        DBFFieldType *listaTipos = new DBFFieldType[nCampos];

        if (nCampos > 0) {
            char nomeCampo[10];
            DBFFieldType tipoCampo;
            int tamanho;
            int decimal;
            for (int i = 0; i < nCampos; ++i) {
                tamanho = 0;
                decimal = 0;
                tipoCampo = DBFGetFieldInfo(*DBFtabelaOrig, i, nomeCampo, &tamanho, &decimal);
                listaTipos[i] = tipoCampo;
                DBFAddField(DBFtabela, nomeCampo, tipoCampo, tamanho, decimal);
            }
        }

        //Verificando a existência dos novos atributos na tabela de orígem.
        QString nomeAtribFluxo = QObject::tr("Fluxo");
        QString nomeAtribStrahler = "Strahler";
        QString nomeAtribStrahlerR = QObject::tr("StrahlerR");
        QString separadorStrahlerR = "";
        QString nomeAtribShreve = "Shreve";
        QString nomeAtribObs = "Obs:";
        QString separador = "_";

        bool fim = false;
        bool ok = false;
        int comp = 0;
        int compGeral = comp;
        int regID;
        int regID2;
        QString nome;
        do {

            //Verificando o atributo fluxo.
            if (comp == 0) {
                nome = nomeAtribFluxo;
            } else {
                nome = nomeAtribFluxo + separador + QString::number(comp);
            }

            do {
                regID = DBFGetFieldIndex(*DBFtabelaOrig, Utils::converterString(nome));
                if (regID >= 0) { //O atributo já existe na tabela original.
                    ++comp;
                    nome = nomeAtribFluxo + separador + QString::number(comp);
                } else { //O atributo é novo.
                    ok = true;
                }
            } while (!ok);

            //Verificando o atributo Strahler.
            QString nomeR;
            if (comp == 0) {
                nome = nomeAtribStrahler;
                nomeR = nomeAtribStrahlerR;
            } else {
                nome = nomeAtribStrahler + separador + QString::number(comp);
                nomeR = nomeAtribStrahlerR + separadorStrahlerR + QString::number(comp);
            }
            ok = false;

            do {
                regID = DBFGetFieldIndex(*DBFtabelaOrig, Utils::converterString(nome));
                regID2 = DBFGetFieldIndex(*DBFtabelaOrig, Utils::converterString(nomeR));
                if ((regID >= 0) || (regID2 >= 0)) { //O atributo já existe na tabela original.
                    ++comp;
                    nome = nomeAtribStrahler + separador + QString::number(comp);
                    nomeR = nomeAtribStrahlerR + separadorStrahlerR + QString::number(comp);
                } else { //O atributo é novo.
                    ok = true;
                }
            } while(!ok);

            //Verificando o atributo Shreve.
            if (comp == 0) {
                nome = nomeAtribShreve;
            } else {
                nome = nomeAtribShreve + separador + QString::number(comp);
            }

            ok = false;
            do {
                regID = DBFGetFieldIndex(*DBFtabelaOrig, Utils::converterString(nome));
                if (regID >= 0) { //O atributo já existe na tabela original.
                    ++comp;
                    nome = nomeAtribShreve + separador + QString::number(comp);
                } else { //O atributo é novo.
                    ok = true;
                }
            } while (!ok);

            //Verificando o atributo observação.
            if (comp == 0) {
                nome = nomeAtribObs;
            } else {
                nome = "Obs." + separador + QString::number(comp) + ":";
            }

            ok = false;
            do {
                regID = DBFGetFieldIndex(*DBFtabelaOrig, Utils::converterString(nome));
                if (regID >= 0) {
                    ++comp;
                    nome = "Obs" + separador + QString::number(comp) + ":";
                } else {
                    ok = true;
                }
            } while (!ok);

            if (comp == compGeral) {
                fim = true;
            } else {
                compGeral = comp;
            }
        } while (!fim);

        //Criando os novos atributos da nova tabela.

        //Criando o atributo fluxo.
        if (compGeral > 0) {
            nomeAtribFluxo = nomeAtribFluxo + separador + QString::number(compGeral);
        }
        DBFAddField(DBFtabela, Utils::converterString(nomeAtribFluxo), FTInteger, 1, 0);
        this->setNomeAtributoFluxo(nomeAtribFluxo);

        //Criando o atributo Strahler.
        if (params->getTipoOrdemStrahler() > 0) {
            switch (params->getTipoOrdemStrahler()) {
            case 1:
                nome = nomeAtribStrahler;
                separadorStrahlerR = separador;
                break;
            case 2:
                nome = nomeAtribStrahlerR;
                break;
            default:
                break;
            }
            if (compGeral > 0) {
                nomeAtribStrahler = nome + separadorStrahlerR + QString::number(compGeral);
            } else {
                nomeAtribStrahler = nome;
            }
            DBFAddField(DBFtabela, Utils::converterString(nomeAtribStrahler), FTInteger, 5, 0);
            this->setNomeAtributoStrahler(nomeAtribStrahler);
        }

        //Criando o atributo Shreve.
        if (params->getEOrdemShreve()) {
            if (compGeral > 0) {
                nomeAtribShreve = nomeAtribShreve + separador + QString::number(compGeral);
            }
            DBFAddField(DBFtabela, Utils::converterString(nomeAtribShreve), FTInteger, 5, 0);
            this->setNomeAtributoShreve(nomeAtribShreve);
        }

        //Criando o atributo observação.
        if (temObservacao) {
            if (compGeral > 0) {
                nomeAtribObs = "Obs" + separador + QString::number(compGeral) + ":";
            }
            DBFAddField(DBFtabela, Utils::converterString(nomeAtribObs), FTString, 80, 0);
            this->setNomeAtributoObservacao(nomeAtribObs);
        }

        // Copiando os dados da tabela original.
        int nRegistros = DBFGetRecordCount(*DBFtabelaOrig);
        if (nRegistros > 0) {
            for (int i = 0; i < nRegistros; ++i) {
                for (int j = 0; j < nCampos; ++j) {
                    if (DBFIsAttributeNULL(*DBFtabelaOrig, i, j)) {
                        DBFWriteNULLAttribute(DBFtabela, i, j);
                    } else if (listaTipos[j] == FTString) {
                        DBFWriteStringAttribute(DBFtabela, i, j, DBFReadStringAttribute(*DBFtabelaOrig, i, j));
                    } else if (listaTipos[j] == FTInteger) {
                        DBFWriteIntegerAttribute(DBFtabela, i, j, DBFReadIntegerAttribute(*DBFtabelaOrig, i, j));
                    } else if (listaTipos[j] == FTDouble){
                        DBFWriteDoubleAttribute(DBFtabela, i, j, DBFReadDoubleAttribute(*DBFtabelaOrig, i, j));
                    } else if (listaTipos[j] == FTLogical) {
                        const char valorConsChr = DBFReadLogicalAttribute(*DBFtabelaOrig, i, j)[0];
                        DBFWriteLogicalAttribute(DBFtabela, i, j, valorConsChr);
                    } else if (listaTipos[j] == FTDate) {
                        DBFWriteDateAttribute(DBFtabela, i, j, DBFReadDateAttribute(*DBFtabelaOrig, i, j));
                    }
                }
            }
        }
        delete[] listaTipos;
        fecharDados(DBFtabela);
    }
    return resultado;
}

void SHPConjuntoFeicaoDAO::gravarRegistro(Feicao *feicao, SHPHandle *SHPfigura, DBFHandle *DBFtabela, char strahler, bool shreve, Atributo *atributos, QString observacao, int nAtributos)
{

    // Montando os registros.
    int nVertices = feicao->getNVertices();
    int *panPartStart = new int[feicao->getNPartes()];
    int *panPartType = new int[feicao->getNPartes()];

    int elemId;
    QString momeCampo = this->getNomeAtributoFluxo();
    int regFluxo = DBFGetFieldIndex(*DBFtabela, Utils::converterString(momeCampo));
    momeCampo = this->getNomeAtributoStrahler();
    int regStrahler = DBFGetFieldIndex(*DBFtabela, Utils::converterString(momeCampo));
    momeCampo = this->getNomeAtributoShreve();
    int regShreve = DBFGetFieldIndex(*DBFtabela, Utils::converterString(momeCampo));
    momeCampo = this->getNomeAtributoObservacao();
    int regObservacao = DBFGetFieldIndex(*DBFtabela, Utils::converterString(momeCampo));

    panPartStart[0] = 0; // Trabalhando com parte simples ou única.
    panPartType[0] = feicao->getTipo();

    double *fX = new double[nVertices];
    double *fY = new double[nVertices];
    double *fZ = new double[nVertices]; // Não estou gravando ainda!
    double *fM = new double[nVertices]; // Não estou gravando ainda!

    if (nVertices > 0) {

        // Verificando a necessidade de inversão do fluxo do segmento.
        if (feicao->getFluxo() == 2) { //Inverter o fluxo.

            // Montando os segmentos na ordem inversa.
            int indice = 0;
            for (int j = nVertices - 1; j >= 0; --j) {
                fX[indice] = feicao->getVertice(j)->getX();
                fY[indice] = feicao->getVertice(j)->getY();
                ++indice;
            }
        } else {

            // Montando os segmentos em ordem.
            for (int j = 0; j < nVertices; ++j) {
                fX[j] = feicao->getVertice(j)->getX();
                fY[j] = feicao->getVertice(j)->getY();
            }
        }
    }

    // Escrevendo a feição no arquivo SHP.
    SHPObject *objeto = SHPCreateObject(feicao->getTipo(), feicao->getId(), 1, panPartStart, panPartType, nVertices, fX, fY, fZ, fM);
    elemId = SHPWriteObject(*SHPfigura, -1, objeto);
    SHPDestroyObject(objeto);

    // Escrevendo no arquivo DBF.
    if ((atributos !=0) && (nAtributos > 0)) { //É elemento novo!

        // Copiando os valores originais dos atributos.
        this->gravarAtributos(DBFtabela, elemId, atributos, nAtributos);
    }

    // Escrevendo o valor do fluxo.
    DBFWriteIntegerAttribute(*DBFtabela, elemId, regFluxo, feicao->getFluxo());

    if (strahler > 0) {

        // Escrevendo o valor da classificação Strahler.
        DBFWriteIntegerAttribute(*DBFtabela, elemId, regStrahler, feicao->getStrahler());
    }

    if (shreve) {

        // Escrevendo o valor da classificação Shreve.
        DBFWriteIntegerAttribute(*DBFtabela, elemId, regShreve, feicao->getShreve());
    }
    if (feicao->getTemObservacao()) {

        // Convertendo para a página de código ISO8895-1.
        char *temp = Utils::converterString(observacao);

        // Escrevendo o texto de observação.
        DBFWriteStringAttribute(*DBFtabela, elemId, regObservacao, temp);
    }

    //Apagando os ponteiros.
    if (nVertices > 0) {
        delete fX;
        delete fY;
        delete fZ;
        delete fM;
    }
    delete panPartStart;
    delete panPartType;
}

void SHPConjuntoFeicaoDAO:: gravarAtributos(DBFHandle *DBFtabela, int idFeicao, Atributo *atributos, int nAtributos)
{
    for (int x = 0; x < nAtributos; ++x) {
        int indice = DBFGetFieldIndex(*DBFtabela, atributos[x].getNome().c_str());
        if (indice >= 0) {
            if (atributos[x].getTipo() == 0) { // Nulo
                DBFWriteNULLAttribute(*DBFtabela, idFeicao, indice);
            } else if (atributos[x].getTipo() == 1) { // FTString
                DBFWriteStringAttribute(*DBFtabela, idFeicao, indice, atributos[x].getValor().c_str());
            } else if (atributos[x].getTipo() == 2){ // FTInteger
                DBFWriteIntegerAttribute(*DBFtabela, idFeicao, indice, atoi(atributos[x].getValor().c_str()));
            } else if (atributos[x].getTipo() == 3) { // FTDouble
                DBFWriteDoubleAttribute(*DBFtabela, idFeicao, indice, atof(atributos[x].getValor().c_str()));
            } else if (atributos[x].getTipo() == 4) { // FTLogical
                const char valorConsChr = atributos[x].getValor()[0];
                DBFWriteLogicalAttribute(*DBFtabela, idFeicao, indice, valorConsChr);
            } else if (atributos[x].getTipo() == 5) { // FTDate
                DBFWriteDateAttribute(*DBFtabela, idFeicao, indice, atoi(atributos[x].getValor().c_str()));
            }
        }
    }
}

void SHPConjuntoFeicaoDAO::copiarArquivosConfiguracao(QString nomeArquivo, QString nomeNovoArquivo)
{
    QFileInfo info(nomeArquivo);
    QFileInfo infoNovo(nomeNovoArquivo);

    QString path = info.absolutePath(); //Caminho completo sem o nome do arquivo.
    QString nome = info.fileName(); //Nome do arquivo com o tipo.
    QString base = info.baseName(); //Nome do arquivo sem o tipo.

    QString pathNovo = infoNovo.absolutePath();
    QString nomeNovo = infoNovo.fileName();
    QString baseNovo = infoNovo.baseName();

    QString nomeProjeto = path + "/" + base + ".prj";
    QString novoNomeProjeto = pathNovo + "/" + baseNovo + ".prj";
    QString nomeConf = path + "/" + nome + ".xml";
    QString novoNomeConf = pathNovo + "/" + nomeNovo + ".xml";

    //Copiando o arquivo .prj.
    if (QFileInfo::exists(nomeProjeto)) {
        if (QFileInfo::exists(novoNomeProjeto)) {
            QFile::remove(novoNomeProjeto);
        }
        QFile::copy(nomeProjeto, novoNomeProjeto);
    }

    //Copiando o arquivo .xml.
    if (QFileInfo::exists(nomeConf)) {
        if (QFileInfo::exists(novoNomeConf)) {
            QFile::remove(novoNomeConf);
        }
        QFile::copy(nomeConf, novoNomeConf);
    }
}

void SHPConjuntoFeicaoDAO::gravarConjuntoFeicao(ConjuntoFeicao *figura, Parametros *params)
{

    // Abrindo a tabela original.
    DBFHandle DBFtabelaOrig = abrirDados(params->getNomeArquivoBacia(), LEITURA);

    // Criando os novos arquivos.
    if (DBFtabelaOrig) {

        SHPHandle SHPfigura = criarConjuntoFeicao(params->getNomeNovoArquivo(), figura->getCodigoTipo());
        bool criouNovaTabela = criarNovaTabela(params->getNomeNovoArquivo(), &DBFtabelaOrig, params, figura->getCriarObservacao());

        if (SHPfigura && criouNovaTabela) {
            fecharConjuntoFeicao(SHPfigura);

            // Abrindo os novos arquivos.
            SHPfigura = abrirConjuntoFeicao(params->getNomeNovoArquivo(), ESCRITA);
            DBFHandle DBFtabela = abrirDados(params->getNomeNovoArquivo(), ESCRITA);
            if (SHPfigura) {
                if (DBFtabela) {
                    Feicao *feicao = 0;
                    Atributo *atributos = 0;
                    Observacao *observacao = figura->getObservacoes();

                    //Gravando os registros já existentes no shapefile original.
                    for (int i = 0; i < figura->getQuantidadeFeicoes(); ++i) {
                        feicao = figura->getFeicao(i);
                        //this->gravarRegistro(feicao, &SHPfigura, &DBFtabela, params->getEOrdemStrahler(), params->getEOrdemShreve(), atributos, observacao->getObservacao(feicao->getId()), 0);
                        this->gravarRegistro(feicao, &SHPfigura, &DBFtabela, params->getTipoOrdemStrahler(), params->getEOrdemShreve(), atributos, observacao->getObservacao(feicao->getId()), 0);
                    }

                    //Gravando os novos registros criados.
                    for (int j = 0; j < figura->getQuantidadeFeicoesNovas(); ++j) {
                        feicao = figura->getFeicaoNova(j);
                        atributos = figura->getAtributosFeicaoNova(feicao->getId());
                        //this->gravarRegistro(feicao, &SHPfigura, &DBFtabela, params->getEOrdemStrahler(), params->getEOrdemShreve(), atributos, observacao->getObservacao(feicao->getId()), figura->getQuantidadeAtributos());
                        this->gravarRegistro(feicao, &SHPfigura, &DBFtabela, params->getTipoOrdemStrahler(), params->getEOrdemShreve(), atributos, observacao->getObservacao(feicao->getId()), figura->getQuantidadeAtributos());
                    }
                    fecharDados(DBFtabela);
                }
                fecharConjuntoFeicao(SHPfigura);

                //Copiando os arquivos de configuração.
                copiarArquivosConfiguracao(params->getNomeArquivoBacia(), params->getNomeNovoArquivo());
            }
        }
        fecharDados(DBFtabelaOrig);
    }
}

QString SHPConjuntoFeicaoDAO::getNomeAtributoFluxo()
{
    return this->nomeAtributoFluxo;
}

void SHPConjuntoFeicaoDAO::setNomeAtributoFluxo(QString nome)
{
    this->nomeAtributoFluxo = nome;
}

QString SHPConjuntoFeicaoDAO::getNomeAtributoStrahler()
{
    return this->nomeAtributoStrahler;
}

void SHPConjuntoFeicaoDAO::setNomeAtributoStrahler(QString nome)
{
    this->nomeAtributoStrahler = nome;
}

QString SHPConjuntoFeicaoDAO::getNomeAtributoShreve()
{
    return this->nomeAtributoShreve;
}

void SHPConjuntoFeicaoDAO::setNomeAtributoShreve(QString nome)
{
    this->nomeAtributoShreve = nome;
}

QString SHPConjuntoFeicaoDAO::getNomeAtributoObservacao()
{
    return this->nomeAtributoObservacao;
}

void SHPConjuntoFeicaoDAO::setNomeAtributoObservacao(QString nome)
{
    this->nomeAtributoObservacao = nome;
}
