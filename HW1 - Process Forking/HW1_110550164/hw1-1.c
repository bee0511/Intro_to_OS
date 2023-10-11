#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void show(int fork_id){
    printf("Fork %d. I'm the child %d, and my parent is %d.\n",
            fork_id, getpid(), getppid());
}

int main(){
    printf("Main Process ID: %d\n\n", getpid());

    pid_t pid;
    pid = fork(); // Fork 1
    if (pid == 0){
        show(1);
        pid = fork(); // Fork 2
        
        if (pid == 0){
            show(2);
        }
        else if (pid > 0){
            wait(NULL);
            pid = fork(); // Fork 3
            
            if (pid == 0){
                show(3);
                pid = fork(); // Fork 4
                
                if(pid == 0){
                    show(4);
                }
                else if(pid > 0){
                    wait(NULL);
                }
            }
            else if (pid > 0){
                wait(NULL);
                pid = fork(); // Fork 4
                if(pid == 0){
                    show(4);
                }
                else if (pid > 0){
                    wait(NULL);
                }
            }
        }
    }
    else if (pid > 0) {

        wait(NULL);
        pid = fork(); // Fork 2
        
        if (pid == 0){
            show(2);
        }
        else if(pid > 0){
            wait(NULL);
            pid = fork(); // Fork 3
            
            if (pid == 0){
                show(3);
                pid = fork(); //Fork 4

                if (pid == 0){
                    show(4);
                }
                else if (pid > 0){
                    wait(NULL);
                }
            }
            else if (pid > 0){
                pid = fork(); // Fork 4
                if(pid == 0){
                    show(4);
                }
                else if (pid > 0){
                    wait(NULL);
                }
            }
        }
    }
    return 0;
}
