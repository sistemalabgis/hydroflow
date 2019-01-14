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

#ifndef HYDROFLOWFORM_H
#define HYDROFLOWFORM_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
class HydroFlowForm;
}

class HydroFlowForm : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit HydroFlowForm(QWidget *parent = 0);
    ~HydroFlowForm();
    
private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_HidLn_clicked();
    void on_pushButton_Lim_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_checkBox_FlowOnly_stateChanged(int CBState);
    void on_pushButton_Exec_clicked();
    void on_toolButton_Idioma_pt_clicked();
    void on_toolButton_Idioma_en_clicked();
    void on_checkBox_Strahler_clicked();
    void on_checkBox_Shreve_clicked();

private:
    Ui::HydroFlowForm *ui;

    QTranslator tradutor;

    QString getPath(QString titulo);
    bool validarNomeArquivos();
    void exibirMensagem(int codigo);
    void avaliarInferencia();
};

#endif // HYDROFLOWFORM_H
