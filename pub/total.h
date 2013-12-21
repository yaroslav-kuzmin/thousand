/*****************************************************************************/
/*                                                                           */
/* Thousand - card game                                                      */
/*  Copyright (C) <2013> <Kuzmin Yaroslav>                                   */
/*                                                                           */
/*  This program is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 3 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  This program is distributed in the hope that it will be useful,          */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                           */
/*   Email contact: kuzmin.yaroslav@gmail.com                                */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Объявлене глобальных макросов                                             */
/*****************************************************************************/

#ifndef TOTAL_H
#define TOTAL_H

extern char SERVER[];
extern char CLIENT[];
extern char ROBOT[];
extern char DEFAULT[];

extern char LOCALE[];

extern char INI_GLOBAL_GROUP[];
extern char INI_SERVER_GROUP[];
extern char INI_CLIENT_GROUP[];
extern char INI_ROBOT_GROUP[];
extern char ACCESS_ROBOT_GROUP[];
extern char ACCESS_USER_GROUP[];

int get_registration_operation(void);
void set_registration_operation(int rg);

char * get_user_name(void);
char * get_home_catalog(void);
char * get_ini_file(void);
char * get_access_file(void);
char * get_log_catalog(void);
char * get_local_socket(void);
int total_check(void);

#endif
/*****************************************************************************/
