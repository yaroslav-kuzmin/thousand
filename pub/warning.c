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
/*   Email contact: esdt@mail.ru                                             */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/
/* Модуль: работа с предупреждениями в  программе                            */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pub.h"
#include "total.h" 
#include "alloc.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/
int len_record;

static struct tm * tm_current;
static time_t time_current = 0;

static int mday;
static int mon;
static int year;
static int hour;
static int min;
static int sec;

/*****************************************************************************/
inline static void current_time(void)
{
	time_current = time(NULL);
	tm_current = gmtime(&time_current);

	mday = tm_current->tm_mday;
	mon = tm_current->tm_mon + 1;
	year = tm_current->tm_year + BEGIN_TIME;
	hour = tm_current->tm_hour;
	min = tm_current->tm_min;
	sec = tm_current->tm_sec;
} 

/*****************************************************************************/
static char * file_warning; 
static char FILE_WARNING[] = "vacation.000000.warning";
#define SIZE_STR_FILE_WARNING 24
#define SIZE_TEMP_STR   9

inline static int init_file_name(void)
{
	int rc = 0;
	char temp[SIZE_TEMP_STR] = {0};
	char * catalog_log = NULL;

	current_time();
	sprintf(temp,"%02d%02d%04d",mday,mon,year);

	FILE_WARNING[9] = temp[6];
	FILE_WARNING[10] = temp[7];

	FILE_WARNING[11] = temp[2];
	FILE_WARNING[12] = temp[3];

	FILE_WARNING[13] = temp[0];
	FILE_WARNING[14] = temp[1];

	catalog_log = get_log_catalog();
	rc = strlen(catalog_log);
	rc += SIZE_STR_FILE_WARNING;

	file_warning = str_alloc(rc);
	strcat(file_warning,catalog_log);
	strcat(file_warning,FILE_WARNING);
	/*DEBUG_PRINTF_S(file_warning);*/
	return SUCCESS;
}

static FILE * warning_stream;
#define NOT_OPEN      1
#define OPEN          0
static int open_warnig = NOT_OPEN;

int init_warning_system(void)
{
	init_file_name();

	warning_stream = fopen(file_warning,"a");
	if(warning_stream == NULL){
		fprintf(stderr,"Немогу открыть файл : %s\n",file_warning);
		open_warnig = NOT_OPEN;
		return FAILURE;
	}
	else{
		current_time();
		fprintf(warning_stream," %02d.%02d.%02d %02d:%02d:%02d :> Start programm !\n",mday,mon,year,hour,min,sec);
		open_warnig = OPEN;
	}
	return SUCCESS;
}

int close_warning_system(void)
{
	if(open_warnig == OPEN){
		current_time();
		fprintf(warning_stream," %02d.%02d.%02d %02d:%02d:%02d :> Shutdown programm !\n",mday,mon,year,hour,min,sec);
		fclose(warning_stream);	
		open_warnig = NOT_OPEN;
	}
	return SUCCESS;
}

/*****************************************************************************/
int global_warning(char * str,...)
{
	va_list arg;
	va_start(arg,str);
	if(open_warnig != OPEN){
		vfprintf(stderr,str,arg);
		fprintf(stderr,"\n");
	}
	else{
		current_time();
		fprintf(warning_stream," %02d.%02d.%02d %02d:%02d:%02d :> ",mday,mon,year,hour,min,sec);
		vfprintf(warning_stream,str,arg);
		fprintf(warning_stream,"\n");
		fflush(warning_stream);
	}
	va_end(arg);
	return SUCCESS;
}

