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

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pub.h"
#include "alloc.h"
#include "ini.h"

int len_record;

static struct tm * tm_current;
static time_t time_current = 0;

static int mday;
static int mon;
static int year;
static int hour;
static int min;
static int sec;

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

static char * full_file_name; 
#define LEN_FILE_NAME_STR  28
static char file_name[] = "log/vacation.000000.warning";
#define SIZE_TEMP_STR   9

inline static int init_file_name(void)
{
	int rc = 0;
	char temp[SIZE_TEMP_STR] = {0};
	char * str_const = file_name; 

	current_time();
	sprintf(temp,"%02d%02d%04d",mday,mon,year);

	str_const[13] = temp[6];
	str_const[14] = temp[7];

	str_const[15] = temp[2];
	str_const[16] = temp[3];

	str_const[17] = temp[0];
	str_const[18] = temp[1];

	rc = search_parameter(WORK_CATALOG);
	if(rc == 0)
		return FAILURE;

	full_file_name = str_alloc(rc + LEN_FILE_NAME_STR);
	rc = full_value_parameter(WORK_CATALOG,&full_file_name,rc + 1);

	if((rc == NOT_PARAMETER) || (rc == LONG_SIZE_VALUE_PARAMETER ) )
		return FAILURE;

	strcat(full_file_name,str_const);
#ifdef _DEBUG
	printf("warning file :> %s\n",full_file_name);
#endif 	
	return SUCCESS;
}

static FILE * warning_stream;
#define NOT_OPEN      1
#define OPEN          0
static int open_warnig = NOT_OPEN;
int init_warning_system(void)
{
	int rc = 0;
	rc = init_file_name();
	if(rc == FAILURE){
		fprintf(stderr,"Not parametr work_catalog in ini file\n");
		return FAILURE;
	}
	warning_stream = fopen(full_file_name,"a");
	if(warning_stream == NULL){
		fprintf(stderr,"Not open file warning - %s\n",full_file_name);
		open_warnig = NOT_OPEN;
		return FAILURE;
	}
	else{
		current_time();
		fprintf(warning_stream,"\n %02d.%02d.%02d %02d:%02d:%02d :> Start programm !\n",mday,mon,year,hour,min,sec);
		open_warnig = OPEN;
	}
	return SUCCESS;
}

int close_warning_system(void)
{
	current_time();
	fprintf(warning_stream," %02d.%02d.%02d %02d:%02d:%02d :> Shutdown programm !\n",mday,mon,year,hour,min,sec);
	fclose(warning_stream);	
	open_warnig = NOT_OPEN;
	return SUCCESS;
}

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

