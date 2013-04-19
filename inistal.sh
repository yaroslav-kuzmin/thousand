#!/bin/bash
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
#   Email contact: esdt@mail.ru                                               #
#                                                                             #
###############################################################################
DIR_ROOT=.thousand
cd $HOME
mkdir $DIR_ROOT 2>&- 
CHECK=$?
if [ $CHECK -eq 1 ]
then
	printf "Немогу создать каталог %s \n" $DIR_ROOT
	exit 0;
fi
printf "Создан рабочий каталог\n"
###############################################################################
cd $DIR_ROOT
exec 10> ini
cat >&10 << _ANYEOF
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
#   Email contact: esdt@mail.ru                                               #
#                                                                             #
###############################################################################

# Глобальные настройки
[global]

#Настройки сервера
[server]

#Настройки клиента
[client]

#Настройки робота
[robot]

_ANYEOF
printf "Создан файл ini\n"
###############################################################################
DIR_LOG=log

mkdir $DIR_LOG 2>&- 
CHECK=$?
if [ $CHECK -eq 1 ]
then
	printf "Немогу создать каталог %s \n" $DIR_LOG
	exit 0;
fi
printf "Создан каталог log\n"
###############################################################################
exit 0
###############################################################################
