#include "xdockerinit_in.h"

int main( int argc , char *argv[] ) {
	struct xdockerInitEnvironment	*env = NULL ;
	int				i ;
	char				*p = NULL ;
	
	int				nret = 0 ;
	
	SetLogcFile( "/xdockerinit.log" );
	SetLogcLevel( LOGCLEVEL_INFO );
	
	INFOLOGC( "--- xdockerinit begin ---" )
	
	env = (struct xdockerInitEnvironment *)malloc( sizeof(struct xdockerInitEnvironment) ) ;
	if( env == NULL ) {
		E( "*** ERROR : malloc failed , errno[%d]\n" , errno )
		return 1;
	}
	memset( env , 0x00 , sizeof(struct xdockerInitEnvironment) );
	
	for( i = 1 ; i < argc ; i++ ) {
		if( STRCMP( argv[i] , == , "-v" ) ) {
			E( "xdockerinit v%s build %s %s\n" , _XDOCKER_VERSION , __DATE__ , __TIME__ )
			free( env );
			exit(0);
		}
		else if( STRCMP( argv[i] , == , "--container" ) && i + 1 < argc ) {
			env->cmd_para.__container_id = argv[i+1] ;
			i++;
		}
		else if( STRCMP( argv[i] , == , "--single" ) ) {
			env->cmd_para.__single = argv[i] ;
		}
		else {
			E( "*** ERROR : invalid parameter '%s'\n" , argv[i] )
			return -7;
		}
	}
	if( env->cmd_para.__container_id == NULL ) {
		E( "*** ERROR : '--container' need for running\n" )
		return -7;
	}
	
	p = getenv( "XDOCKER_ALIVE_PIPE" ) ;
	if( p == NULL ) {
		E( "*** ERROR : please run me by xdocker\n" )
		return 1;
	}
	else {
		I( "getenv(\"XDOCKER_ALIVE_PIPE\")[%s]\n" , p )
	}
	env->alive_pipe_0 = atoi(p) ;
	
	nret = server( env ) ;
	
	free( env );
	
	INFOLOGC( "--- xdockerinit end ---" )
	
	return -nret;
}
