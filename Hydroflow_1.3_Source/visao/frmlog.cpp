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

#include "frmlog.h"
#include "ui_frmlog.h"
#include <QFile>
#include <QFileDialog>
#include "QMessageBox"
#include <QString>
#include <QTextStream>
#include "visao/logerro.h"

FrmLog::FrmLog(QWidget *origem, Mensagem *mensagem) :
    QDialog(origem),
    ui(new Ui::FrmLog)
{
    ui->setupUi(this);
    this->origem = origem;
    this->mensagem = mensagem;
}

FrmLog::~FrmLog()
{
    delete ui;
}

void FrmLog::exibirLog()
{
    this->setCursor(Qt::WaitCursor);

    processa = new QThread();
    log = new Logerro();
    log->setMensagem(mensagem);
    log->moveToThread(processa);

    //Conectando os sinais.
    connect(processa, SIGNAL(started()), log, SLOT(listarLogErro()));
    connect(log, SIGNAL(listarMenagem(QString)), this, SLOT(listar(QString)));
    connect(log, SIGNAL(fimMensagem()), this, SLOT(logParado()));
    connect(this, SIGNAL(finished(int)), log, SLOT(pararLogErro()));

    //Executando a thread para listar mensagens.
    processa->start();

    // Abrindo o formulário.
    ui->pushButton->setEnabled(false);
    this->move((origem->x() + 10), (origem->y() + 32));
    this->exec();


    for (int cont = 0; cont < 10; ++cont) {
        if (processa->isRunning()) {
            log->pararLogErro();
            QThread::currentThread()->msleep(10);
        } else {
            delete processa;
            delete log;
            break;
        }
    }
}

// Botão fechar.
void FrmLog::on_pushButton_2_clicked()
{
    this->close();
}

// Botão Salvar.
void FrmLog::on_pushButton_clicked()
{
    QString nomeArquivo = QFileDialog::getSaveFileName(this, tr("Salvar arquivo como"), "", "(*.txt)");
    QFile arquivo(nomeArquivo);
    if (arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream linha(&arquivo);
        linha << mensagem->getCabecalho() << endl;
        for (int i = 0; i < mensagem->getTamanho(); ++i) {
            linha << mensagem->getMensagem(i) << endl;
        }
        linha << mensagem->getRodape() << endl;
        arquivo.close();
    }
}

void FrmLog::listar(QString texto)
{
    ui->textEdit->append(texto);
}

void FrmLog::logParado()
{
    this->setCursor(Qt::ArrowCursor);
    ui->pushButton->setEnabled(true);
}


