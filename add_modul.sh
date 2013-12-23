#!/bin/bash

################################ VARIABLE #####################################
HEADER='/*****************************************************************************/
/*                                                                           */
/*Thousand - card game                                                       */
/* Copyright (C) <2013> <Kuzmin Yaroslav>                                    */
/*                                                                           */
/* This program is free software: you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, either version 3 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.     */
/*                                                                           */
/*  Email contact: kuzmin.yaroslav@gmail.com                                 */
/*                                                                           */
/*****************************************************************************/
'
DIR=$1
MODUL=$2
MODUL_C=${MODUL}.c
MODUL_H=${MODUL}.h
################################ FUNCTION #####################################

################################## MAIN #######################################

if [ $# -ne 2 ]
	then
		printf "Нет названия каталога и названия модуля!\n"
		exit 0
	fi

cd $DIR
CHECK=$?
if [ $CHECK -eq 1 ]
then
	printf "Нет такого каталога %s \n" $DIR
	exit 0;
fi
printf "Перешли в рабочий каталог %s\n" ${DIR}

##############

exec 10> ${MODUL_C}
cat >&10 << _ANYEOF
${HEADER}
/*****************************************************************************/
/* Модуль ${MODUL}                                */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/

#include <glib.h>

#include "pub.h"

/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/

/*****************************************************************************/
_ANYEOF
printf "Создан файл %s\n" ${MODUL_C}

##############

MODUL_NAME=`printf "%s" ${MODUL} | tr 'a-z' 'A-Z'`
exec 11> ${MODUL_H}
cat >&11 << _ANYEOF
${HEADER}
/*****************************************************************************/
/* Модуль ${MODUL}                                */
/*****************************************************************************/

#ifndef ${MODUL_NAME}_H
#define ${MODUL_NAME}_H

#endif

/*****************************************************************************/
_ANYEOF
printf "Создан файл %s\n" ${MODUL_H}

exit 0
###############################################################################
