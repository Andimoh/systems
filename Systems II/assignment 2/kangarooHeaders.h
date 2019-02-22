/*--------------------------------------------------------------------------*
 *----									----*
 *----		kangarooHeaders.h					----*
 *----									----*
 *----	    This file declares constants and objects common to the	----*
 *----	Unix-signalling kangaroo simulation programs.			----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1.0		2017 April 17		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/
 
//
//Common include sequence:
//
#include	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<signal.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/wait.h>


//
//		Declaration of constants:
//
#define		LINE_LEN		16
#define		MALL_PROG_NAME		"mall"
#define		JOEY_PROG_NAME		"joey"

#define		MAMAS_NUM_JOEYS_CMD_LINE_INDEX	((int)1)
#define		MAMAS_RAND_SEED_CMD_LINE_INDEX	((int)2)

#define		MALLS_NUM_JOEYS_CMD_LINE_INDEX	((int)1)
#define		MALLS_RAND_SEED_CMD_LINE_INDEX	((int)2)
#define		MIN_MALL_CMD_LINE_PARAMS	\
					(MALLS_RAND_SEED_CMD_LINE_INDEX + 1)

#define		JOEYS_INDEX_CMD_LINE_INDEX	((int)1)
#define		JOEYS_MALL_PID_CMD_LINE_INDEX	((int)2)
#define		JOEYS_RAND_SEED_CMD_LINE_INDEX	((int)3)
#define		MIN_JOEY_CMD_LINE_PARAMS	\
					(JOEYS_RAND_SEED_CMD_LINE_INDEX + 1)

#define		MIN_NUM_LEGAL_JOEYS		((int)1)

#define		NUM_TURNS_MUST_SUCCESSFULLY_MAKE ((int)8)

#define		LEFT_TURN_SIGNAL		SIGUSR1
#define		RIGHT_TURN_SIGNAL		SIGUSR2

#define		WRONG_TURN_SIGNAL		SIGUSR1
#define		CORRECT_TURN_SIGNAL		SIGUSR2