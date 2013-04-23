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

all:
	make -C pub
	make -C server
	make -C client
	make -C robot

.PHONY:server
server:
	make -C pub
	make -C server

.PHONY:client
client:	
	make -C pub
	make -C client

.PHONY:robot
robot:
	make -C pub
	make -C robot

.PHONY:clean
.PHONY:clean_all
.PHONY:clean_client
.PHONY:clean_server
.PHONY:clean_robot
.PHONY:clean_pub


clean:clean_all
	
clean_all:
	make -C pub clean 
	make -C server clean
	make -C client clean
	make -C robot clean

clean_pub:
	make -C pub clean

clean_server:clean_pub
	make -C server clean

clean_client:clean_pub
	make -C client clean

clean_robot:clean_pub
	make -C robot clean


