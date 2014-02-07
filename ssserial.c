#include "ssserial.h"

struct serial* SerialCreate(int filedescriptorid)
{
   //PROTOTIPOS
  int serialConfigDefault(struct serial* ser);
  int serialConfignew(struct serial* ser,unsigned int c_cflag,unsigned int c_iflag,unsigned int c_oflag,unsigned int c_lflag);
  int serialSetold(struct serial* ser);  
  int serialSetnew(struct serial* ser);
  int serialGetold(struct serial* ser);
  int serialGetnew(struct serial* ser);
  ssize_t serialWrite(struct serial* ser, const void* buf, size_t count);
  ssize_t serialRead(struct serial* ser, void* buf, size_t count);
  int serialIOflush(struct serial* ser);
  int serialRequest(struct serial* ser, int request, char* argp);
  int serialSetOwner(struct serial* ser, pid_t owner);
  //ALLOCAÇÂO MEMORIA PARA CLASS
  struct serial* ser;
  ser=(struct serial*)calloc(1,sizeof(struct serial));
  if(ser == NULL){
	perror("SerialCreate at calloc");
  }
  //variables
  ser->c=0;
  errno=0;
  ser->id=filedescriptorid;
  sprintf(ser->errcode,"None errno: %d\n", errno);
  //Direccionar apontadores para PROTOTIPOS
  ser->ConfigDefault=serialConfigDefault;
  ser->Confignew=serialConfignew;
  ser->Setold=serialSetold;
  ser->Setnew=serialSetnew;
  ser->Getold=serialGetold;
  ser->Getnew=serialGetnew;
  ser->Write=serialWrite;
  ser->Read=serialRead;
  ser->IOflush=serialIOflush;
  ser->Request=serialRequest;
  ser->SetOwner=serialSetOwner;
  //save old configuration
  if(tcgetattr(ser->id, &ser->oldtio)<0){
    perror("SerialCreate at tcgetattr");
    sprintf(ser->errcode,"SerialCreate at tcgetattr errno: %d\n", errno);
  }
  bzero(&ser->newtio, sizeof(ser->newtio));
  return ser;
}

int serialConfigDefault(struct serial* ser)
{
  /************DEFAULT*****************/
  ser->newtio.c_cflag = B9600 | CS8 | CREAD | CLOCAL; //default 3261-1024
  ser->newtio.c_iflag = ICRNL | IXON; //default 1280
  ser->newtio.c_oflag = OPOST | ONLCR; //default 5
  //newtio.c_lflag = ISIG | ICANON | ECHO | ECHOE| ECHOK | ECHOCTL | ECHOKE | IEXTEN; //default 35387 //
  //newtio.c_cflag|=(CLOCAL | CREAD);
  //newtio.c_iflag|=(INPCK | ISTRIP);//raw input
  //newtio.c_oflag&=~OPOST;//raw output
  ser->newtio.c_lflag&=~(ICANON | ECHO | ECHOE | ISIG);
  /************************************/
  ser->newtio.c_cc[VINTR] = 3; // Ctrl-c 0 default 3
  ser->newtio.c_cc[VQUIT] = 28; // Ctrl-\ 0 default 28
  ser->newtio.c_cc[VERASE] = 127; // del 0 default 127
  ser->newtio.c_cc[VKILL] = 21; // @ 0 default 21
  ser->newtio.c_cc[VEOF] = 4; // Ctrl-d 4 default 4
  ser->newtio.c_cc[VTIME] = 0; // inter-character timer unused 0 default 0
  ser->newtio.c_cc[VMIN] = 1; // blocking read until 1 character arrives 1 default 1
  ser->newtio.c_cc[VSWTC] = 0; // '\0' 0 default 0
  ser->newtio.c_cc[VSTART] = 17; // Ctrl-q 0 default 17
  ser->newtio.c_cc[VSTOP] = 19; // Ctrl-s 0 default 19
  ser->newtio.c_cc[VSUSP] = 26; // Ctrl-z 0 default 26
  ser->newtio.c_cc[VEOL] = 0; // '\0' 0 default 0
  ser->newtio.c_cc[VREPRINT] = 18; // Ctrl-r 0 default 18
  ser->newtio.c_cc[VDISCARD] = 15; // Ctrl-u 0 default 15
  ser->newtio.c_cc[VWERASE] = 23; // Ctrl-w 0 default 23
  ser->newtio.c_cc[VLNEXT] = 22; // Ctrl-v 0 default 22
  ser->newtio.c_cc[VEOL2] = 0; // '\0' 0 default 0
  /************************************/
  return 0;
}

int serialConfignew(struct serial* ser, unsigned int c_cflag, unsigned int c_iflag, unsigned int c_oflag, unsigned int c_lflag)
{
  ser->newtio.c_iflag=c_iflag;
  ser->newtio.c_oflag=c_oflag;
  ser->newtio.c_cflag=c_cflag;
  ser->newtio.c_lflag=c_lflag;
  /************************************/
  ser->newtio.c_cc[VINTR] = 3; // Ctrl-c 0 default 3
  ser->newtio.c_cc[VQUIT] = 0; // Ctrl-\ 0 default 28
  ser->newtio.c_cc[VERASE] = 0; // del 0 default 127
  ser->newtio.c_cc[VKILL] = 0; // @ 0 default 21
  ser->newtio.c_cc[VEOF] = 0; // Ctrl-d 0 default 4
  ser->newtio.c_cc[VTIME] = 0; // inter-character timer unused 0 default 0
  ser->newtio.c_cc[VMIN] = 1; // blocking read until 1 character arrives 1 default 1
  ser->newtio.c_cc[VSWTC] = 0; // '\0' 0 default 0
  ser->newtio.c_cc[VSTART] = 0; // Ctrl-q 0 default 17
  ser->newtio.c_cc[VSTOP] = 0; // Ctrl-s 0 default 19
  ser->newtio.c_cc[VSUSP] = 0; // Ctrl-z 0 default 26
  ser->newtio.c_cc[VEOL] = 0; // '\0' 0 default 0
  ser->newtio.c_cc[VREPRINT] = 0; // Ctrl-r 0 default 18
  ser->newtio.c_cc[VDISCARD] = 0; // Ctrl-u 0 default 15
  ser->newtio.c_cc[VWERASE] = 0; // Ctrl-w 0 default 23
  ser->newtio.c_cc[VLNEXT] = 0; // Ctrl-v 0 default 22
  ser->newtio.c_cc[VEOL2] = 0; // '\0' 0 default 0
  /************************************/
  return 0;	
}

int serialSetold(struct serial* ser)
{
  int RETURN;
  //now clean the modem line and activate the settings for the port
  if(tcflush(ser->id, TCIFLUSH) != 0){
    perror("Setold at tcflush");
    sprintf(ser->errcode,"Setold at tcflush errno: %d\n", errno);
    RETURN=-1;
  }else
    RETURN=0;
  if(tcsetattr(ser->id, TCSANOW, &ser->oldtio) != 0){
    perror("Setold at tcsetattr");
    sprintf(ser->errcode,"Setold at tcsetattr errno: %d\n", errno);
    RETURN=-1;
  }else{
    RETURN=0;
  }
  return RETURN;
}

int serialSetnew(struct serial* ser)
{
  int RETURN;
  //now clean the modem line and activate the settings for the port
  if(tcflush(ser->id, TCIFLUSH) != 0){
	perror("Setnew at tcflush");
    sprintf(ser->errcode,"Setnew at tcflush errno: %d\n", errno);
    RETURN=-1;
  }
  if(tcsetattr(ser->id, TCSANOW, &ser->newtio) != 0){
    perror("Setnew at tcsetattr");
    sprintf(ser->errcode,"Setnew at tcsetattr errno: %d\n", errno);
    if(tcsetattr(ser->id, TCSANOW, &ser->oldtio) != 0){
    perror("Setnew at tcsetattr");
    sprintf(ser->errcode,"Setnew at tcsetattr errno: %d\n", errno);
    }
    RETURN=-1;
  }else{
    RETURN=0;
  }
  return RETURN;
}

int serialGetold(struct serial* ser)
{
  printf("c_iflag %u\n",ser->oldtio.c_iflag);
  printf("c_olag %u\n",ser->oldtio.c_oflag);
  printf("c_cflag %u\n",ser->oldtio.c_cflag);
  printf("c_lflag %u\n",ser->oldtio.c_lflag);
  printf("c_cc[0] %u\n",ser->oldtio.c_cc[0]);
  printf("c_cc[1] %u\n",ser->oldtio.c_cc[1]);
  printf("c_cc[2] %u\n",ser->oldtio.c_cc[2]);
  printf("c_cc[3] %u\n",ser->oldtio.c_cc[3]);
  printf("c_cc[4] %u\n",ser->oldtio.c_cc[4]);
  printf("c_cc[5] %u\n",ser->oldtio.c_cc[5]);
  printf("c_cc[6] %u\n",ser->oldtio.c_cc[6]);
  printf("c_cc[7] %u\n",ser->oldtio.c_cc[7]);
  printf("c_cc[8] %u\n",ser->oldtio.c_cc[8]);
  printf("c_cc[9] %u\n",ser->oldtio.c_cc[9]);
  printf("c_cc[10] %u\n",ser->oldtio.c_cc[10]);
  printf("c_cc[11] %u\n",ser->oldtio.c_cc[11]);
  printf("c_cc[12] %u\n",ser->oldtio.c_cc[12]);
  printf("c_cc[13] %u\n",ser->oldtio.c_cc[13]);
  printf("c_cc[14] %u\n",ser->oldtio.c_cc[14]);
  printf("c_cc[15] %u\n",ser->oldtio.c_cc[15]);
  printf("c_cc[16] %u\n",ser->oldtio.c_cc[16]);
  return 0;	
}

int serialGetnew(struct serial* ser)
{
  printf("c_iflag %u\n",ser->newtio.c_iflag);
  printf("c_olag %u\n",ser->newtio.c_oflag);
  printf("c_cflag %u\n",ser->newtio.c_cflag);
  printf("c_lflag %u\n",ser->newtio.c_lflag);
  printf("c_cc[0] %u\n",ser->newtio.c_cc[0]);
  printf("c_cc[1] %u\n",ser->newtio.c_cc[1]);
  printf("c_cc[2] %u\n",ser->newtio.c_cc[2]);
  printf("c_cc[3] %u\n",ser->newtio.c_cc[3]);
  printf("c_cc[4] %u\n",ser->newtio.c_cc[4]);
  printf("c_cc[5] %u\n",ser->newtio.c_cc[5]);
  printf("c_cc[6] %u\n",ser->newtio.c_cc[6]);
  printf("c_cc[7] %u\n",ser->newtio.c_cc[7]);
  printf("c_cc[8] %u\n",ser->newtio.c_cc[8]);
  printf("c_cc[9] %u\n",ser->newtio.c_cc[9]);
  printf("c_cc[10] %u\n",ser->newtio.c_cc[10]);
  printf("c_cc[11] %u\n",ser->newtio.c_cc[11]);
  printf("c_cc[12] %u\n",ser->newtio.c_cc[12]);
  printf("c_cc[13] %u\n",ser->newtio.c_cc[13]);
  printf("c_cc[14] %u\n",ser->newtio.c_cc[14]);
  printf("c_cc[15] %u\n",ser->newtio.c_cc[15]);
  printf("c_cc[16] %u\n",ser->newtio.c_cc[16]);
  return 0;	
}

ssize_t serialWrite(struct serial* ser, const void* buf, size_t count)
{
  ssize_t RETURN; 
  RETURN=write(ser->id,buf,count);
  if(RETURN<0){
    perror("Write at write");
    sprintf(ser->errcode,"Write ar write errno: %d\n", errno);
    RETURN=-1;
  }  
  return RETURN;
}

ssize_t serialRead(struct serial* ser, void* buf, size_t count)
{
  ssize_t RETURN=1; 
  RETURN=read(ser->id,buf,count);
  if(RETURN<0){
    perror("Read at read");
    sprintf(ser->errcode,"Read at read errno: %d\n", errno);
    RETURN=-1;
  }	
  return RETURN;
}

int serialIOflush(struct serial* ser)
{
  int RETURN;
  if(tcflush(ser->id, TCIOFLUSH) != 0){
    perror("IOflush at tcflush");
    sprintf(ser->errcode,"IOflush at tcflush errno: %d\n", errno);
    RETURN=-1;
  }else
    RETURN=0;
  return RETURN;
}

int serialRequest(struct serial* ser, int request, char* argp)
{
  int RETURN;
  ser->argp=argp;
  RETURN=ioctl(ser->id,request,ser->argp);
  if(RETURN<0){
    perror("Request at ioctl");
    sprintf(ser->errcode,"Request at ioctl errno: %d\n", errno);
  }
  return RETURN;
}

int serialSetOwner(struct serial* ser, pid_t owner){
  int RETURN;
  RETURN=fcntl(ser->id, F_SETOWN, owner);
  if (RETURN < 0){
    perror("SetOwner at Unable to set process to owner");
  }
  return RETURN;
}

