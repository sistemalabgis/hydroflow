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

#ifndef FRMLOG_H
#define FRMLOG_H

#include <QDialog>
#include "utils/mensagem.h"
#include <QString>
#include <QTextEdit>
#include <QThread>
#include "visao/logerro.h"

namespace Ui {
class FrmLog;
}

class FrmLog : public QDialog
{
    Q_OBJECT

public:
    FrmLog(QWidget *origem, Mensagem *mensagem);
    ~FrmLog();
    void exibirLog();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void logParado();

public slots:
    void listar(QString texto);

private:
    Ui::FrmLog *ui;
    QWidget *origem;
    Mensagem *mensagem;
    QThread *processa;
    Logerro *log;

};

#endif // FRMLOG_H
