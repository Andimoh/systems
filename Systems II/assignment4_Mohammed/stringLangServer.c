/*-------------------------------------------------------------------------*
 *---									---*
 *---		stringLangServer.c					---*
 *---									---*
 *---	    This file defines a C program that gets file-sys commands	---*
 *---	from client via a socket, executes those commands in their own	---*
 *---	threads, and returns the corresponding output back to the	---*
 *---	client.								---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ gcc stringLangServer.c -o stringLangServer -lpthread

//---		Header file inclusion					---//

#include	"clientServer.h"
#include	<pthread.h>	// For pthread_create()


//---		Definition of constants:				---//

#define		STD_OKAY_MSG		"Okay"

#define		STD_ERROR_MSG		"Error doing operation"

#define		STD_BYE_MSG		"Good bye!"

#define		THIS_PROGRAM_NAME	"stringLangServer"

const int	ERROR_FD		= -1;


//---		Definition of global vars:				---//

//  PURPOSE:  To be non-zero for as long as this program should run, or '0'
//	otherwise.


//---		Definition of functions:				---//

//  PURPOSE:  To:
//	(1) create a pipe
//	(2) fork() a child process.  This child process will:
//	  (2a) close() its file descriptor to stdout,
//	  (2b) send its output from the pipe to the close stdout file descriptor
//	  (2c) Run the program STRING_LANG_PROGNAME with cPtr on the cmd line
//	  (2d) Send an error message back to the client on file descriptor 'fd'
//		if the execl() failed.
//	(3) get input from the pipe, put the '\0' char at the end
//	(4) wait() for the child process to end
//	(5) send the input back to the client using file descriptor 'fd'
void		stringLangFile	(int		socketFd,
				 const char*	cPtr
				)
{
  //  I.  Application validity check:

  //  II.  Apply string language file:
  //  YOUR CODE HERE
int Mypipe[2];

if(pipe(Mypipe)<0){
	write(socketFd,STD_ERROR_MSG,sizeof(STD_ERROR_MSG));
}

int pid = fork();
if(pid<0){
  fprintf(stderr,"%s\n","pipe error!");
}
//child
else if(pid==0){

close(1);

dup(Mypipe[1]);

execl(STRING_LANG_PROGNAME,STRING_LANG_PROGNAME,cPtr,NULL);

write(socketFd,STD_ERROR_MSG,sizeof(STD_ERROR_MSG));
exit(EXIT_FAILURE);
return;
}
else if(pid>0){
//parent
char buffer[BUFFER_LEN];
int txt = read(Mypipe[0],buffer,BUFFER_LEN);
buffer[txt]='\0';
wait(&pid);
write(socketFd,buffer,BUFFER_LEN);

}

  //  III.  Finished:
}


//  PURPOSE:  To cast 'vPtr' to the pointer type coming from 'doServer()'
//	that points to two integers.  Then, to use those two integers,
//	one as a file descriptor, the other as a thread number, to fulfill
//	requests coming from the client over the file descriptor socket.
//	Returns 'NULL'.
void*		handleClient	(void*		vPtr
				)
{
  //  I.  Application validity check:

  //  II.  Handle client:
  //  YOUR CODE HERE
  //
  int*     intPtr = (int*) vPtr;
  int 	   fd		= intPtr[0];	// <-- CHANGE THAT 0!
  int	   threadNum	= intPtr[1];	// <-- CHANGE THAT 0!
	

  //  YOUR CODE HERE
  free(intPtr); 
  char buffer[BUFFER_LEN];
  char command;
  char* textPtr;
  int  shouldContinue = 1;

  while(shouldContinue)
{
read(fd,buffer,BUFFER_LEN);
printf("Thread %d received: %s\n",threadNum,buffer);
command = buffer[0];
textPtr = buffer +2;

if(command == QUIT_CMD_CHAR){

write(fd,STD_BYE_MSG,sizeof(STD_BYE_MSG));
shouldContinue=0;
}
else if(command == STRING_LANG_CMD_CHAR){

stringLangFile(fd,textPtr);
}

} 
  //  III.  Finished:
  printf("Thread %d quitting.\n",threadNum);
  return(NULL);
}


//  PURPOSE:  To run the server by 'accept()'-ing client requests from
//	'listenFd' and doing them.
void		doServer	(int		listenFd
				)
{
  //  I.  Application validity check:

  //  II.  Server clients:
  pthread_t		threadId;
  pthread_attr_t	threadAttr;
  int			threadCount	= 0;
  
  int*   iPtr; 

listen(listenFd,5); 
pthread_attr_init(&threadAttr);
  while  (1) {
    //  YOUR CODE HERE

    printf("descriptor\n");
    int  fd = accept(listenFd,NULL,NULL);    
    printf("connectionDescriptor = %d\n",fd);

    if (fd < 0) {
      perror("error on attempt\n");
      exit(EXIT_FAILURE);
    }

    iPtr = (int*)calloc(2,sizeof(int*));
    iPtr[0] = fd;
    threadId = getpid();
    iPtr[1] = threadId;
    printf("In doServer, iPtr[0] = %d, and iPtr[1] = %d \n",iPtr[0], iPtr[1]);
    threadCount++;

    pthread_attr_setdetachstate(&threadAttr,PTHREAD_CREATE_DETACHED);
    pthread_create(&threadId,&threadAttr,handleClient,(void*)iPtr);

  //  YOUR CODE HERE

  //  III.  Finished:
  }

}


//  PURPOSE:  To decide a port number, either from the command line arguments
//	'argc' and 'argv[]', or by asking the user.  Returns port number.
int		getPortNum	(int	argc, 
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Get listening socket:
  int	portNum;

  if  (argc >= 2)
    portNum	= strtol(argv[1],NULL,0);
  else
  {
    char	buffer[BUFFER_LEN];

    printf("Port number to monopolize? ");
    fgets(buffer,BUFFER_LEN,stdin);
    portNum	= strtol(buffer,NULL,0);
  }

  //  III.  Finished:  
  return(portNum);
}


//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int		getServerFileDescriptor
				(int		port
				)
{
  //  I.  Application validity check:

  //  II.  Attempt to get socket file descriptor and bind it to 'port':
  //  II.A.  Create a socket
  int socketDescriptor = socket(AF_INET, // AF_INET domain
			        SOCK_STREAM, // Reliable TCP
			        0);

  if  (socketDescriptor < 0)
  {
    perror(THIS_PROGRAM_NAME);
    return(ERROR_FD);
  }

  //  II.B.  Attempt to bind 'socketDescriptor' to 'port':
  //  II.B.1.  We'll fill in this datastruct
  struct sockaddr_in socketInfo;

  //  II.B.2.  Fill socketInfo with 0's
  memset(&socketInfo,'\0' ,sizeof(socketInfo));

  //  II.B.3.  Use TCP/IP:
  socketInfo.sin_family = AF_INET;

  //  II.B.4.  Tell port in network endian with htons()
  socketInfo.sin_port = htons(port);

  //  II.B.5.  Allow machine to connect to this service
  socketInfo.sin_addr.s_addr = INADDR_ANY;

  //  II.B.6.  Try to bind socket with port and other specifications
  int status = bind(socketDescriptor, // from socket()
		    (struct sockaddr*)&socketInfo,
		    sizeof(socketInfo)
		   );

  if  (status < 0)
  {
    perror(THIS_PROGRAM_NAME);
    return(ERROR_FD);
  }

  //  II.B.6.  Set OS queue length:
  listen(socketDescriptor,5);

  //  III.  Finished:
  return(socketDescriptor);
}


int		main		(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Do server:
  int 	      port	= getPortNum(argc,argv);
  int	      listenFd	= getServerFileDescriptor(port);
  int	      status	= EXIT_FAILURE;

  if  (listenFd >= 0)
  {
    doServer(listenFd);
    close(listenFd);
    status	= EXIT_SUCCESS;
  }

  //  III.  Finished:
  return(status);
}