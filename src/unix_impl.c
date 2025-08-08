#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <sys/inotify.h>

#include "fswatch.h"

void watch_directory(const char* path_name) {

    // TODO: Support for higher level settings
    uint32_t fd_mask = IN_CREATE;

    int fd = inotify_init1(IN_NONBLOCK);

    if(fd < 0)
    {
        perror("INOTFINY FILE DESCRIPTOR: ");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, path_name, fd_mask);

    if(wd < 0)
    {
        perror("WATCH FILE DESCRIPTOR: ");
        exit(EXIT_FAILURE);
    }

    uint32_t buff_size = sizeof(struct inotify_event) + MAX_NOTIFICATION_BUFF_SZ + 1;

    struct inotify_event buff[buff_size];
    memset(buff, 0, buff_size);

    short nfds = 1;
    struct pollfd pfd = { fd, nfds, 0 };

    int open_fds = nfds;

    for(;;)
    {
        if(usleep(P_INTERVAL_NANOS))
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

        // TODO: Parse the buffer

        // Clean up
        memset(buff, 0, buff_size);
    }

    inotify_rm_watch(fd, wd);

    close(fd);
}