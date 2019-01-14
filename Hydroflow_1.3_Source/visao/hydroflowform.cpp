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

#include "hydroflowform.h"
#include "ui_HydroFlow.h"
#include "QFileDialog"
#include "QMessageBox"
#include "controle/controle.h"
#include "modelo/parametros.h"

#include "utils/mensagem.h"
#include "QDebug"

HydroFlowForm::HydroFlowForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HydroFlowForm)
{
    ui->setupUi(this);

    tradutor.load("idioma_en.qm", qApp->applicationDirPath());
    ui->toolButton_Idioma_pt->setEnabled(false);
    ui->toolButton_Idioma_en->setEnabled(true);
}

HydroFlowForm::~HydroFlowForm()
{
    delete ui;
}

QString HydroFlowForm::getPath(QString titulo)
{
   QString fileName = QFileDialog::getOpenFileName(this, titulo,"", "Shape File(*.shp)");
   return fileName;
}

bool HydroFlowForm::validarNomeArquivos()
{
    bool resultado = false;
    if (!ui->lineEdit_HidLn->text().isEmpty() && !ui->lineEdit_Lim->text().isEmpty()) {
        if (ui->lineEdit_HidLn->text() != ui->lineEdit_Lim->text()) {
            ui->pushButton_Exec->setEnabled(true);
            resultado = true;
        } else {
            this->exibirMensagem(1);
            ui->pushButton_Exec->setEnabled(false);
        }
    } else {
        ui->pushButton_Exec->setEnabled(false);
    }
    return resultado;
}

//Botão de seleção do arquivo a bacia de drenagem.
void HydroFlowForm::on_pushButton_HidLn_clicked()
{
    ui->lineEdit_HidLn->setText(getPath(tr("Arquivo da rede de drenagem")));
    validarNomeArquivos();
}

//Botão de seleção do arquivo de limite da bacia de drenagem.
void HydroFlowForm::on_pushButton_Lim_clicked()
{
    ui->lineEdit_Lim->setText(getPath(tr("Arquivo do limite da área")));
    validarNomeArquivos();
}

//Seleção do ComboBox.
void HydroFlowForm::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->lineEdit_TolXY->setText("0.001");
        break;
    case 1:
        ui->lineEdit_TolXY->setText("0.000001");
        break;
    case 2:
        ui->lineEdit_TolXY->setText("0.03937");
        break;
    case 3:
        ui->lineEdit_TolXY->setText("0.001");
        break;
    };
}

//CheckBox da classificação Fluxo.
void HydroFlowForm::on_checkBox_FlowOnly_stateChanged(int CBState)
{
    if (CBState == Qt::Checked) {
        ui->checkBox_Strahler->setChecked(false);
        ui->checkBox_Strahler->setEnabled(false);
        ui->checkBox_Shreve->setChecked(false);
        ui->checkBox_Shreve->setEnabled(false);
    } else {
        ui->checkBox_Strahler->setEnabled(true);
        ui->checkBox_Strahler->setChecked(true);
        ui->checkBox_Shreve->setEnabled(true);
        ui->checkBox_Shreve->setChecked(true);
    }
}

//Botão executar.
void HydroFlowForm::on_pushButton_Exec_clicked()
{
    try {

        //Validando arquivos.
        Controle *con = new Controle();
        QString nomeBacia = ui->lineEdit_HidLn->text();
        if (!con->validarArquivo(nomeBacia)) {
            this->exibirMensagem(2);
        } else {
            QString nomeLimite = ui->lineEdit_Lim->text();
            if (!con->validarArquivo(nomeLimite)) {
                this->exibirMensagem(3);
            } else {

                // Validando a tolerância.
                if (ui->lineEdit_TolXY->text().isEmpty()) {
                        ui->lineEdit_TolXY->setText("0");
                }
                double tol = ui->lineEdit_TolXY->text().toDouble();
                if (tol < 0) {
                    this->exibirMensagem(4);
                } else {

                    //Determinando o tipo de classificação Strahler.
                    char tipoClassificacaoStrahler = 0;
                    /*
                     * A opção de classificação Strahler relaxada não está disponível
                     * no formulário (tipoClassificacaoStrahler = 2).
                     *
                     */
                    if (ui->checkBox_Strahler->isChecked()) {
                        tipoClassificacaoStrahler = 1;
                    } else {
                        tipoClassificacaoStrahler = 0;
                    }

                    Parametros *params = new Parametros(nomeBacia, nomeLimite, tol, tipoClassificacaoStrahler, ui->checkBox_Shreve->isChecked(), this);

                    // Iniciando o processo.
                    int resultado = con->classificarBacia(params);
                    this->exibirMensagem(resultado); //Valores para resultado: 0, 2, 3 ou 9.
                    delete params;
                }
            }
        }
        delete con;
    } catch (...) {
        this->exibirMensagem(5);
    }
}

/* Códigos:
0 - SEM MENSAGEM.
1 - Os dois arquivos de entrada são iguais!
2 - Arquivo da rede de drenagem inválido ou não pode ser acessado!
3 - Arquivo do limite da bacia/exutório inválido ou não pode ser acessado!
4 - O valor da Tolerância XY não pode ser negativo!
*/
void HydroFlowForm::exibirMensagem(int codigo)
{
    switch (codigo) {
    case 1:
        QMessageBox::warning(this, tr("Atenção"), tr("Os dois arquivos de entrada são iguais!"));
        break;
    case 2:
        QMessageBox::warning(this, tr("Atenção"), tr("Arquivo da rede de drenagem inválido ou não pode ser acessado!"));
        break;
    case 3:
        QMessageBox::warning(this, tr("Atenção"), tr("Arquivo do limite da bacia/exutório inválido ou não pode ser acessado!"));
        break;
    case 4:
        QMessageBox::warning(this, tr("Atenção"), tr("O valor da Tolerância XY não pode ser negativo!"));
        break;
    case 5:
        QMessageBox::warning(this, tr("Atenção"), tr("Houve um erro inesperado. O processamento foi interrompido!"));
        break;
    default:
        break;
    }
}

void HydroFlowForm::avaliarInferencia()
{
    if ((!ui->checkBox_Strahler->isChecked()) && (!ui->checkBox_Shreve->isChecked())) {
        ui->checkBox_FlowOnly->setChecked(true);
    }
}

//Botão fechar.
void HydroFlowForm::on_pushButton_Close_clicked()
{
    qApp->exit(0);
}

//Botão de troca de idioma para português.
void HydroFlowForm::on_toolButton_Idioma_pt_clicked()
{

    qApp->removeTranslator(&tradutor);
    ui->retranslateUi(this);

    ui->toolButton_Idioma_pt->setEnabled(false);
    ui->toolButton_Idioma_en->setEnabled(true);
}

//Botão de troca de idioma para inglês.
void HydroFlowForm::on_toolButton_Idioma_en_clicked()
{
    qApp->installTranslator(&tradutor);
    ui->retranslateUi(this);

    ui->toolButton_Idioma_pt->setEnabled(true);
    ui->toolButton_Idioma_en->setEnabled(false);
}

void HydroFlowForm::on_checkBox_Strahler_clicked()
{
    avaliarInferencia();
}

void HydroFlowForm::on_checkBox_Shreve_clicked()
{
    if (!ui->checkBox_Shreve->isChecked()) {
        avaliarInferencia();
    }
}
