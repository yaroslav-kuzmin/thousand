#!/bin/bash

################################ VARIABLE #####################################
HEADER='
###############################################################################
#                                                                             #
# Thousand - card game                                                        #
#  Copyright (C) <2013> <Kuzmin Yaroslav>                                     #
#                                                                             #
#  This program is free software: you can redistribute it and/or modify       #
#  it under the terms of the GNU General Public License as published by       #
#  the Free Software Foundation, either version 3 of the License, or          #
#  (at your option) any later version.                                        #
#                                                                             #
#  This program is distributed in the hope that it will be useful,            #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#  GNU General Public License for more details.                               #
#                                                                             #
#  You should have received a copy of the GNU General Public License          #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.      #
#                                                                             #
#   Email contact: kuzmin.yaroslav@gmail.com                                  #
#                                                                             #
###############################################################################
'
DIR_ROOT=.thousand
FILE_INI=ini
DIR_LOG=log
DIR_DATABASE=database
FILE_ACCESS=access

################################ FUNCTION #####################################

################################## MAIN ######################################

cd $HOME
mkdir $DIR_ROOT 2>&- 
CHECK=$?
if [ $CHECK -eq 1 ]
then
	printf "Немогу создать каталог %s \n" $DIR_ROOT
	exit 0;
fi
printf "Создан рабочий каталог\n"

##############

cd $DIR_ROOT
exec 10> ${FILE_INI}
cat >&10 << _ANYEOF
${HEADER}
# Глобальные настройки
[global]

#Настройки сервера
[server]
robot=/usr/local/games/thr

#Настройки клиента
[client]

#Настройки робота
[robot]

_ANYEOF
printf "Создан файл ini\n"

##############

mkdir $DIR_LOG 2>&- 
CHECK=$?
if [ $CHECK -eq 1 ]
then
	printf "Немогу создать каталог %s \n" $DIR_LOG
	exit 0;
fi
printf "Создан каталог %s\n" $DIR_LOG

##############

mkdir $DIR_DATABASE 2>&- 
CHECK=$?
if [ $CHECK -eq 1 ]
then
	printf "Немогу создать каталог %s \n" $DIR_DATABASE
	exit 0;
fi
printf "Создан каталог %s\n" $DIR_DATABASE

cd ${DIR_DATABASE}
exec 11> ${FILE_ACCESS} 
cat >&11 << _ANYEOF
${HEADER}
#Доступ для роботов
[robot]
robot001=00000000000000000000000000000000
robot002=00000000000000000000000000000000
robot003=00000000000000000000000000000000

#доступ для игроков
[user]
admin=00000000000000000000000000000000

_ANYEOF
printf "Создан файл доступа %s\n" ${FILE_ACCESS}

cd ..

exit 0
###############################################################################
