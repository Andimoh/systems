/*--------------------------------------------------------------------------*
 *----									----*
 *----		mall.c							----*
 *----									----*
 *----	    This program implements the mall process, which gives the 	----*
 *----	joeys processes feedback on how to leave.			----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1.0		2017 April 17		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

//
//		Header inclusion:
//
#include	"kangarooHeaders.h"


//
//		Definition of global vars:
//

//  PURPOSE:  To hold the number of joeys the user requested to make.
int		numJoeys		= MIN_NUM_LEGAL_JOEYS - 1;

//  PURPOSE:  To tell if the mama is still looking for joeys.
int		shouldStillRun		= 1;

//  PURPOSE:  To encode the unique sequence of left and right turns each joey
//	make to leave the mall.
//	0 bit = left
//	1 bit = right
//	Bits read from least to most significant.
int*		joeysPaths		= NULL;

//  PURPOSE:  To keep track of the number of successful turns each joey has
//	already made.
int*		joeysNumSuccessfulTurns	= NULL;

//  PURPOSE:  To hold the process ids of the joey processes.
pid_t*		joeysPids		= NULL;


//
//		Definition of global fncs:
//

//  PURPOSE:  To initialize most global structures given the 'argc' command
//	line parameters given in 'argv[]'.  No return value.
void		initializeMostGlobals	(int		argc,
                                     char*		argv[]
                                     )
{
    //  I.  Application validity check:
    if  (argc < MIN_MALL_CMD_LINE_PARAMS)
    {
        fprintf(stderr,"Usage: %s <numJoeys> <randSeed>\n",MALL_PROG_NAME);
        exit(EXIT_FAILURE);
    }
    
    //  II.  Initialize globals:
    //  II.A.  Initialize 'numJoeys':
    numJoeys	=  strtol(argv[MALLS_NUM_JOEYS_CMD_LINE_INDEX],NULL,10); // <-- REPLACE with code that gets integer from 'argv[MALLS_NUM_JOEYS_CMD_LINE_INDEX]'
    
    if  (numJoeys < MIN_NUM_LEGAL_JOEYS)
    {
        fprintf(stderr,"Illegal number of joeys.\n");
        exit(EXIT_FAILURE);
    }
    
    //  II.B.  Initialize random number seed:
    int	randNumSeed	=  strtol(argv[MALLS_RAND_SEED_CMD_LINE_INDEX],NULL,10); // <-- REPLACE with code that gets integer from 'argv[MALLS_RAND_SEED_CMD_LINE_INDEX]'
    
    srand(randNumSeed);
    
    //  II.C.  Initialize 'joeysPaths' and 'joeysNumSuccessfulTurns':
    joeysNumSuccessfulTurns	= (int*)calloc(sizeof(int),numJoeys);
    joeysPaths			= (int*)calloc(sizeof(int),numJoeys);
    joeysPids			= (pid_t*)calloc(sizeof(pid_t),numJoeys);
    
    int	numChoices		= (1 << NUM_TURNS_MUST_SUCCESSFULLY_MAKE);
    int	joey;
    
    for  (joey = 0;  joey < numJoeys;  joey++)
        joeysPaths[joey]	= rand() % numChoices;
    
    //  III.  Finished:
}


//  PURPOSE:  To return the index of the joey process 'joeyPid' in array
//  	'joeysPids[]'.
int		getJoeyIndex		(int		joeyPid
                             )
{
    //  I.  Application validity check:
    
    //  II.  Find index of/for 'joeyPid':
    int	joey;
    
    //  II.A.  Look for 'joeyPid' in 'joeysPids[]':
    for  (joey = 0;  joey < numJoeys;  joey++)
        if  ( (joeysPids[joey] == joeyPid)  ||  (joeysPids[joey] == 0) )
            break;
    
    //  II.B.  Complain if run out of array positions:
    if  (joey >= numJoeys)
    {
        fprintf(stderr,
                "More joey Pids that number of joeys given on command line!\n"
                );
        exit(EXIT_FAILURE);
    }
    
    //  II.C.  Record 'joeyPid' if not currently in 'joeysPids[]':
    if  (joeysPids[joey] != joeyPid)
        joeysPids[joey] = joeyPid;
    
    //  III.  Finished:
    return(joey);
}


//  PURPOSE:  To return '1' ('true') if the joey 'joeyIndex' made the correct
//  	turn with turn signal 'turnSignal', or to return '0' ('false')
//	otherwise.
int		wasJoeysTurnCorrect		(int	joeyIndex,
                                 int	turnSignal
                                 )
{
    //  I.  Application validity check:
    
    //  II.  Return value:
    int	didJoeyTurnRight	= (turnSignal == RIGHT_TURN_SIGNAL);
    int	shouldHaveTurnedRight	= (joeysPaths[joeyIndex]		>>
                                   joeysNumSuccessfulTurns[joeyIndex]
                                   )
    & 0x1;
    
    return(didJoeyTurnRight == shouldHaveTurnedRight);
}



void     signal_handler (int sig, siginfo_t* infoPtr, void* dataPtr)
{
    
    pid_t joeyProcessId = infoPtr-> si_pid;
    
    
    int joeyIndex = getJoeyIndex(joeyProcessId);
    
    if ( wasJoeysTurnCorrect(joeyIndex,sig) == 0 ){
    
        
      printf("Mall: Joey # %d made the wrong turn!  (%s) \n", joeyIndex, (sig == LEFT_TURN_SIGNAL) ? "left" : "right");
        
        kill(joeyProcessId, WRONG_TURN_SIGNAL);
    }
    
    else if ( wasJoeysTurnCorrect(joeyIndex,sig) == 1){
        
      printf("Mall: Joey # %d turned correctly!  (%s) \n", joeyIndex, (sig == LEFT_TURN_SIGNAL) ? "left" : "right");
        

	//  kill(joeyProcessId, CORRECT_TURN_SIGNAL);
        
        joeysNumSuccessfulTurns[joeyIndex]++;
        


	if ( joeysNumSuccessfulTurns[joeyIndex] >= NUM_TURNS_MUST_SUCCESSFULLY_MAKE ){

	  //  joeysNumSuccessfulTurns[joeyIndex]++;

	  printf("Mall: Joey # %d Got out of the mall and returend to its mama!\n", joeyIndex);
	  kill(joeyProcessId, SIGINT);
	}
        else
	  {
            printf("Mall: Joey # %d must make %d more correct turns.\n", joeyIndex , NUM_TURNS_MUST_SUCCESSFULLY_MAKE - joeysNumSuccessfulTurns[joeyIndex] );
            kill(joeyProcessId, CORRECT_TURN_SIGNAL);
	    
	  }
    }
    
    
        
}


//  YOUR CODE HERE to add either two advanced signal handlers, for
//    'LEFT_TURN_SIGNAL' and 'RIGHT_TURN_SIGNAL' respectively, or add one that
//    can handle them both.
//
//  If the joey took the wrong move then it:
//    printf()s a message that tells this.
//    sends WRONG_TURN_SIGNAL back to the joey process.
//  else if the joey took the correct move then it:
//    printf()s a message that tells this.
//    increments joeysNumSuccessfulTurns[joey]
//    If joeysNumSuccessfulTurns[joey] is greater than or equal to NUM_TURNS_MUST_SUCCESSFULLY_MAKE then
//      It printf()s a message that tells that it is out,
//      Sends SIGINT to the joey process.
//    else
//      It printf()s a message telling how many more moves the joey should make
//      Sends CORRECT_TURN_SIGNAL back to the joey process.
//
//  You can get the index of a joey with:
//  int joeyIndex = getJoeyIndex(joeyProcessId)
//
//  You can see if the Joey's turn was correct with:
//  if  ( wasJoeysTurnCorrect(joeyIndex,receivedSignal) )
//    goodMove();
//  else
//    badMove();



//  YOUR CODE HERE to add a SIGINT handler.  It can be simple.
//  It printf()s a message and changes the global state so that the
//  while-loop in main() quits.


void	sigIntHandler	(int sig
                                     )
{
    
    printf("Mall: All the joeys have found their mama!  Now the mall can finally close! \n");  
    shouldStillRun	= 0;
}

//  PURPOSE:  To do the program given the 'argc' command line parameters
//	pointed to by 'argv[]'.  Returns 'EXIT_SUCCESS' to OS when program
//	naturally finishes.
int	 main (int		argc,
           char*	argv[]
                         )
{
    //  I.  Application validity check (done by 'initializeMostGlobals()'):
    
    //  II.  Sheppard joeys out of mall:
    //  II.A.  Initialize:
    initializeMostGlobals(argc,argv);
    // YOUR CODE HERE to install your 'LEFT_TURN_SIGNAL' and 'RIGHT_TURN_SIGNAL' handler(s)
    // YOUR CODE HERE to install your 'SIGINT' handler
 
    /* struct sigaction sa;
    * sa.sa_handler = handler;
    * sigemptyset(&sa.sa_mask);
    * sa.sa_flags = SA_RESTART;  //Restart functions if
    *        interrupted by handler
    * if (sigaction(SIGINT, &sa, NULL) == -1)
    *            Handle error ;
    *           Further code 
} 
*/
    
    struct sigaction	sa;
    
    memset(&sa,'\0',sizeof(struct sigaction));
    sa.sa_flags		= SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction	= signal_handler;
    sigaction(WRONG_TURN_SIGNAL,&sa,NULL);
    
    struct sigaction	saa;

    memset(&saa,'\0',sizeof(struct sigaction));
    saa.sa_flags		= SA_SIGINFO | SA_RESTART;
    saa.sa_sigaction	= signal_handler;
    sigaction(CORRECT_TURN_SIGNAL,&saa,NULL);

    struct sigaction	act;

    memset(&act,'\0',sizeof(struct sigaction));
    act.sa_handler = sigIntHandler;
    act.sa_flags	 = SA_NOCLDSTOP | SA_RESTART;
    sigaction(SIGINT,&act,NULL);

    //  II.B.  Sheppard joeys out of mall:
    while  (shouldStillRun)
        sleep(1);
    
    //  II.C.  Clean up afterward:
    free(joeysPids);
    free(joeysNumSuccessfulTurns);
    free(joeysPaths);
    
    //  III.  Finished:
    return(EXIT_SUCCESS);
}
