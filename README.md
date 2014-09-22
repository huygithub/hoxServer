hoxServer
==========

hoxServer is the Xiangqi server that is currently hosted on http://www.playxiangqi.com
It is written entirely in C++ and designed to run in a Linux server.

FAQ
---

### How to compile this program ###

Here are the basic steps to compile the server from scratch on "Ubuntu Server 14.04.1 (64-bit) LTS"
- Install the required packages:
    - $ sudo apt-get install cmake build-essential
    - $ sudo apt-get install libboost-dev libsqlite3-dev libst-dev libconfig++-dev
- Build 'dbagent':
    - $ cd hoxServer/dbagent
    - $ mkdir build && cd build
    - $ cmake ..
    - $ make
- Run 'dbagent':
    - $ mkdir logs && mkdir -p server/logs
    - $ ../run.sh
- Build 'server'
    - $ cd hoxServer/server
    - $ mkdir build && cd build
    - $ cmake ..
    - $ make
- Run 'server':
    - $ mkdir logs
    - $ ln -s ../server.cfg
    - $ ../run.sh

License
-------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
