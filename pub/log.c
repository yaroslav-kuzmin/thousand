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
/* Модуль: работа с логом программы                                          */
/*****************************************************************************/

/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include "pub.h"
#include "total.h"
#include "alloc.h"
#include "warning.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/
static struct tm * tm_current;
static time_t time_current = 0;

static int mday;
static int mon;
static int year;
static int hour;
static int min;
static int sec;

/*****************************************************************************/
/* Вспомогательные функция                                                   */
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
static char * file_log;
static char FILE_LOG[] = ".0000000.log";
#define SIZE_STR_FILE_LOG  21
#define SIZE_TEMP_STR     8

inline static int init_file_name(int flag)
{
	int rc = 0;
	char temp[SIZE_TEMP_STR] = {0};
	char * catalog_log = NULL;

	rc = getpid();
	sprintf(temp,"%07d",rc);

	FILE_LOG[1] = temp[0];
	FILE_LOG[2] = temp[1];
	FILE_LOG[3] = temp[2];
	FILE_LOG[4] = temp[3];
	FILE_LOG[5] = temp[4];
	FILE_LOG[6] = temp[5];
	FILE_LOG[7] = temp[6];
	

	catalog_log = get_log_catalog();
	rc = strlen(catalog_log);
	rc += SIZE_STR_FILE_LOG;

	file_log = str_alloc(rc);
	strcat(file_log,catalog_log);
	switch(flag){
		case SERVER_FLAG:
			strcat(file_log,SERVER);
			break;
		case CLIENT_FLAG:
			strcat(file_log,CLIENT);
			break;
		case ROBOT_FLAG:
			strcat(file_log,ROBOT);
			break;
		default:
			strcat(file_log,DEFAULT);
			break;
	}
	strcat(file_log,FILE_LOG);
	/*DEBUG_PRINTF_S(file_log);*/
	return SUCCESS;
}

#define SIZE_BUFFER_LOG     0x2800
static char buffer_log[SIZE_BUFFER_LOG];
static int len_buffer_log = 0;
#define SIZE_BUFFER_TEMP    0x400	 
static char buffer_temp[SIZE_BUFFER_TEMP];

static FILE * log_stream = NULL;

int init_log_system(int flag)
{
	int rc = 0;
	char * str = buffer_log;
	rc = 	init_file_name(flag);

	rc = get_registration_operation();
	if(rc != YES){
		if(log_stream != NULL){
			fclose(log_stream);
		}
		log_stream = NULL; 
		return SUCCESS;
	}

	log_stream = fopen(file_log,"a");
	if(log_stream == NULL){
		global_warning("Немогу открыть файл для записи лога %s : %s",file_log,strerror(errno));
		return FAILURE;
	}
	else{
		current_time();
		sprintf(str,"****************************************\n");
		str += strlen(str);
		sprintf(str," %02d.%02d.%02d %02d:%02d:%02d :> Запуск !\n",mday,mon,year,hour,min,sec);
		str = buffer_log;
		len_buffer_log = strlen(str);
	}
	return SUCCESS;
}

int close_log_system(void)
{
	char * str = buffer_log;
	if(log_stream != NULL){
		fprintf(log_stream,"%s",str);
		current_time(); 
		fprintf(log_stream," %02d.%02d.%02d %02d:%02d:%02d :> Останов !\n",mday,mon,year,hour,min,sec);
		fflush(log_stream);
		fclose(log_stream);	
		open_log = NOT_OPEN;
	}	
	return SUCCESS;
}

/*****************************************************************************/
int global_log(char * str,...)
{
	char * buf = buffer_log + len_buffer_log;
	char * buf_temp = buffer_temp;
	int len_buf_temp = 0;
	va_list arg;
	va_start(arg,str);

	if(log_stream == NULL ){
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
		fprintf(log_stream,"%s",buf);
		len_buffer_log = 0;
	}
	buf_temp = buffer_temp;
	strcpy(buf,buf_temp);
	len_buffer_log += len_buf_temp;	

	va_end(arg);
	return SUCCESS;
}

