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

EXEC=ths
OBJS=server.o kernel.o net_server.o list_user.o list_message.o access.o \
     list_acting.o
CXX=gcc
PUB_CATALOG=../pub/
CFLAGS=-g2 -Wall -I$(PUB_CATALOG) -I. `pkg-config --cflags glib-2.0` \
       -DDEBUG -D_GNU_SOURCE -DG_LOG_DOMAIN=\"ths\"
LDFLAGS=-g2 -L$(PUB_CATALOG)
LIB_PUB=$(PUB_CATALOG)libthp.a
LIB=-lthp -lglib-2.0

$(EXEC):$(OBJS) $(LIB_PUB)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIB)

.c.o:
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	-rm -f $(EXEC) *.o *~ 

