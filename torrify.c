#include <stdio.h>
#include <unistd.h>

extern int execvpe(const char*, char*const[], char*const[]);

int main(int argc, char *argv[]) {
  char *env[] = { "LD_PRELOAD=./libtorrify.so", NULL };

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <command>\n", argv[0]);
    return 1;
  }

  return execvpe(argv[1], argv + 1, env);
}
