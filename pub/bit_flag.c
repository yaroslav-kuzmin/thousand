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
/* Модуль содержит информацию о битовых флагах                               */
/*****************************************************************************/
/*****************************************************************************/
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include <glib.h>

#include "pub.h"

/*****************************************************************************/
/* глобальные переменые                                                      */
/*****************************************************************************/
#define AMOUNT_BIT_IN_UINT8     3

#define AMOUNT_BIT_FIELDS    3
typedef struct bit_field_s
{
	uint32_t byte;
	uint8_t * bits;
}bit_field_t;

bit_field_t * bit_fields = NULL;
int amount_bit_fields = 0;
int current_bit_field = 0;
/*****************************************************************************/
/* Вспомогательные функция                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Основные Функции                                                          */
/*****************************************************************************/
int init_bit_fields(void)
{
	size_t s = sizeof(bit_field_t) * AMOUNT_BIT_FIELDS;
	bit_fields = (bit_field_t *)g_malloc(s);
	memset(bit_fields,0,s);	
	amount_bit_fields = AMOUNT_BIT_FIELDS;
	current_bit_field = 0;
	return SUCCESS;
}
int deinit_bit_fields(void)
{
	g_free(bit_fields);
	amount_bit_fields = 0;
	current_bit_field = 0;
	return SUCCESS;	
}
int reinit_bit_fields(void)
{
	bit_field_t * bft = NULL;
	bit_field_t * bfst = bit_fields;
	int ns = sizeof(bit_field_t) * (amount_bit_fields + AMOUNT_BIT_FIELDS);
	int os = sizeof(bit_field_t) * amount_bit_fields;

	bft = (bit_field_t *)g_malloc(ns);
	memset(bft,0,ns);
	memcpy(bft,bfst,os);
	g_free(bit_fields);
	bit_fields = bft;
	amount_bit_fields = amount_bit_fields + AMOUNT_BIT_FIELDS;

	return SUCCESS;
}

/*---------------------------------------------------------------------------*/
int init_bit_flags(uint32_t size)
{
	bit_field_t * bfst = bit_fields;	
	uint32_t s = size >> AMOUNT_BIT_IN_UINT8;
	uint8_t * t;
	int c = 0;

	if( (size == 0) || (size - (s<<AMOUNT_BIT_IN_UINT8)) )
		s++;

	for(c = 0;c < current_bit_field;c++){
		if(bfst->byte == 0 ){
			break;
		}
		bfst++; 
	}

	if(c == current_bit_field){
		if(current_bit_field == amount_bit_fields){
			reinit_bit_fields();
			bfst = bit_fields + current_bit_field;
		}
		current_bit_field++;
	}
	t = (uint8_t *)g_malloc(s);
	memset(t,0,s);
	bfst->bits = t;
	bfst->byte = s;

	return c;
}	

int deinit_bit_flag(int number)
{
	bit_field_t * bft = bit_fields;	
	uint8_t * t;

	if(number >= current_bit_field){
		return FAILURE;
	}	
	bft = bft + number;
	t = bft->bits;
	g_free(t);
	bft->bits = NULL;
	bft->byte = 0;
	return SUCCESS;
}

int reinit_bit_flags(int number,uint32_t size)
{
	bit_field_t * bft = bit_fields;
	uint8_t *new_tb;
	uint8_t *old_tb;
	uint32_t old_s;
	uint32_t new_s;

	if(number >= current_bit_field){
		return FAILURE;
	}
	bft = bft + number;
	old_tb = bft->bits;
	old_s = bft->byte;
	new_s = size >> AMOUNT_BIT_IN_UINT8;

	if( (size - (new_s<<AMOUNT_BIT_IN_UINT8)) ){
		new_s++;
	}

	if(new_s < old_s){
		return SUCCESS;
	}

	new_tb = (uint8_t *)g_malloc(new_s);
	memset(new_tb,0,new_s);
	memcpy(new_tb,old_tb,old_s);	
	g_free(old_tb);

	bft->bits = new_tb;
	bft->byte = new_s;
	return SUCCESS;
}
/*---------------------------------------------------------------------------*/
int set_bit_flag(int number,uint32_t number_bit,uint32_t size)
{
	bit_field_t * bft = bit_fields;	
	uint8_t * bits;
	uint32_t as;

	uint32_t i, n;
	uint32_t n_byte;
	int n_bit; 
	uint8_t * t_c;

	if(number >= current_bit_field){
		return FAILURE;
	}
	bft = bft + number;
	bits = bft->bits;
	as = bft->byte;
	/*Проверка на переполнение*/
	if( (number_bit + size) > (as << AMOUNT_BIT_IN_UINT8) ){
		return FAILURE;
	}

	n = number_bit;
	for(i = 0;;n++){
		uint8_t c;
		n_byte = n >> AMOUNT_BIT_IN_UINT8;
		n_bit = 1 << ( n - (n_byte <<AMOUNT_BIT_IN_UINT8));
		t_c = bits + n_byte;
		c = *t_c;
		*t_c = c | n_bit;
		i++;
		if(i >= size){
			break;
		}
	}

	return SUCCESS;
}

int get_bit_flag(int number, uint32_t number_bit,uint32_t size)
{
	bit_field_t * bft = bit_fields;	
	uint8_t * bits;
	uint32_t as;

	uint32_t i,n;
	uint32_t n_byte;
	int n_bit; 
	uint8_t * t_c;
	
	if(number >= current_bit_field){
		return FAILURE;
	}
	bft = bft + number;
	bits = bft->bits;
	as = bft->byte;

	if( (number_bit + size) > (as << AMOUNT_BIT_IN_UINT8) ){
		return FAILURE;
	}

	n = number_bit;

	for(i = 0;;n++){
		uint8_t c;
		n_byte = n >> AMOUNT_BIT_IN_UINT8;
		n_bit = 1 << ( n - (n_byte <<AMOUNT_BIT_IN_UINT8));
		t_c = bits + n_byte;
		c = *t_c;
		*t_c = c & (~n_bit);
		i++;
		if(i >= size){
			break;
		}
	}
	return SUCCESS;
}

int check_bit_flag(int number,uint32_t number_bit,uint32_t size)
{
	bit_field_t * bft = bit_fields;	
	uint8_t * bits;
	uint32_t as;

 	uint32_t i,n;
	uint32_t n_byte;
	int n_bit; 
	uint8_t * t_c;
	
	if(number >= current_bit_field){
		return FAILURE;
	}
	bft = bft + number;
	bits = bft->bits;
	as = bft->byte;

	if( (number_bit + size) > (as << AMOUNT_BIT_IN_UINT8) ){
		return FAILURE;
	}

	n = number_bit;
	for(i = 0;i < size;i ++,n++){
		uint8_t c;
		n_byte = n >> AMOUNT_BIT_IN_UINT8;
		n_bit = 1 << ( n - (n_byte <<AMOUNT_BIT_IN_UINT8));
		t_c = bits + n_byte;
		c = *t_c;
		if( !(c & n_bit)){
			return NO;
		}
	}
	return YES;
}

/*выдает первый свободный порядковый номер*/
uint32_t free_bit_flag(int number,uint32_t size)
{
	bit_field_t * bft = bit_fields;	
	uint8_t * bits;
	uint32_t as;

	uint32_t n; 
	uint32_t s,cs;
	uint32_t n_byte;
	int n_bit; 
	uint8_t * t_c;
	uint32_t a; 
	
	if(number >= current_bit_field){
		return UINT32_MAX;
	}
	bft = bft + number;
	bits = bft->bits;
	as = bft->byte;
	a = as << AMOUNT_BIT_IN_UINT8; 

	if( size > a ){
		return UINT32_MAX;
	}

	if(size == 0){
		s = 1;
	}

	for(n = 0;n < a;n++ ){
		uint32_t nt = n;
		n_byte = n >> AMOUNT_BIT_IN_UINT8;
		n_bit = 1 << ( n - (n_byte <<AMOUNT_BIT_IN_UINT8));
		t_c = bits + n_byte;
		for(s = 0,cs = 0;s < size;s++){
			uint8_t c = *t_c;
			if( !(c & n_bit)){
				cs ++; 
				if(cs >= s){
					if(  FAILURE != set_bit_flag(number,n,size))
						return n;
					else
						return UINT32_MAX;
				}
			}
			else{
				n = nt;
				break; 
			}
			nt++;
			n_byte = nt >> AMOUNT_BIT_IN_UINT8;
			n_bit = 1 << ( nt - (n_byte <<AMOUNT_BIT_IN_UINT8));
			t_c = bits + n_byte;
		}
	}
	return UINT32_MAX;
}

/*****************************************************************************/
