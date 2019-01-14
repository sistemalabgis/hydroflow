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

#ifndef SHPCONJUNTOFEICAODAO_H
#define SHPCONJUNTOFEICAODAO_H

#include <QString>
#include "utils/mensagem.h"
#include "feicao.h"
#include "conjuntofeicao.h"
#include "shapelib/shapefil.h"
#include "parametros.h"
#include "atributo.h"

using namespace std;

#define LEITURA 0
#define ESCRITA 1

class SHPConjuntoFeicaoDAO
{
private:
    double tolerancia;
    QString nomeAtributoFluxo;
    QString nomeAtributoStrahler;
    QString nomeAtributoShreve;
    QString nomeAtributoObservacao;

    SHPHandle criarConjuntoFeicao(QString nomeArquivo, int tipo);
    DBFHandle abrirDados(QString nomeArquivo, int modo); //0-leitura; 1-leitura e gravação.)
    DBFHandle criarDados(QString nomeArquivo);
    void fecharDados(DBFHandle handle);
    bool criarNovaTabela(QString nomeArquivo, DBFHandle *DBFtabelaOrig, Parametros *params, bool temObservacao);
    void gravarRegistro(Feicao *feicoes, SHPHandle *SHPfigura, DBFHandle *DBFtabela, char strahler, bool shreve, Atributo *atributos, QString observacao, int nAtributos);
    void gravarAtributos(DBFHandle *DBFtabela, int idFeicao, Atributo *atributos, int nAtributos);
    void copiarArquivosConfiguracao(QString nomeArquivo, QString nomeNovoArquivo);

public:
    SHPConjuntoFeicaoDAO();
    SHPConjuntoFeicaoDAO(double tolerancia);

    ConjuntoFeicao* lerConjuntoFeicao(QString nomeArquivo, int tipoFigura); // Le dados de shapefile.
    void gravarConjuntoFeicao(ConjuntoFeicao *figura, Parametros *params);
    Atributo* lerAtributos(DBFHandle DBFtabela, int idFeicao, int &nAtributos);
    SHPHandle abrirConjuntoFeicao(QString nomeArquivo, int modo); //0-leitura; 1-leitura e gravação.
    void fecharConjuntoFeicao(SHPHandle handle);
    QString getNomeAtributoFluxo();
    void setNomeAtributoFluxo(QString nome);
    QString getNomeAtributoStrahler();
    void setNomeAtributoStrahler(QString nome);
    QString getNomeAtributoShreve();
    void setNomeAtributoShreve(QString nome);
    QString getNomeAtributoObservacao();
    void setNomeAtributoObservacao(QString nome);
};

#endif // SHPCONJUNTOFEICAODAO_H

