#ifndef _SSSERIAL_H
#define _SSSERIAL_H

// fopen perror fread fwrite feof fseek ferror fclose rewind scanf sscanf getchar scanf fscanf
#include <stdio.h>
// calloc free realloc malloc
#include <stdlib.h>
// strcpy strcmp strcat memcmp
#include <string.h>
// termios tcflush
#include <termios.h>
// nanosleep sleep
#include <time.h>
// tcflsuh read write close
#include <unistd.h>
// perror
#include <errno.h>
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio_ext.h>
#include <math.h>

struct serial{
  int c;
  int id;
  char* argp;
  struct termios oldtio, newtio;
  char errcode[64];
  //pointer type prototypes
  int (*ConfigDefault)(struct serial* ser);
  int (*Confignew)(struct serial* ser, unsigned int c_cflag,unsigned int c_iflag, unsigned int c_oflag, unsigned int c_lflag);
  int (*Setold)(struct serial* ser);
  int (*Setnew)(struct serial* ser);
  int (*Getold)(struct serial* ser);
  int (*Getnew)(struct serial* ser);
  ssize_t (*Write)(struct serial* ser, const void* buf, size_t count);
  ssize_t (*Read)(struct serial* ser, void* buf, size_t count);
  int (*IOflush)(struct serial* ser);
  int (*Request)(struct serial* ser, int request, char* argp);
  int (*SetOwner)(struct serial* ser, pid_t owner);
};

typedef struct serial serial;

struct serial* SerialCreate(int filedescriptorid);

#endif

