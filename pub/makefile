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
#   Email contact: esdt@mail.ru                                               #
#                                                                             #
###############################################################################

LIB=libthp.a
OBJS=warning.o alloc.o log.o ini.o total.o
AR=ar
CXX=gcc
CFLAGS=-g2 -Wall
ARFLAGS=sru

$(LIB):$(OBJS)
	$(AR) $(ARFLAGS) $@ $^ 

.c.o:
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	-rm -f $(EXEC) *.o *~ 













