#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <poll.h>
#include <sys/inotify.h>

#define ARG_FILE_IDX 1
#define PINTERVAL_MS 500
#define PINTERVAL_NANOS 100000

int main(int argc, char** argv)
{
  if(argc < 1)                                       // Missing required argument
    return -1;

  const char* path = argv[ARG_FILE_IDX];

  // TODO: Check if file exists
  // TODO: Check if file is a directory or file to flavour message to user
  printf("Watching file: %s\n", path);

  // TODO: Support for higher level settings
  uint32_t fd_mask = IN_CREATE;

  int fd = inotify_init1(IN_NONBLOCK);

  if(fd < 0)
  {
    perror("INOTFINY FILE DESCRIPTOR: ");
    exit(EXIT_FAILURE);
  }

  int wd = inotify_add_watch(fd, path, fd_mask);

  if(wd < 0)
  {
    perror("WATCH FILE DESCRIPTOR: ");
    exit(EXIT_FAILURE);
  }

  uint32_t NAME_MAX = 512;
  uint32_t buff_size = sizeof(struct inotify_event) + NAME_MAX + 1;

  struct inotify_event buff[buff_size];
  memset(buff, 0, buff_size);

  short nfds = 1;
  struct pollfd pfd = { fd, nfds, 0 };

  int open_fds = nfds;

  for(;;)
  {
    if(usleep(PINTERVAL_NANOS))
    {
      perror("Thread interval: ");
      return errno;
    }

    if(poll(&pfd, open_fds, -1) <= 0)               // No events
      continue;

    if(!(pfd.revents & POLLIN))                     // No POLLIN events
      continue;

    ssize_t s = read(fd, buff, buff_size);

    if(s == -1)
    {
      perror("Read");
      exit(EXIT_FAILURE);
    }

    // TODO: Check mask to report event triggered by file
    fprintf(stdout, "Created a file: '%s';\n", buff[0].name);

    // TODO: Figure out a nice timming to clean up the buffer;
    memset(buff, 0, buff_size);
  }

  inotify_rm_watch(fd, wd);

  close(fd);

  return 0;
}