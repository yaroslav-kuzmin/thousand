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
/* Дополнительные файлы                                                      */
/*****************************************************************************/
#include <stdio.h>
#include <signal.h>

#include "pub.h"

#include "net_server.h"
/*****************************************************************************/
/* Глобальные переменые                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Вспомогательные функции                                                   */
/*****************************************************************************/
/* TODO
volatile sig_atomic_t amount_alrm = 0;
void handler_timer(int signal_num)
{
	amount_alrm++;
}
*/
volatile sig_atomic_t amount_sig_io = 1;
void sigaction_io(int num,siginfo_t * sig,void * test)
{
	amount_sig_io++;
}
/*****************************************************************************/
/* Основная функция                                                          */
/*****************************************************************************/
int main_loop(void)
{
	int rc;
	user_t * ptu;
	unsigned long int nu,i;

	DEBUG_PRINTF_S("main_loop");

	for(;;){
/*Проверка нового подсоединения*/
		check_new_connect(); 
/*Чтение информации от клиентов*/
		ptu = get_begin_user_list();
		nu = get_number_user()
		for(i = 0;i < nu;i++,ptu++){

		}
/* Ожидание сигналов на дискрипторах*/
		if(amount_sig_io <= 1){	
			amount_sig_io = 0;
			rc = pause();
		}
		else{
			amount_sig_io--;
		}
	}

	return SUCCESS;
}
/*****************************************************************************/
