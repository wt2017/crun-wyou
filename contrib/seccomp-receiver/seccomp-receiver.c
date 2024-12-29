/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2021 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <config.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>

#define error(status, errno, fmt, ...) do {                             \
    if (errno)                                                          \
      fprintf (stderr, "crun: " fmt, ##__VA_ARGS__);                    \
    else                                                                \
      fprintf (stderr, "crun: %s:" fmt, strerror (errno), ##__VA_ARGS__); \
    if (status)                                                         \
      exit (status);                                                    \
  } while(0)

static int
open_unix_domain_socket (const char *path)
{
  struct sockaddr_un addr = {};
  int ret;
  int fd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0)
    error (EXIT_FAILURE, errno, "error creating UNIX socket");

  strcpy (addr.sun_path, path);
  addr.sun_family = AF_UNIX;
  ret = bind (fd, (struct sockaddr *) &addr, sizeof (addr));
  if (ret < 0)
    error (EXIT_FAILURE, errno, "error binding UNIX socket");

  ret = listen (fd, 1);
  if (ret < 0)
    error (EXIT_FAILURE, errno, "listen");

  return fd;
}

static void
print_payload (int from)
{
  int fd = -1;
  struct iovec iov[1];
  struct msghdr msg = {};
  char ctrl_buf[2048] = {};
  char data[2048];
  int ret;
  struct cmsghdr *cmsg;

  iov[0].iov_base = data;
  iov[0].iov_len = sizeof (data) - 1;

  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_controllen = CMSG_SPACE (sizeof (int));
  msg.msg_control = ctrl_buf;

  do
    ret = recvmsg (from, &msg, 0);
  while (ret < 0 && errno == EINTR);
  if (ret < 0)
    {
      error (0, errno, "recvmsg");
      return;
    }

  data[iov[0].iov_len] = '\0';
  puts (data);

  cmsg = CMSG_FIRSTHDR (&msg);
  if (cmsg == NULL)
    {
      error (0, 0, "no msg received");
      return;
    }
  memcpy (&fd, CMSG_DATA (cmsg), sizeof (fd));
  close (fd);
}

int
main (int argc, char **argv)
{
  char buf[8192];
  int ret, fd, socket;
  if (argc < 2)
    error (EXIT_FAILURE, 0, "usage %s PATH\n", argv[0]);

  unlink (argv[1]);

  socket = open_unix_domain_socket (argv[1]);
  while (1)
    {
      struct termios tset;
      int conn;

      do
        conn = accept (socket, NULL, NULL);
      while (conn < 0 && errno == EINTR);
      if (conn < 0)
        error (EXIT_FAILURE, errno, "accept");

      print_payload (conn);
    }

  return 0;
}
