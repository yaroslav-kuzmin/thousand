/*****************************************************************************/
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

/*****************************************************************************/
/* Модуль пользовательского интерфейса                                       */
/*****************************************************************************/

#ifndef INTERFACE_H
#define INTERFACE_H

int if_get_name_user(char ** user,int len);
int if_set_name_user(char *user);
int if_get_passwd(char ** passwd,int len);
int if_set_passwd(char * passwd);
int if_set_connect(void);

interface_cmd_e if_cmd(void);

int init_interface(void);
int close_interface(void);

#endif 

/*****************************************************************************/
