#include <stdio.h>
int main( void ) {
	int pid = fork();
	if ( pid == 0 ) {
		execlp("./sequential_min_max",3,3,NULL);
	}

	/* Put the parent to sleep for 2 seconds--let the child finished executing */
	wait( 2 );

	printf( "Finished executing the parent process\n"
	        " - the child won't get here--you will only see this once\n" );

	return 0;
}