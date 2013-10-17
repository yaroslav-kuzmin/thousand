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

#ifndef BIT_FLAG_H
#define BIT_FLAG_H
/* Работа с полями битов*/
int init_bit_fields(void);
int deinit_bit_fields(void);
/*Выделение одного битового поля для работы и получение его номера*/
uint32_t init_bit_flags(uint32_t size);
/*Высвобождене битового поля*/
int deinit_bit_flag(uint32_t number);
/*Запрос на увеличение количества бит в поле*/
int reinit_bit_flags(uint32_t number,uint32_t size);
/*Установить флаги с определеного номера number_bit на определеный размер size*/
int set_bit_flag(uint32_t number,uint32_t number_bit,uint32_t size);
/*Снять флаг с определеного номера number_bit на определеный размер size*/
int get_bit_flag(uint32_t number, uint32_t number_bit,uint32_t size);
/*Проверяет установлен флаг или нет */
int check_bit_flag(uint32_t number,uint32_t number_bit,uint32_t size);
/*Находит поле свободных флагов размером size и устанавливает их*/
uint32_t free_bit_flag(uint32_t number,uint32_t size);

#endif
