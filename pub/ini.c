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
/*  Модуль: Работы с файлом параметров                                       */
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "pub.h"
#include "log.h"
#include "warning.h"

#define MIN_SIZE_CONFIG_FILE   30 

static char file_name[] = "vacation.ini";
static char * buffer_config = NULL;
static int size_buffer_config = 0;
static int change_config = NO;

int read_config(void)
{
	int rc = 0;	
	FILE * stream = NULL;
	struct stat buffer_stat_file;
	off_t size_file = 0;

	rc = stat(file_name,&buffer_stat_file);
	if(rc != 0){
		global_warning("Not file ini %s",file_name);
 		return FAILURE;
	}
	size_file = buffer_stat_file.st_size;
	if(size_file <= MIN_SIZE_CONFIG_FILE){
		 global_warning("Not config data %d",MIN_SIZE_CONFIG_FILE);
		return FAILURE;
	}
	
	buffer_config = malloc(size_file + 1);
		assert(buffer_config);
	stream = fopen(file_name,"r");
	if(stream == NULL){
		 global_warning("Not open file ini %s",file_name);
		return FAILURE; 
	}
	global_log("Open ini file %s",file_name);
	rc = fread(buffer_config,size_file,1,stream);
	global_log("Read ini file : %d",rc);
	buffer_config[size_file] = 0;
	size_buffer_config = size_file;

#ifdef _DEBUG
	DEBUG;
	printf("%s\n",buffer_config);
	DEBUG;
#endif
	fclose(stream);	 
	
 	return SUCCESS;	
}

int search_parameter(char * parameter)
{
	int i;
	char * buffer = strstr(buffer_config,parameter);
	if(buffer == NULL){
		global_warning("Not name parametr in ini file : %s",parameter);
		return 0;
	}
	for(i = 0; i < size_buffer_config;i++,buffer++){
		if(*buffer == '='){
	 		buffer++;
			if(*buffer == ' ')
				buffer++;
		 	break;
		} 
	}

	for(i = 0;i < size_buffer_config;i++,buffer++){
		if((*buffer == ' ') || (*buffer == '\n') || (*buffer == 0)){
			break;
		}	
	}
	global_log("Find parametr : %s size %d",parameter,i);
	return i;
}

int full_value_parameter(char * parameter,char ** value_parameter,int count)
{
	int i = 0;
	char * buffer = NULL;
	char * buffer_temp = *value_parameter;
	buffer = strstr(buffer_config,parameter);
	if(buffer == NULL){
		global_warning("Not name parametr in ini file : %s",parameter);
		return NOT_PARAMETER;
	}
	for(i = 0;i<size_buffer_config;i++,buffer++){
		if(*buffer == '='){
	 		buffer++;
			if(*buffer == ' ')
				buffer++;
		 	break;
		} 
	}
#ifdef _DEBUG
	printf(" name parameter :> %s : value parameter :> %s",parameter,buffer);
#endif		
	for(i = 0;i < count;i++,buffer_temp++,buffer++){
		if((*buffer == ' ') || (*buffer == '\n')){
			*buffer_temp = 0;
			break;
		}	
		*buffer_temp = *buffer;
	}
	if(i == count){
		buffer_temp--;
		*buffer_temp = 0;
		global_warning("Long size value parametr : %s",*value_parameter);
		return LONG_SIZE_VALUE_PARAMETER;
	}
	global_log("Find parametr : %s",parameter);
	global_log("              :> %s",*value_parameter);
	return SUCCESS; 
}

#define SIZE_SPACE       3
#define STR_SPACE       " = "
int add_new_parameter(char * parameter,char * value_parameter)
{
	size_t len_p = 0;
	size_t len_v = 0;

	int size_buffer = 0;
	char * buffer = NULL;

	buffer = strstr(buffer_config,parameter);
	if(buffer != NULL){
		global_warning("There name parametr in ini file : %s",parameter);
		return FAILURE;
	}

	len_p = strlen(parameter);
	len_v = strlen(value_parameter);
	
	size_buffer = size_buffer_config + len_p + len_v + SIZE_SPACE + 1 + 1;//+1 символ конец строки \n, +1 на strcat  
	
	buffer = (char *)malloc(size_buffer * sizeof(char));
		assert(buffer);
	memset(buffer,0,size_buffer);
	strcat(buffer,buffer_config);
	strcat(buffer,parameter);
	strcat(buffer,STR_SPACE);
	strcat(buffer,value_parameter);
	strcat(buffer,"\n"); // конец строки 	

	free(buffer_config);
	buffer_config = buffer;
	size_buffer_config = size_buffer;
#ifdef _DEBUG
	printf(" config :>\n%s\n",buffer_config);
#endif	
	change_config = YES;
	return SUCCESS;
}

int rewrite_parameter(char * parameter,char * value_parameter)
{
	char * buffer_begin = NULL;
	char * buffer_end = NULL;
	char * buffer_temp = NULL;
	size_t len_p = 0;
	size_t len_v = 0;
	size_t len_str = 0;
	size_t size_buffer = 0;

	buffer_begin = strstr(buffer_config,parameter);
	if(buffer_begin == NULL){
		return add_new_parameter(parameter,value_parameter);
	}

	len_p = strlen(parameter);
	len_v = strlen(value_parameter);

	buffer_end = buffer_begin;
	for(len_str = 0;len_str < size_buffer_config;len_str++,buffer_end++){
		if(*buffer_end == '\n'){
			*buffer_begin = 0; // начало строки изменения
			break;
		}	
	}
	
	size_buffer = size_buffer_config - len_str + len_p + len_v + SIZE_SPACE + 1;//+1 на strcat 

#ifdef _DEBUG
	DEBUG;
	printf(" size buffer :> %d | len_str :> %d | len_p :> %d | len_v :> %d\n",size_buffer,len_str,len_p,len_v);
#endif

	buffer_temp = (char *)malloc(size_buffer * sizeof(char));
		assert(buffer_temp);
	
	memset(buffer_temp,0,size_buffer);
	strcat(buffer_temp,buffer_config);
	strcat(buffer_temp,parameter);
	strcat(buffer_temp,STR_SPACE);
	strcat(buffer_temp,value_parameter);
	strcat(buffer_temp,buffer_end);	

	free(buffer_config);
	buffer_config = buffer_temp;
	size_buffer_config = size_buffer;

#ifdef _DEBUG 
	printf(" config :> %d\n%s\n",size_buffer_config,buffer_config);
#endif	
	change_config = YES;
	return SUCCESS;
}

int close_config(void)
{
	FILE * stream = NULL;

	if(change_config == YES){
		stream = fopen(file_name,"w");
		if(stream == NULL){
			global_warning("Not open ini file from write !");
 			goto exit_close_config;
		}
 		fputs(buffer_config,stream);
		fclose(stream);
 	}

exit_close_config:
	free(buffer_config);
	global_log("Close ini file %s",file_name);
	return SUCCESS;
}


