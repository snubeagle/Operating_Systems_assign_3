#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <signal.h>
#include <errno.h>

#include "syscall.h"

int eye2eh(int i, char *buffer, int buffersize, int base);

void sigurg_hdl() {
	WRITESTRING("SIGURG received\n");
}

void sigusr_hdl() {
	WRITESTRING("SIGUSR1 received from child\n");
}

void sigusr2_hdl() {
	WRITESTRING("SIGUSR2 received from child\n");
}

void sigchld_hdl() {
	int chd_stat;
	syscall (waitpid(-1, &chd_stat, 0));
	WRITESTRING("SIGCHLD received Child exitted with status: ");
	if WIFSIGNALED(chd_stat){
		WRITEINT(WTERMSIG(chd_stat), 2);
	}
}

void sigcont_hdl() {
	WRITESTRING("SIGCONT received\n");
}

void undefined() {
	WRITESTRING("Undefined signal received\n");
}

void handler(int action1) {
	//Assistance from Domminic on fc
	void (*fc[])(void) = {&undefined, &undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined, 
								&undefined, &sigusr_hdl, &undefined, &sigusr2_hdl, 
								&undefined, &undefined, &undefined, &undefined,
								&sigchld_hdl, &sigcont_hdl, &undefined, &undefined,
								&undefined, &undefined, &sigurg_hdl, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined, &undefined,
								&undefined, &undefined, &undefined};

	if ((action1 <= 64) & (action1 >= 0)) {
		(*fc[action1])();
	}
	else {
		WRITESTRING("Invalid signal received\n");
	}
}

int main(int argc, char *argv[]){
    int result;
	int i = 0;
	
	struct sigaction action;
	sigemptyset (&action.sa_mask);

	action.sa_handler = handler;
	action.sa_flags = SA_RESTART||SA_NOCLDSTOP;

	syscall (sigaction (SIGUSR1, &action, NULL));
	syscall (sigaction (SIGUSR2, &action, NULL));
	syscall (sigaction (SIGURG, &action, NULL));
	syscall (sigaction (SIGCHLD, &action, NULL));
	syscall (sigaction (SIGCONT, &action, NULL));

    int arrrrrrr = fork();
    
    if (arrrrrrr < 0){
		perror("Fork");
		void exit(int);
	}
	else if (arrrrrrr == 0) {
		assert ((result = execl("./child", "child", (char *)NULL)) > 0);	
		if (result < 0) {
			WRITESTRING("Error executing counter file");
			void exit(int);
		}
	}
    else
		while (i < 5) {
			WRITESTRING("Parent is going to SIGSTOP child\n");
			assert (kill(arrrrrrr, SIGSTOP) == 0);
			syscall (sleep(2));
			WRITESTRING("Parent is going to SIGCONT the child\n");
			assert (kill(arrrrrrr, SIGCONT) == 0);
			syscall (sleep(2));
			i++;
		}

		assert (kill(arrrrrrr, SIGINT) ==0);
		pause();


    return 0;
}