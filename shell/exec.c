#include "shtypes.h"
#include "syscalls.h"
#include "io.h"

char * historyList[10];
int histPos = 0;

int handle_builtin(struct cmd *c) /* c is the pointer to a cmd struct */
{
  /* already written : make sure c parsed properly and if not return 0 */
  if (!c)
    return 0;
  
  if(histPos == 9){
    for(int i =0; i<9; i++){
      historyList[i] = historyList[i+1];
      //printf("%s", historyList[i[0]]);
    }
    historyList[9] = 0;
    historyList[histPos] = *c->exec.argv;
  }
  if(histPos < 9){
    printf("%s", *c->exec.argv);
    printf("\n");
    historyList[histPos] = *c->exec.argv;
    histPos++;
  }


  /* already written : tp is the type of the command, like Redir, background, 
  exec, list, or pipe (shtypes.h) */
  switch (c->tp) /* tp is the field out of the struct that we are pointing to (using the ->) */
  {
  /* Note : only if c is an exec then it could be a built-in */
  case EXEC: // Note : EXEC is an enum (shtypes.h)
    /* echo command already written */
    /* if comparing c->exec.argv[0] and "echo" gives 0 then they are 
    equal and you loop over the other arguments and print them out */
    if (strcmp(c->exec.argv[0], "echo") == 0) 
    {
      for (int i = 1; ; i++)
        if (c->exec.argv[i])
          printf("%s ", c->exec.argv[i]);
        else
          break;
      printf("\n");
      return 1;
	  }
    /* exit command already written */ 
    else if (strcmp(c->exec.argv[0], "exit") == 0) 
    {
		  int code = 0;
		  if(c->exec.argv[1]) {
			code = atoi(c->exec.argv[1]);
		}
		exit(code); /* kills your process and returns code */
    } 
    // TODO for students: implement cd
    else if(strcmp(c->exec.argv[0], "cd") == 0){
      chdir(c->exec.argv[1]);
      return 1;
    }
    // TODO for students: implement pwd
    else if (strcmp(c->exec.argv[0], "pwd") == 0){ 
      char * pwd = malloc(1023); 
      getcwd(pwd, 1023);
      printf("%s", pwd);
      return 1;
    }
    // TODO for students: implement jobs
    else if (strcmp(c->exec.argv[0], "jobs") == 0){
      
      return 1;
    }
    // TODO for students: implement history
    else if(strcmp(c->exec.argv[0], "history") == 0){
      for(int i = 0; i<histPos;i++){
        printf("%s", historyList[i]);
      }
    }
    // TODO for students: implement kill
    // TODO for students: implement help
    else if(strcmp(c->exec.argv[0], "help") ==0){
      printf("jobs - provides a numbered list of processes currently execulting in the background\n");
      printf("cd <directory> - changes the directory to the path listed\n");
      printf("pwd - lists the current path you are working in\n");
      printf("history - lists the last 10 commands executed !<number> will execute that command\n");\
      printf("clear - clears the terminal\n");
      printf("help - lists built in commands\n");
      printf("exit - closes the shell and terminates the process\n");
      printf("echo - prints out the rest of the arguements\n");
      printf("kill <mod num> - kills the process with that number which was listed when the jobs command is run");
      return 1;
    }
    // TODO for students: implement clear
    else if (strcmp(c->exec.argv[0], "clear") == 0){
      system("clear");
      return 1;
    }
    else {
      return 0;
    }
  default:
    return 0; //Note : returns 0 when you call handle_builtin in main
  }
}

/* Most of your implementation needs to be in here, so a description of this
 * function is in order:
 *
 * int exec_cmd(struct cmd *c)
 *
 * Executes a command structure. See shtypes.h for the definition of the
 * `struct cmd` structure.
 *
 * The return value is the exit code of a subprocess, if a subprocess was
 * executed and waited for. (Id est, it should be the low byte of the return
 * value from that program's main() function.) If no return code is collected,
 * the return value is 0.
 *
 * For pipes and lists, the return value is the exit code of the rightmost
 * process.
 *
 * The function does not change the assignment of file descriptors of the
 * current process--but it may fork new processes and change their file
 * descriptors. On return, the shell is expected to remain connected to
 * its usual controlling terminal (via stdin, stdout, and stderr).
 *
 * This will not be called for builtins (values for which the function above
 * returns a nonzero value).
 */
int exec_cmd(struct cmd *c)
{
  /* don't try to execute a command if parsing failed. */
  if (!c)
    return -1;

  int child;

  for(int i = 0; c->exec.argv[i] != NULL; i++){
    if(c->exec.argv[i] == "&"){
      printf("TO THE BACKGROUND WITH YOU\n");
      int pid = fork();
      if(pid < 0){
        exit(0);
      }
      if(pid){
        execvp(c->exec.argv[0], c->exec.argv);
      }
    }
  }

  switch (c->tp)
  {
  case EXEC:
	// TODO for students: What will run a command? (Hint: use execvp syscall)
  {
    if((child = fork1())) {
      int code;
      while(wait(&code) != child);
		  //printf("command exited wlsith code %d\n", WEXITSTATUS(code));
    } 
    else {
      exit(execvp(c->exec.argv[0], c->exec.argv));
      
    }
  }
    break;

  case PIPE:
	/* TODO for students: Run *two* commands, but with the stdout chained to the stdin
	 * of the next command. How? Use the syscall pipe: */
	{
		int pipefds[2];
		pipe(pipefds);
		/* At this point, pipefds[0] is a read-only FD, and pipefds[1] is a
		 * write-only FD. Make *absolutely sure* you close all instances of
		 * pipefds[1] in processes that aren't using it, or the reading child
		 * will never know when it needs to terminate! */
	}
    break;

  case LIST:
	/* The LIST case is already written */
	exec_cmd(c->list.first);
	return exec_cmd(c->list.next);
    break;

  case REDIR:
    //You need to use the fields in the c->redir struct here; fd is the file 
    //descriptor you are going to redirect with (STDIN or STDOUR), the path 
    //is the file you need to open for redirecting, the mode is how to open
    //the file you are going to use, and the cmd field is what command to run
    if((child = fork1())) {
      int code;
      //while(wait(&code) != child);
		  } 
    else {
      c->redir.fd == 0 ? freopen(c->redir.path, "r", stdin) : freopen(c->redir.path, "w", stdout);
      exec_cmd(c->redir.cmd);
    }
    break;

  case BACK:
	/* TODO for students: This should be easy--what's something you can remove from EXEC to have
	 * this function return before the child exits? */
    break;

  default:
    fatal("BUG: exec_cmd unknown command type\n");
  }
}
