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
/* Объявлене глобальных макросов                                             */
/*****************************************************************************/

#ifndef PUB_H
#define PUB_H

#define SUCCESS        0
#define FAILURE        -1

#define YES            1
#define NO             0 

#define NOT_PARAMETER                 -1
#define LONG_SIZE_VALUE_PARAMETER     -2
#define NAME_BUSY                     -3
#define CONSILIENCE_NUMBER            -4
#define LESS_DATE                     -5
#define MORE_DATE                     -6
#define NOT_PERSON_LIST               -8


#undef FALSE
#define FALSE          (0)
#undef TRUE
#define TRUE           (!FALSE)

#define BEGIN_TIME     1900

/*
#define ERROR_OPEN_WARNING_FILE  70
#define ABORT_GET_OPTION         71
#define ERROR_GTK_INIT           72
#define ERROR_MAIN_WINDOW        73
*/

#define WORK_CATALOG             "work_catalog"

#ifdef DEBUG_INFO
#define DEBUG   printf("\n <: debug :>\n")
#endif 

#define AMOUNT_DAY_YEAR                  366
#define DEFAULT_AMOUNT_DAY_VACATION      28

#define YEARtoTIME(x)   ((x<<16) & 0xFFFF0000)
#define TIMEtoYEAR(x)   ((x>>16) & 0x0000FFFF)
#define MONtoTIME(x)    ((x<<8) & 0x00000F00)
#define TIMEtoMON(x)    ((x>>8) & 0x0F)
#define DAYtoTIME(x)    (x & 0x0000001F)
#define TIMEtoDAY(x)    (x & 0x1F)

#define ADD_NEW_ACTION     1
#define ADD_LOAD_ACTION    2	
#define DELETE_ACTION      3
#define CHANGE_ACTION      4

#endif


