#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/time.h>
#include "fastcommon/logger.h"
#include "fastcommon/shared_func.h"

int main(int argc, char *argv[])
{
#define SEEK_POS   (2 * 1024)
    char *filename;
    int fd;
    int result;
    int n;
    char buf[1024];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    log_init();
    filename = argv[1];
    if (access(filename, F_OK) == 0) {
        logError("file: "__FILE__", line: %d, "
            "file %s already exists", __LINE__, filename);
        return EEXIST;
    }

    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        result = errno != 0 ? errno : EIO;
        logError("file: "__FILE__", line: %d, " \
            "open file %s fail, " \
            "errno: %d, error info: %s", \
            __LINE__, filename, \
            result, STRERROR(result));
        return result;
    }

    if (lseek(fd, SEEK_POS, SEEK_SET) < 0) {
        logError("file: "__FILE__", line: %d, " \
            "lseek file %s fail, " \
            "errno: %d, error info: %s", __LINE__, \
            filename, errno, STRERROR(errno));
        return errno != 0 ? errno : EIO;
    }

    memset(buf, '\n', sizeof(buf));
    if ((n=write(fd, buf, sizeof(buf))) <= 0) {
        logError("file: "__FILE__", line: %d, " \
            "write to file %s fail, " \
            "errno: %d, error info: %s", __LINE__, \
            filename, errno, STRERROR(errno));
        return errno != 0 ? errno : EIO;
    }
    printf("write bytes: %d\n", n);

    if (lseek(fd, 0, SEEK_SET) < 0) {
        logError("file: "__FILE__", line: %d, " \
            "lseek file %s fail, " \
            "errno: %d, error info: %s", __LINE__, \
            filename, errno, STRERROR(errno));
        return errno != 0 ? errno : EIO;
    }

    if ((n=read(fd, buf, sizeof(buf))) <= 0) {
        logError("file: "__FILE__", line: %d, " \
            "read from file %s fail, " \
            "errno: %d, error info: %s", __LINE__, \
            filename, errno, STRERROR(errno));
        return errno != 0 ? errno : EIO;
    }

    printf("read bytes: %d, 0 => 0x%02x, %d => 0x%02x\n",
            n, (unsigned char)buf[0], n - 1, (unsigned char)buf[n - 1]);
    return 0;
}
