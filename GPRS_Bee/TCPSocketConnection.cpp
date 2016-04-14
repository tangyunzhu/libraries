/*
  TCPSocketConnection.cpp
  2014 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:lawliet zou(lawliet.zou@gmail.com)
  2014-2-24

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "TCPSocketConnection.h"

TCPSocketConnection::TCPSocketConnection()
{
}

int TCPSocketConnection::connect(const char* host, const int port)
{
    if (!gprs->connect(TCP, host, port)) {
        return false;
    }
    return true;
}

bool TCPSocketConnection::is_connected(void)
{
    return gprs->is_connected();
}

int TCPSocketConnection::send(char* data, int length)
{
    return gprs->send(data, length);
}

int TCPSocketConnection::send_all(char* data, int length)
{
    return send(data,length);
}

int TCPSocketConnection::receive(char* data, int length)
{
#if 0
	int size = gprs->wait_readable(DEFAULT_TIMEOUT);
    if(size)
		return gprs->recv(data, size>length?length:size);
	else	
		return false;
#endif
	return gprs->recv(data,length);
}

int TCPSocketConnection::receive_all(char* data, int length)
{
    return receive(data,length);
}
