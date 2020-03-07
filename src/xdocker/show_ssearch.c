
#include "xdocker_in.h"

int DoShow_ssearch( struct CockerEnvironment *env )
{
	char		srepo[ SREPO_LEN_MAX + 1 ] ;
	char		srepo_file[ PATH_MAX + 1 ] ;
	char		cmd[ 4096 ] ;
	FILE		*pp = NULL ;
	char		buf[ 4096 ] ;
	int		count ;
	int		image_size ;
	char		image_size_str[ 32 + 1] ;
	char		image_modify_datetime[ 32 + 1 ] ;
	char		image_id[ IMAGES_ID_LEN_MAX + 1 ] ;
	
	int		nret = 0 ;
	
	if( env->cmd_para.__srepo )
	{
		memset( srepo_file , 0x00 , sizeof(srepo_file) );
		nret = WriteFileLine( env->cmd_para.__srepo , srepo_file , sizeof(srepo_file) , "%s/srepo" , env->xdocker_home ) ;
		I1TER1( "write %s failed\n" , srepo_file )
		EIDTI( "write %s ok\n" , srepo_file )
	}
	
	memset( srepo , 0x00 , sizeof(srepo) );
	memset( srepo_file , 0x00 , sizeof(srepo_file) );
	nret = ReadFileLine( srepo , sizeof(srepo) , srepo_file , sizeof(srepo_file) , "%s/srepo" , env->xdocker_home ) ;
	I1TER1( "read %s failed\n" , srepo_file )
	EIDTI( "read %s ok\n" , srepo_file )
	
	if( env->cmd_para.__match == NULL )
	{
		Snprintf( cmd , sizeof(cmd) , "ssh %s ls -l --full-time \"*.xdockerimage\" 2>/dev/null" , srepo ) ;
	}
	else
	{
		Snprintf( cmd , sizeof(cmd) , "ssh %s ls -l --full-time \"*%s*.xdockerimage\" 2>/dev/null" , srepo , env->cmd_para.__match ) ;
	}
	pp = popen( cmd , "r" ) ;
	IxTER1( (pp==NULL) , "popen [%s] failed , errno[%d]\n" , cmd , errno )
	EIDTI( "popen [%s] ok\n" , cmd )
	
	count = 0 ;
	while(1)
	{
		memset( buf , 0x00 , sizeof(buf) );
		if( fgets( buf , sizeof(buf) , pp ) == NULL )
			break;
		TrimEnter( buf );
		
		memset( image_modify_datetime , 0x00 , sizeof(image_modify_datetime) );
		memset( image_id , 0x00 , sizeof(image_id) );
		sscanf( buf , "%*s%*s%*s%*s%d%s %[^.].%*s%*s%s" , & image_size , image_modify_datetime , image_modify_datetime+10 , image_id );
		image_modify_datetime[10] = 'T' ;
		image_id[strlen(image_id)-sizeof(XDOCKERIMAGE_FILE_EXTNAME)] = '\0' ;
		
		if( image_size > 1024*1024*1024 )
			Snprintf( image_size_str , sizeof(image_size_str) , "%d GB" , image_size / (1024*1024*1024) );
		else if( image_size > 1024*1024 )
			Snprintf( image_size_str , sizeof(image_size_str) , "%d MB" , image_size / (1024*1024) );
		else if( image_size > 1024 )
			Snprintf( image_size_str , sizeof(image_size_str) , "%d KB" , image_size / 1024 );
		else if( image_size >= 0 )
			Snprintf( image_size_str , sizeof(image_size_str) , "%d B" , image_size );
		else
			Snprintf( image_size_str , sizeof(image_size_str) , "(unknow)" );
		
		/* output */
		if( count == 0 )
		{
			printf( "%-45s %-19s %-10s\n" , "image_id" , "modify_datetime" , "size" );
			printf( "----------------------------------------------------------------------\n" );
		}
		
		printf( "%-45s %-19s %s\n" , image_id , image_modify_datetime , image_size_str );
		
		count++;
	}
	
	return 0;
}

