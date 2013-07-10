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


#undef FALSE
#define FALSE          (0)
#undef TRUE
#define TRUE           (!FALSE)

#define BEGIN_TIME     1900

#ifdef DEBUG
#define DEBUG_PRINTF_S(n)  printf(" debug :> %s\n",n);
#define DEBUG_PRINTF_D(n)  printf(" debug :> %d\n",n);
#define DEBUG_PRINTF_X(n)  printf(" debug :> %#x\n",n);
#define DEBUG_PRINTF_LD(n) printf(" debug :> %lld\n",n);
#define DEBUG_PRINTF_P(n)  printf(" debug :> %p\n",n); 
#else 
#define DEBIG_PRINTF_S(n)   ;
#define DEBIG_PRINTF_D(n)   ;
#define DEBIG_PRINTF_X(n)   ;
#define DEBIG_PRINTF_LD(n)   ;
#endif 

#define SERVER_FLAG       0x01
#define CLIENT_FLAG       0x02
#define ROBOT_FLAG        0x03
#endif


