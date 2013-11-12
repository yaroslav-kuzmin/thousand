###############################################################################
#                                                                             #
# Thousand - card game                                                        #
#  Copyright (C) <2013> <Kuzmin Yaroslav>                                     #
#                                                                             #
#  This program is free software: you can redistribute it and/or modify       #
#  it under the terms of the GNU General Public License as published by       #
#  the Free Software Foundation, either version 3 of the License, or          #
#  (at your option) any later version.                                        #
#                                                                             #
#  This program is distributed in the hope that it will be useful,            #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#  GNU General Public License for more details.                               #
#                                                                             #
#  You should have received a copy of the GNU General Public License          #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.      #
#                                                                             #
#   Email contact: kuzmin.yaroslav@gmail.com                                  #
#                                                                             #
###############################################################################

# добавить каталог в опцию sed 
OBJ_CATALOG=obj/
DEPEND_CATALOG=depend/

LIB=../lib/libthp.a
SOURCE=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJ_CATALOG)%.o,$(SOURCE))
DEPEND=$(patsubst %.c,$(DEPEND_CATALOG)%.d,$(SOURCE))

AR=ar
CXX=gcc
CFLAGS=-g2 -Wall -I. -DDEBUG -D_GNU_SOURCE `pkg-config --cflags glib-2.0`
ARFLAGS=sru

$(LIB):$(OBJS)
	$(AR) $(ARFLAGS) $@ $^ 

$(OBJ_CATALOG)%.o:%.c
	$(CXX) $(CFLAGS) -c $< -o $@

$(DEPEND_CATALOG)%.d:%.c
	$(CXX) -MM -I. $< | sed -e '1s/^/obj\//' > $@

include $(DEPEND)

.PHONY:clean
clean:
	-rm -f $(LIB) *~ $(OBJ_CATALOG)*.o $(DEPEND_CATALOG)*.d 













