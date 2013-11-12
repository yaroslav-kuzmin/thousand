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

PUB_CATALOG=../pub/
LIB_CATALOG=../lib/
# добавить каталог в опцию sed 
OBJ_CATALOG=obj/
DEPEND_CATALOG=depend/

EXEC=../bin/ths
SOURCE=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJ_CATALOG)%.o,$(SOURCE))
DEPEND=$(patsubst %.c,$(DEPEND_CATALOG)%.d,$(SOURCE))

CXX=gcc
CFLAGS=-g2 -Wall -I$(PUB_CATALOG) -I. `pkg-config --cflags glib-2.0` \
       -DDEBUG -D_GNU_SOURCE -DG_LOG_DOMAIN=\"ths\"
LDFLAGS=-g2 -L$(LIB_CATALOG)
LIB_PUB=$(LIB_CATALOG)libthp.a
LIB=-lthp -lglib-2.0

$(EXEC):$(OBJS) $(LIB_PUB)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIB)

$(OBJ_CATALOG)%.o:%.c
	$(CXX) $(CFLAGS) -c $< -o $@

$(DEPEND_CATALOG)%.d:%.c
	$(CXX) -MM -I$(PUB_CATALOG) -I. $< | sed -e '1s/^/obj\//' > $@

include $(DEPEND)

.PHONY:clean
clean:
	-rm -f $(EXEC) *~ $(OBJ_CATALOG)*.o $(DEPEND_CATALOG)*.d 

