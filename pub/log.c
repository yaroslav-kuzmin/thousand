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
/* Модуль: работа с логом программы                                          */
/*****************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

#include "total.h"
#include "alloc.h"
#include "ini.h"
#include "warning.h"
#include "pub.h"

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
static char file_name[] = "log/vacation.000000.log";
#define SIZE_TEMP_STR     9

inline static int init_file_name(void)
{
	int rc = 0;
	char temp[SIZE_TEMP_STR] = {0};
	char * str_const = file_name;

	current_time();
	sprintf(temp,"%02d%02d%04d",mday,mon,year);

	file_name[13] = temp[6];
	file_name[14] = temp[7];

	file_name[15] = temp[2];
	file_name[16] = temp[3];

	file_name[17] = temp[0];
	file_name[18] = temp[1];
	
	rc = search_parameter(WORK_CATALOG);

	if(rc == 0)
		return FAILURE;

	full_file_name = str_alloc(rc + LEN_FILE_NAME_STR);

	rc = full_value_parameter(WORK_CATALOG,&full_file_name,rc + 1);

	if((rc == NOT_PARAMETER) || (rc == LONG_SIZE_VALUE_PARAMETER ) )
		return FAILURE;
	
	strcat(full_file_name,str_const);

#ifdef _DEBUG
	printf("log file :> %s\n",full_file_name);
#endif 	
	return SUCCESS;
}

#define SIZE_BUFFER_LOG     0x2800
static char buffer_log[SIZE_BUFFER_LOG];
static int len_buffer_log = 0;
#define SIZE_BUFFER_TEMP    0x400	 
static char buffer_temp[SIZE_BUFFER_TEMP];

static FILE * stream_log;
#define NOT_OPEN      1
#define OPEN          0
static int open_log = NOT_OPEN;
int init_log_system(void)
{
	int rc = 0;
	char * str = buffer_log;
	rc = 	init_file_name();
	if(rc == FAILURE){
		global_warning("Not work catalog in file ini");
		return FAILURE;
	}
	stream_log = fopen(full_file_name,"a");
	if(stream_log == NULL){
		global_warning("Not open file log %s",full_file_name);
		open_log = NOT_OPEN;
		return FAILURE;
	}
	else{
		current_time();
		sprintf(str,"\n %02d.%02d.%02d %02d:%02d:%02d :> Start log system !\n",mday,mon,year,hour,min,sec);
		len_buffer_log = strlen(str);
		open_log = OPEN;
	}
	return SUCCESS;
}

int close_log_system(void)
{
	char * str = buffer_log;
	if(registration_operation == YES){
		fprintf(stream_log,str);
		current_time(); 
		fprintf(stream_log," %02d.%02d.%02d %02d:%02d:%02d :> Shutdown log system !\n",mday,mon,year,hour,min,sec);
	}	
	fclose(stream_log);	
	open_log = NOT_OPEN;
	return SUCCESS;
}

int global_log(char * str,...)
{
	char * buf = buffer_log + len_buffer_log;
	char * buf_temp = buffer_temp;
	int len_buf_temp = 0;
	va_list arg;
	va_start(arg,str);

	if(registration_operation == NO){
		va_end(arg);
		return FAILURE;
	}
	if(open_log != OPEN ){
		vfprintf(stderr,str,arg);
		fprintf(stderr,"\n");
		va_end(arg);
		return SUCCESS;
	}

	current_time();
	sprintf(buf_temp," %02d.%02d.%02d %02d:%02d:%02d :> ",mday,mon,year,hour,min,sec);
	len_buf_temp = strlen(buf_temp);
	buf_temp += len_buf_temp;

	vsnprintf(buf_temp,SIZE_BUFFER_TEMP - len_buf_temp,str,arg);
	len_buf_temp += strlen(buf_temp);
	buf_temp = buffer_temp;
	buf_temp += len_buf_temp;
	strcpy(buf_temp,"\n");
	len_buf_temp ++;
	if((len_buffer_log + len_buf_temp) >= SIZE_BUFFER_LOG){
		buf = buffer_log;
		fprintf(stream_log,buf);
		len_buffer_log = 0;
	}
	buf_temp = buffer_temp;
	strcpy(buf,buf_temp);
	len_buffer_log += len_buf_temp;	

	va_end(arg);
	return SUCCESS;
}

