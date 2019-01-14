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

#include "logerro.h"
#include <QObject>
#include <QThread>

Logerro::Logerro()
{

}

Logerro::~Logerro()
{

}

void Logerro::setMensagem(Mensagem *valor)
{
    mensagem = valor;
}

void Logerro::listarLogErro()
{
    // Imprimindo o cabeçalho;
    emit listarMenagem(mensagem->getCabecalho());

    // Imprimindo as mensagens.
    parar = false;
    QThread::currentThread()->msleep(1);
    for (int i = 0; i < mensagem->getTamanho(); ++i) {
        if (!parar) {
            emit listarMenagem(mensagem->getMensagem(i));
        } else {
            break;
        }
    }

    // Imprimindo o rodapé.
    if (!parar) {
        emit listarMenagem(mensagem->getRodape());
    }

    //Finalizado a thread;
    emit fimMensagem();
    emit thread()->exit(0);
}

void Logerro::pararLogErro()
{
    parar = true;
}

