#pragma once

#ifdef WIN32
//workaround for http://bugs.mysql.com/bug.php?id=53316 (mysql bug #53316)
#include <WinSock2.h>
#endif
#include <mysql.h>