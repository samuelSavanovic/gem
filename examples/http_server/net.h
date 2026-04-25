/* net.h — declarations for Gem extern fn wrappers */
#ifndef NET_H
#define NET_H

#include <stdint.h>

int64_t net_listen(int64_t port);
int64_t net_accept(int64_t server_fd);
char *net_read(int64_t fd, int64_t max_bytes);
int64_t net_write(int64_t fd, char *data);
void net_close(int64_t fd);

#endif
