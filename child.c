#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>


#include "syscall.h"

int eye2eh(int i, char *buffer, int buffersize, int base);

int main(int argc, char *argv[]) {
    while (1) {
        WRITESTRING("awake in ");
        WRITEINT(getpid(), 6);
        WRITESTRING("\n");
        assert (sleep(1) == 0);
    }
    return 0;
}