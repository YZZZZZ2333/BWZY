
#include "xdocker_in.h"

int DoAction_rplfile( struct CockerEnvironment *env )
{
	char		template_file[ PATH_MAX + 1 ] ;
	char		instance_file[ PATH_MAX + 1 ] ;
	
	int		nret = 0 ;
	
	SetLogcFile( "/var/xdocker/xdocker.log" );
	SetLogcLevel( LOGCLEVEL_INFO );
	
	/* preprocess input parameters */
	Snprintf( env->container_path_base , sizeof(env->container_path_base)-1 , "%s/%s" , env->containers_path_base , env->cmd_para.__container );
	nret = access( env->container_path_base , F_OK ) ;
	INTER1( "*** ERROR : container '%s' not found\n" , env->cmd_para.__container )
	
	Snprintf( template_file , sizeof(template_file)-1 , "%s/merged%s" , env->container_path_base , env->cmd_para.__template_file );
	nret = access( template_file , F_OK ) ;
	INTER1( "*** ERROR : template file '%s' not found\n" , template_file )
	
	if( env->cmd_para.__instance_file )
	{
		Snprintf( instance_file , sizeof(instance_file)-1 , "%s/merged%s" , env->container_path_base , env->cmd_para.__instance_file );
	}
	else
	{
		strncpy( instance_file , template_file , sizeof(instance_file)-1 );
	}
	
	/* replace file */
	if( env->cmd_para.__debug )
	{
		I( "filerpl [%s] [%s] [%s]\n" , template_file , env->cmd_para.__mapping_file , instance_file )
	}
	nret = filerpl( template_file , env->cmd_para.__mapping_file , instance_file ) ;
	INTER1( "*** ERROR : filerpl failed[%d]\\n" , nret )
	
	printf( "完成\n" );
	
	return 0;
}

