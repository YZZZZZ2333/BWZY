#include "xdocker_in.h"

int CreatexdockerEnvironment( struct xdockerEnvironment **pp_env ) {
	struct xdockerEnvironment	*env = NULL ;
	
	char				netbr_name[ ETHERNET_NAME_LEN_MAX + 1 ] ;
	char				cmd[ 4096 ] ;
	int				len ;
	
	int				nret = 0 ;
	
	env = (struct xdockerEnvironment *)malloc( sizeof(struct xdockerEnvironment) ) ;
	if( env == NULL ) {
		printf( "*** ERROR : malloc failed , errno[%d]\n" , errno );
		return 1;
	}
	memset( env , 0x00 , sizeof(struct xdockerEnvironment) );
	
	if( getenv("xdocker_HOME" ) ) {
		nret = SnprintfAndMakeDir( env->xdocker_home , sizeof(env->xdocker_home)-1 , "%s" , getenv("xdocker_HOME" ) ) ;
		if( nret ) {
			printf( "*** ERROR : SnprintfAndMakeDir[%s] failed[%d]\n" , env->xdocker_home , nret );
			free( env );
			return -1;
		}
	}
	else {
		nret = SnprintfAndMakeDir( env->xdocker_home , sizeof(env->xdocker_home)-1 , "/var/xdocker" ) ;
		if( nret ) {
			printf( "*** ERROR : SnprintfAndMakeDir[%s] failed[%d]\n" , env->xdocker_home , nret );
			free( env );
			return -1;
		}
	}
	
	nret = SnprintfAndMakeDir( env->images_path_base , sizeof(env->images_path_base)-1 , "%s/images" , env->xdocker_home ) ;
	if( nret ) {
		printf( "*** ERROR : SnprintfAndMakeDir[%s] failed[%d]\n" , env->images_path_base , nret );
		return -1;
	}
	
	nret = SnprintfAndMakeDir( env->containers_path_base , sizeof(env->containers_path_base)-1 , "%s/containers" , env->xdocker_home ) ;
	if( nret ) {
		printf( "*** ERROR : SnprintfAndMakeDir[%s] failed[%d]\n" , env->containers_path_base , nret );
		return -1;
	}
	
	memset( netbr_name , 0x00 , sizeof(netbr_name) );
	len = snprintf( netbr_name , sizeof(netbr_name)-1 , "xdocker0" ) ;
	if( SNPRINTF_OVERFLOW(len,sizeof(netbr_name)-1) ) {
		printf( "*** ERROR : netbr name overflow\n" );
		free( env );
		return -1;
	}
	
	nret = SnprintfAndSystem( cmd , sizeof(cmd) , "brctl show | grep -E \"^%s\" >/dev/null 2>&1" , netbr_name ) ;
	if( nret ) {
		nret = SnprintfAndSystem( cmd , sizeof(cmd) , "brctl addbr %s" , netbr_name ) ;
		if( nret ) {
			printf( "*** ERROR : system [%s] failed[%d] , errno[%d]\n" , cmd , nret , errno );
			free( env );
			return -1;
		}
		else if( env->cmd_para.__debug ) {
			printf( "system [%s] ok\n" , cmd );
		}
		
		nret = SnprintfAndSystem( cmd , sizeof(cmd) , "ifconfig %s 166.88.0.1" , netbr_name ) ;
		if( nret ) {
			printf( "*** ERROR : system [%s] failed[%d] , errno[%d]\n" , cmd , nret , errno );
			return -1;
		}
		else if( env->cmd_para.__debug ) {
			printf( "system [%s] ok\n" , cmd );
		}
		
		nret = SnprintfAndSystem( cmd , sizeof(cmd) , "ifconfig %s up" , netbr_name ) ;
		if( nret ) {
			printf( "*** ERROR : system [%s] failed[%d] , errno[%d]\n" , cmd , nret , errno );
			return -1;
		}
		else if( env->cmd_para.__debug ) {
			printf( "system [%s] ok\n" , cmd );
		}
	}
	
	memset( cmd , 0x00 , sizeof(cmd) );
	SnprintfAndPopen( env->netbr_ip , sizeof(env->netbr_ip) , cmd , sizeof(cmd) , "ifconfig xdocker0 | grep -w inet | awk '{print $2}'" );
	TrimEnter( env->netbr_ip );
	
	INIT_LIST_HEAD( & (env->cmd_para.volume_list) );
	
	(*pp_env) = env ;
	
	return 0;
}

void DestroyxdockerEnvironment( struct xdockerEnvironment **pp_env ) {
	struct xdockerVolume	*volume = NULL ;
	struct xdockerVolume	*next_volume = NULL ;
	
	list_for_each_entry_safe( volume , next_volume , & ((*pp_env)->cmd_para.volume_list) , struct xdockerVolume , volume_node ) {
		list_del( & (volume->volume_node) );
		free( volume );
	}
	
	free( (*pp_env) );
	(*pp_env) = NULL ;
	
	return;
}

