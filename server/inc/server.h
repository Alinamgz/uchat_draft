#pragma once

// ===== defines =====
// #define SRVR_ADDR
#define SRVR_PORT 9001

// ===== includes =====
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

// ===== funcs =====
void mx_server_err(int err, int serv_fd, int client_fd);
