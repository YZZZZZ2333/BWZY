#include "xdocker_in.h"

int DoAction_spush( struct xdockerEnvironment *env ) {
	char		image[ IMAGES_ID_LEN_MAX + 1 ] ;
	char		version[ PATH_MAX + 1 ] ;
	char		image_file[ PATH_MAX + 1 ] ;
	char		current_path[ PATH_MAX + 1 ] ;
	char		cmd[ 4096 ] ;
	char		srepo[ SREPO_LEN_MAX + 1 ] ;
	char		srepo_file[ PATH_MAX + 1 ] ;

	int		nret = 0 ;
	int		nret2 = 0 ;
	
	/* preprocess input parameters */
	Snprintf( image , sizeof(image) , "%s" , env->cmd_para.__image );
	SplitImageVersion( image , version , sizeof(version) );
	
	Snprintf( image_file , sizeof(image_file) , "%s%s%s.xdockerimage" , image , (version[0]=='\0'?"":":") , version );
	
	Snprintf( env->image_path_base , sizeof(env->image_path_base)-1 , "%s/%s/%s" , env->images_path_base , image , (version[0]?version:"_") );
	nret = access( env->image_path_base , F_OK ) ;
	I1TER1( "*** ERROR : image '%s' not found\n" , env->cmd_para.__image )
	
	memset( srepo , 0x00 , sizeof(srepo) );
	memset( srepo_file , 0x00 , sizeof(srepo_file) );
	nret = ReadFileLine( srepo , sizeof(srepo) , srepo_file , sizeof(srepo_file) , "%s/srepo" , env->xdocker_home ) ;
	I1TER1( "read %s failed\n" , srepo_file )
	EIDTI( "read %s ok\n" , srepo_file )
	
	/* pack and upload image */
	nret = SnprintfAndSystem( cmd , sizeof(cmd) , "ssh %s \"ls ./%s\" >/dev/null 2>/dev/null" , srepo , image_file ) ;
	if( nret == 0 ) {
		E( "*** ERROR : image '%s' exist in srepo\n" , image_file );
		if( ! env->cmd_para.__forcely )
			return nret;
	}
	
	memset( current_path , 0x00 , sizeof(current_path) );
	getcwd( current_path , sizeof(current_path) );
	
	nret = SnprintfAndSystem( cmd , sizeof(cmd) , "cd %s/rlayer/ && tar --force-local -czf %s * && mv %s %s/" , env->image_path_base , image_file , image_file , current_path ) ;
	INTER1( "*** ERROR : SnprintfAndSystem [cd %s/rlayer/ && tar --force-local -czf %s * && mv %s %s/] failed[%d] , errno[%d]\n" , env->image_path_base , image_file , image_file , current_path , nret , errno )
	EIDTI( "system [%s] ok\n" , cmd )
	
	if( env->cmd_para.__debug )
		nret = SnprintfAndSystem( cmd , sizeof(cmd) , "scp ./%s %s:~/" , image_file , srepo ) ;
	else
		nret = SnprintfAndSystem( cmd , sizeof(cmd) , "scp ./%s %s:~/ >/dev/null 2>/dev/null" , image_file , srepo ) ;
	INTEx( {if(nret2==0)nret2=nret;} , "*** ERROR : SnprintfAndSystem [%s] failed[%d] , errno[%d]\n" , cmd , nret , errno )
	EIDTI( "system [%s] ok\n" , cmd )
	
	nret = SnprintfAndSystem( cmd , sizeof(cmd) , "rm %s" , image_file ) ;
	INTER1( "*** ERROR : SnprintfAndSystem [rm %s] failed[%d] , errno[%d]\n" , image_file , nret , errno )
	EIDTI( "system [%s] ok\n" , cmd )
	
	printf( "OK\n" );
	
	if( nret2 )
		return nret2;
	else
		return 0;
}

