#pragma once

// ===== defines =====
#define MAXSLEEP 16
#define SRVR_PORT 9001

// ===== includes =====
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

// ===== funcs =====
int mx_connect_retry(const struct sockaddr *adr, socklen_t alen, int *err);
