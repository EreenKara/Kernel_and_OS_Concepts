/**
 * Figure 3.34
 */

  #include <stdio.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>

  int main()
  {
    pid_t pid, pid1;

    /* fork a child process */
    pid = fork();
    if (pid < 0) { 
      /* error occurred */ 
      fprintf(stderr, "Fork Failed"); 
      return 1;
    }
    else if (pid == 0) { /* child process */
      pid1 = getpid();
      printf("child: pid = %d\n",pid); /* A */ // 0
      printf("child: pid1 = %d\n",pid1); /* B */  // kendi pid'si yani cocuk pid 
    }
    else { /* parent process */
      pid1 = getpid();
      printf("parent: pid = %d\n",pid); /* C */ //  cocuk pid
      printf("parent: pid1 = %d\n",pid1); /* D */ // kendi pid'si yani parent pid.
      wait(NULL);
    }

    return 0;
  }