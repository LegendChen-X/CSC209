#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];

  /* The user will type in a user name on one line followed by a password 
     on the next.
     DO NOT add any prompts.  The only output of this program will be one 
	 of the messages defined above.
   */

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
    
    int fd[2];
    if(pipe(fd)==-1)
    {
        perror("pipe");
    }
    
    int child_ret;
    pid_t pid_1,pid_2;
    
    pid_1 = fork();
    if(pid_1>0)
    {
        if((close(fd[0])) == -1) perror("close");
        
        if(write(fd[1],user_id,10)==-1) perror("write to pipe");
        if(write(fd[1],password,10)==-1) perror("write to pipe");
        
        close(fd[1]);
        
        if((pid_2=wait(&child_ret))==-1) exit(1);
        
        if(WIFEXITED(child_ret))
        {
            if(WEXITSTATUS(child_ret)==0) printf("%s\n",SUCCESS);
            else if(WEXITSTATUS(child_ret)==2) printf("%s\n",INVALID);
            else if(WEXITSTATUS(child_ret)==3) printf("%s\n",NO_USER);
            else exit(1);
        }
    }
    
    else if(pid_1==0)
    {
        if((dup2(fd[0], fileno(stdin))) == -1)
        {
        perror("dup2");
        exit(1);
        }
        
        if((close(fd[1])) == -1) perror("close");
        if((close(fd[0])) == -1) perror("close");
        execl("./validate","validate",(char *) 0);
    
    }
    else
    {
        perror("fork");
        exit(0);
    }

  return 0;
}
