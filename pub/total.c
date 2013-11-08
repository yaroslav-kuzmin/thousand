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
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pub.h"
#include "alloc.h"
#include "log.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/
char SERVER[] = "server";
char CLIENT[] = "client";
char ROBOT[] = "robot";
char DEFAULT[] = "_";

char LOCALE[] = "ru_RU.UTF-8";

static int registration_operation = YES;

char INI_GLOBAL_GROUP[] = "global";
char INI_SERVER_GROUP[] = "server";
char INI_CLIENT_GROUP[] = "client";
char INI_ROBOT_GROUP[] = "robot"; 
char ACCESS_ROBOT_GROUP[] = "robot"; 
char ACCESS_USER_GROUP[] = "user";
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

int get_registration_operation(void)
{
	return registration_operation;
}
void set_registration_operation(int rg)
{
	if(rg != NO){
		registration_operation = YES;
	}
	else{
		registration_operation = NO;
	}
}
/*****************************************************************************/
static char * user_name = NULL;
static char * home_user = NULL; 

static char * home_catalog = NULL;
static const char * HOME_CATALOG = "/.thousand/";
#define SIZE_STR_HOME_CATAOLG    12

static const char * USER_NAME ="USER";
static const char * HOME_USER ="HOME";

static char * ini_file = NULL;
static const char * INI = "ini";
#define SIZE_STR_INI   4

static char * log_catalog = NULL;
static const char * LOG_CATALOG = "log/";
#define SIZE_STR_LOG_CATALOG 5

static char * local_socket = NULL;
static const char * LOCAL_SOCKET = "socket";
#define SIZE_STR_LOCAL_SOCKET  7

static char * access_file = NULL;
static const char * ACCESS = "database/access";
#define SIZE_STR_ACCESS 16

char * get_user_name(void)
{
	return user_name;
}
char * get_home_catalog(void)
{
	return home_catalog;
}
char * get_ini_file(void)
{
	return ini_file;
}
char * get_log_catalog(void)
{
	return log_catalog;
}
char * get_local_socket(void)
{
	return local_socket;
}

char * get_access_file(void)
{
	return access_file;
}

int total_check(void)
{
	int rc = 0;
	int size = 0;
	struct stat buf;

	user_name = getenv(USER_NAME);
	if(user_name == NULL){
		fprintf(stderr,"Нет переменой \"USER\" в системе !!");
		exit(1);
	}
	home_user = getenv(HOME_USER);
	if(home_user == NULL){
		fprintf(stderr,"Нет переменой \"HOME\" в системе !!");
		exit(1);
	}
	
	size = strlen(home_user);
	size += SIZE_STR_HOME_CATAOLG;
	home_catalog = str_alloc(size);
	strcat(home_catalog,home_user);
	strcat(home_catalog,HOME_CATALOG);
	rc = stat(home_catalog,&buf);
	if(rc != 0){
		rc = mkdir(home_catalog,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
		if(rc != 0){
			perror(home_catalog);
			exit(1);
		}
	}

	size = strlen(home_catalog);
	size += SIZE_STR_INI;
	ini_file = str_alloc(size);
	strcat(ini_file,home_catalog);
	strcat(ini_file,INI);
	rc = stat(ini_file,&buf);
	if(rc != 0){
		perror(ini_file);
		exit(1);
	}

	size = strlen(home_catalog);
	size += SIZE_STR_LOG_CATALOG;
	log_catalog = str_alloc(size);
	strcat(log_catalog,home_catalog);
	strcat(log_catalog,LOG_CATALOG);
	rc = stat(log_catalog,&buf);
	if(rc != 0){
		perror(log_catalog);
		exit(0);
	}	

	size = strlen(home_catalog);
	size += SIZE_STR_LOCAL_SOCKET;
	local_socket = str_alloc(size);
	strcat(local_socket,home_catalog);
	strcat(local_socket,LOCAL_SOCKET);

	size = strlen(home_catalog);
	size += SIZE_STR_ACCESS;
	access_file = str_alloc(size);
	strcat(access_file,home_catalog);
	strcat(access_file,ACCESS);
	rc = stat(access_file,&buf);
	if(rc != 0){
		perror(ini_file);
		exit(1);
	}
#if 0
	if(g_mem_is_system_malloc()){
		DEBUG_PRINTF_S("malloc!");
	}
	else{
		DEBUG_PRINTF_S("Not malloc!");
	}
#endif	
	return SUCCESS;
}

/*****************************************************************************/
