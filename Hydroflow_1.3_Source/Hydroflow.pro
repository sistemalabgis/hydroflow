#   Copyright (C) 2015 Sistema Labgis of Instituto Multidisciplinar de
#   Formação Humana com Tecnologias of Universidade do Estado do Rio de Janeiro,
#   Alexandre Monteiro Reis and José Augusto Sapienza Ramos.
#
#   This file is part of HydroFlow.
#
#   HydroFlow is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#-------------------------------------------------
# Hydroflow 1.3 2018-08-17
# Project created by QtCreator 2013-06-10T12:58:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hydroflow
TEMPLATE = app

SOURCES += main.cpp\
        visao/hydroflowform.cpp \
        controle/controle.cpp \
        shapelib/dbfopen.c \
        shapelib/shpopen.c \
        modelo/parametros.cpp \
        visao/frmlog.cpp \
        utils/utils.cpp \
        modelo/hydroflow.cpp \
        modelo/conjuntofeicao.cpp \
        modelo/segmento.cpp \
        modelo/pontovarredura.cpp \
        modelo/shpconjuntofeicaodao.cpp \
        utils/modulo.cpp \
        modelo/atributo.cpp \
        modelo/atributofeicaonova.cpp \
        modelo/feicao.cpp \
        modelo/indiceitem.cpp \
        modelo/no.cpp \
        modelo/observacao.cpp \
        modelo/posicao.cpp \
        modelo/relacao.cpp \
        modelo/relacaoitem.cpp \
        modelo/varredura.cpp \
        modelo/varredura_item.cpp \
        modelo/vertice.cpp \
        utils/geometria.cpp \
        utils/mensagem.cpp \
        visao/logerro.cpp

HEADERS  += visao/hydroflowform.h \
            controle/controle.h \
            shapelib/shapefil.h \
            modelo/parametros.h \
            visao/frmlog.h \
            utils/utils.h \
            modelo/hydroflow.h \
            modelo/conjuntofeicao.h \
            modelo/segmento.h \
            modelo/pontovarredura.h \
            modelo/shpconjuntofeicaodao.h \
            utils/modulo.h \
            modelo/atributo.h \
            modelo/atributofeicaonova.h \
            modelo/feicao.h \
            modelo/indiceitem.h \
            modelo/no.h \
            modelo/observacao.h \
            modelo/posicao.h \
            modelo/relacao.h \
            modelo/relacaoitem.h \
            modelo/varredura.h \
            modelo/varredura_item.h \
            modelo/vertice.h \
            utils/geometria.h \
            utils/mensagem.h \
            visao/logerro.h

FORMS    += visao/HydroFlow.ui \
            visao/frmlog.ui

TRANSLATIONS += idioma_en.ts
RESOURCES += hydroflow.qrc

# Definindo o ícone da aplicação para a plataforma Windows.
RC_ICONS = hydroflow.ico
