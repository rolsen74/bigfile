
/*
 *
 * BigFile (c) 2023 by Rene W. Olsen < renewolsen @ gmail . com >
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#ifdef win32
void WinMain( void )
{

}
#endif

int stop = 0;

static void myTimeString( char *buffer, int secs )
{
char yearbuffer[32];
char daysbuffer[32];
char hourbuffer[32];
char minbuffer[32];
char secbuffer[32];
int years;
int days;
int hours;
int mins;

	years = secs / ( 60 * 60 * 24 * 365 );

	secs -= ( 60 * 60 * 24 * 365 * years );

	days = secs / ( 60 * 60 * 24 );

	secs -= ( 60 * 60 * 24 * days );

	hours = secs / ( 60 * 60 );

	secs -= ( 60 * 60 * hours );

	mins = secs / ( 60 );

	secs -= ( 60 * mins );


	if ( years )
	{
		sprintf( yearbuffer, "%dy ", years );
	}
	else
	{
		yearbuffer[0] = 0;
	}

	if ( days )
	{
		sprintf( daysbuffer, "%dd ", days );
	}
	else
	{
		daysbuffer[0] = 0;
	}

	if ( hours )
	{
		sprintf( hourbuffer, "%dh ", hours );
	}
	else
	{
		hourbuffer[0] = 0;
	}

	if ( mins )
	{
		sprintf( minbuffer, "%dm ", mins );
	}
	else
	{
		minbuffer[0] = 0;
	}

	if (( secs ) || (( days == 0 ) && ( hours == 0 ) && ( mins == 0 )))
	{
		sprintf( secbuffer, "%ds", secs );
	}
	else
	{
		secbuffer[0] = 0;
	}

	sprintf( buffer, "%s%s%s%s%s", yearbuffer, daysbuffer, hourbuffer, minbuffer, secbuffer );
}

static void intHandler( int dummy __attribute__ ((unused)) )
{
	stop = 1;
}

int main( void )
{
long long fsize;
time_t starttime;
time_t endtime;
char filename[ L_tmpnam ];
char mytime[64];
char *mem;
FILE *fp;
int size;
int pos;

	starttime = time( NULL );

	signal( SIGINT, intHandler );

	printf( "\n" );
	printf( "BigFile (c) 2023 by Rene W. Olsen\n" );
	printf( "All rights reserved.\n" );
	printf( "\n" );
	printf( "Generates a very big file with random data\n" );
	printf( "\n" );
	printf( "It only stops when disk is full\n" );
	printf( "or maximum filesize has been reached\n" );
	printf( "or user presses ctrl+c\n" );
	printf( "\n" );

	// -- Generate Random data block (17mb)

	size = 1024*1024*17;

	mem = malloc( size );

	if ( ! mem )
	{
		printf( "Error allocating memory\n" );
		return( 1 );
	}

	srand( time( NULL ) );

	for( int cnt=0 ; cnt<size ; cnt++ )
	{
		mem[cnt] = rand();
	}

	// -- Find free Temp file

	sprintf( filename, "Delete.me" );

	pos = 0;

	while( access( filename, F_OK ) != -1 )
	{
		sprintf( filename, "Deleteme.%03d", pos++ );

		if ( pos >= 1000 )
		{
			printf( "Error creating a temp filename\n" );
			return( 1 );
		}
	}

	// -- Open file

	printf( "Opening temp file '%s'\n", filename );

	fp = fopen( filename, "wb");

	if ( fp == NULL )
	{
		printf( "Error opening file!\n" );
		return( 1 );
	}

	// -- Write data

	printf( "\n" );

	fsize = 0;

	while( ! stop )
	{
		printf( "\rData written : %lld MB", fsize );
		fflush( stdout );

		pos = rand() % 1024*1024*16;

		if ( fwrite( & mem[pos], sizeof(char), 1024*1024, fp ) != 1024*1024 )
		{
			break;
		}

		fsize++;
	}

	while( ! stop )
	{
		pos = rand() % 1024*1024*16;

		if ( fwrite( & mem[pos], sizeof(char), 1, fp ) != 1 )
		{
			break;
		}
	}

	printf( "\n" );

	// Close the file
	fclose( fp );

	printf( "\n" );
	printf( "Deleting temp file\n" );
	printf( "\n" );

	// Delete the file
	if ( remove( filename ) != 0 ) 
	{
		printf( "Error deleting file!\n" );
		return( 1 );
	}

	// --

	endtime = time(NULL);

	myTimeString( mytime, endtime - starttime );

	printf( "Runtime: %s\n", mytime );
	printf( "\n" );

	return 0;
}
