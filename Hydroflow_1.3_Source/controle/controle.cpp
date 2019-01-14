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

#include "controle.h"
#include <QObject>
#include <QMessageBox>
#include <QFileDialog>
#include "utils/mensagem.h"
#include "modelo/hydroflow.h"
#include "modelo/shpconjuntofeicaodao.h"
#include "visao/frmlog.h"

Controle::Controle()
{

}

bool Controle::validarArquivo(QString arquivo)
{
    bool resultado = false;
    SHPConjuntoFeicaoDAO *dao = new SHPConjuntoFeicaoDAO();
    SHPHandle shp = dao->abrirConjuntoFeicao(arquivo, LEITURA);
    if (shp) {
        resultado = true;
        dao->fecharConjuntoFeicao(shp);
    }
    delete dao;
    return resultado;
}

int Controle::classificarBacia(Parametros *params)
{
    //throw 1;

    int resultado;
    this->parametros = params;
    Mensagem *log = new Mensagem(params);

    // Lendo os arquivos de dados.
    params->getOrigem()->setCursor(Qt::WaitCursor);
    SHPConjuntoFeicaoDAO *dao = new SHPConjuntoFeicaoDAO(params->getToleranciaXY());

    ConjuntoFeicao *bacia = dao->lerConjuntoFeicao(params->getNomeArquivoBacia(), 0);
    ConjuntoFeicao *limite = dao->lerConjuntoFeicao(params->getNomeArquivoLimite(), 1);

    if (bacia == 0) {
        resultado = 2;
        params->getOrigem()->setCursor(Qt::ArrowCursor);
    } else {
        if (limite == 0) {
            resultado = 3;
            params->getOrigem()->setCursor(Qt::ArrowCursor);
        } else {

            // Classificando a bacia.
            Hydroflow *hi = new Hydroflow(bacia, limite, params, log);

            /* Códigos de erro:
            0 - Processamento concluído com sucesso!
            1 - Processamento concluído com alertas!
            2 - Foz da bacia hidrográfica não identificada!
            3 - Foi identificada mais de uma foz na bacia hidrográfica! (listado no log)
            4 - Foi identificado uma feição com mais de dois afluentes! (listado no log)
            5 - Relações topológicas inesperadas! (listado no log)
            */
            resultado = hi->classificarBacia();
            params->getOrigem()->setCursor(Qt::ArrowCursor);
            exibirMensagem(resultado);

            // Salvando o novo arquivo.
            if ((resultado == 0) || (resultado == 1)) {

                // Obtendo nome do novo arquivo.
                QString novo = QFileDialog::getSaveFileName(params->getOrigem(), QObject::tr("Salvar bacia classificada como"), "", "Shape File(*.shp)");
                if (novo != "") {
                    params->getOrigem()->setCursor(Qt::WaitCursor);
                    params->setNomeNovoArquivo(novo);
                    log->setResultado(novo);

                    // Gravando os arquivos.
                    dao->gravarConjuntoFeicao(bacia, params);
                    params->getOrigem()->setCursor(Qt::ArrowCursor);
                }
                resultado = 0;
            } else {
                resultado = 9;
            }

            // Verificando se há mensagens no log.
            if (log->haMensagem()) {
                FrmLog *formLog = new FrmLog(params->getOrigem(), log);
                formLog->exibirLog();
                delete formLog;
            }

            // Apagando os objetos.
            hi->limparHydroflow();
            delete hi;
            hi = 0;
            limite->LimparConjuntoFeicao();
            delete limite;
            limite = 0;
            bacia->LimparConjuntoFeicao();
            delete bacia;
            bacia = 0;
            delete dao;
            dao = 0;
            delete log;
            log = 0;
        }
    }

    return resultado;
}

/* Códigos:
0 - Processamento concluído com sucesso!
1 - Processamento concluído com alertas!
2 - Foz da bacia hidrográfica não identificada!
*/
void Controle::exibirMensagem(int codigo)
{
    switch (codigo) {
    case 0:
        QMessageBox::about(parametros->getOrigem(), QObject::tr("Aviso"), QObject::tr("Processamento concluído com sucesso!"));
        break;
    case 1:
        QMessageBox::about(parametros->getOrigem(), QObject::tr("Aviso"), QObject::tr("Processamento concluído com alertas - leia o log para detalhes!"));
        break;
    case 2:
        QMessageBox::warning(parametros->getOrigem(), QObject::tr("Atenção"), QObject::tr("Exutório da bacia hidrográfica não identificado ou não conectado corretamente à rede de drenagem!"));
        break;
    default: //Houve erro listado no log!
        QMessageBox::warning(parametros->getOrigem(), QObject::tr("Atenção"), QObject::tr("Processamento não concluído - leia o log para detalhes!"));
        break;
    }
}
