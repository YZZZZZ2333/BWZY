
#ifndef _H_XDOCKER_IN_
#define _H_XDOCKER_IN_

#include "xdocker_util.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CockerVolume
{
	char			*host_path ;
	int			host_path_len ;
	char			*container_path ;
	
	struct list_head	volume_node ;
} ;

struct CommandParameter
{
	char			*_action ;
	char			*_show ;
	
	char			*__version ;
	char			*__image ;
	char			*__container ;
	char			*__host_name ;
	char			*__net ;
	char			*__host_eth ;
	char			*__vip ;
	char			*__port_mapping ;
	char			*__cpus ;
	char			*__cpu_quota ;
	char			*__mem_limit ;
	
	char			*__boot ;
	char			*__attach ;
	char			*__exec ;
	char			*__shutdown ;
	
	char			*__from_image ;
	char			*__to_image ;
	char			*__from_container ;
	char			*__to_container ;
	
	char			*__image_file ;
	
	char			*__debug ;
	char			*__forcely ;
	
	struct list_head	volume_list ;
	char			*__volume ;
	
	char			*__srepo ;
	char			*__match ;
	
	char			*__cmd ;
	char			*__template_file ;
	char			*__mapping_file ;
	char			*__instance_file ;
	char			*__src_file ;
	char			*__dst_file ;
} ;

struct CockerEnvironment
{
	struct CommandParameter	cmd_para ;
	unsigned char		cgroup_enable ;
	
	char			xdocker_home[ PATH_MAX + 1 ] ;
	char			images_path_base[ PATH_MAX + 1 ] ;
	char			containers_path_base[ PATH_MAX + 1 ] ;
	char			netbr_ip[ IP_LEN_MAX + 1 ] ;
	
	char			host_eth_name[ ETHERNET_NAME_LEN_MAX + 1 ] ;
	char			host_eth_ip[ IP_LEN_MAX + 1 ] ;
	
	char			version_path_base[ PATH_MAX + 1 ] ;
	char			image_path_base[ PATH_MAX + 1 ] ;
	char			container_path_base[ PATH_MAX + 1 ] ;
	int			src_port ;
	int			dst_port ;
	char			netns_name[ ETHERNET_NAME_LEN_MAX + 1 ] ;
	char			netbr_name[ ETHERNET_NAME_LEN_MAX + 1 ] ;
	char			veth1_name[ ETHERNET_NAME_LEN_MAX + 1 ] ;
	char			veth0_name[ ETHERNET_NAME_LEN_MAX + 1 ] ;
	char			veth0_sname[ ETHERNET_NAME_LEN_MAX + 1 ] ;
	
	int			alive_pipe[ 2 ] ;
} ;

/*
 * util
 */

void GetEthernetNames( struct CockerEnvironment *env , char *container_id );
int SplitImageVersion( char *image_id , char *version , int version_bufsize );
int GetMaxVersionPath( char *version_path_base , char *max_version , int max_version_bufsize );

/*
 * environment
 */

int CreateCockerEnvironment( struct CockerEnvironment **pp_env );
void DestroyCockerEnvironment( struct CockerEnvironment **pp_env );

int CleanContainerResource( struct CockerEnvironment *env );

int CreateContainer( struct CockerEnvironment *env , char *__image_id , char *__container_id );

int DoShow_images( struct CockerEnvironment *env );
int DoShow_containers( struct CockerEnvironment *env );
int DoShow_container_root( struct CockerEnvironment *env );

int DoAction_install_test( struct CockerEnvironment *env );

int DoAction_create( struct CockerEnvironment *env );
int DoAction_destroy( struct CockerEnvironment *env );
int DoAction_boot( struct CockerEnvironment *env );
int DoAction_attach( struct CockerEnvironment *env );
int DoAction_run( struct CockerEnvironment *env );
int DoAction_rplfile( struct CockerEnvironment *env );
int DoAction_putfile( struct CockerEnvironment *env );
int DoAction_getfile( struct CockerEnvironment *env );
int DoAction_shutdown( struct CockerEnvironment *env );
int _DoAction_kill( struct CockerEnvironment *env , int signal_no );
int DoAction_kill( struct CockerEnvironment *env );

int DoAction_version( struct CockerEnvironment *env );

int DoAction_vip( struct CockerEnvironment *env );
int DoAction_port_mapping( struct CockerEnvironment *env );

int DoAction_volume( struct CockerEnvironment *env );

int DoAction_to_image( struct CockerEnvironment *env );
int DoAction_to_container( struct CockerEnvironment *env );

int DoAction_copy_image( struct CockerEnvironment *env );
int DoAction_del_image( struct CockerEnvironment *env );

int DoAction_export( struct CockerEnvironment *env );
int DoAction_import( struct CockerEnvironment *env );

int DoShow_ssearch( struct CockerEnvironment *env );

int DoAction_spush( struct CockerEnvironment *env );
int DoAction_spull( struct CockerEnvironment *env );

/* depend on
yum install -y telnet
yum install -y nmap-ncat
yum install -y bridge-utils
yum install -y man-pages
yum install -y man-pages-zh-CN
echo "1" >/proc/sys/net/ipv4/ip_forward
	or add net.ipv4.ip_forward=1 to /etc/sysctl.conf and run sysctl -p
*/

/* for test
xdocker -s images
xdocker -s containers
xdocker -s container_root

xdocker -a install_test -d --version "1.0.0"

xdocker -a create -d -m test -c test
xdocker -a create -d -m test --host test --net BRIDGE --vip 166.88.0.2 --port-mapping 19527:9527 -c test
xdocker -a create -d -m test --host test --net HOST --vip 166.88.0.2
xdocker -a create -d -m test --host test --net CUSTOM --vip 166.88.0.2
xdocker -a create -d -m "test:test2" --host test --net BRIDGE --vip 166.88.0.2 --port-mapping 19527:9527 -c test
xdocker -a create -d -m test --volume "/tmp:/tmp" --volume "/mnt/cdrom:/mnt/cdrom" --host test --net BRIDGE --vip 166.88.0.2 --port-mapping 19527:9527 -c test
xdocker -a create -d -m test --host test --net BRIDGE --vip 166.88.0.2 --port-mapping 19527:9527 -c test -b
xdocker -a create -d -m test --host test --net BRIDGE --vip 166.88.0.2 --port-mapping 19527:9527 -c test -b -t
xdocker -a create -d -m test --host test --net BRIDGE --vip 166.88.0.2 --port-mapping 19527:9527 -c test -b -t -e "/bin/bash -l"
xdocker -a create -d -m "calvin=rhel-7.4-x86_64:1.0.0,calvin=rhel-7.4-gcc-x86_64" --host test --net BRIDGE --vip 166.88.0.2 --port-mapping "19527:9527,5142:5142" -c test
xdocker -a create -d -m "calvin=rhel-7.4-x86_64,calvin=rhel-7.4-sshd-x86_64" --host test --net BRIDGE --vip 166.88.0.2 --port-mapping "2222:22" -c test
xdocker -a boot -d -c test
xdocker -a boot -d -c test -t
xdocker -a boot -d --cpus 1 --cpu-quota 30% --mem-limit 100M -c test -t
xdocker -a boot -d -c test -t -e "/bin/bash -l"
xdocker -a attach -d -c test
xdocker -a run -d -c test --cmd "hostname"
xdocker -a shutdown -d -c test
xdocker -a destroy -d -c test
xdocker -a destroy -d -c test -h
xdocker -a destroy -d -f -c test

printf "\${LEAF} ÎÒµÄÊ÷Ò¶\n" >map.txt
xdocker -a rplfile -d -c test --template-file "/root/tpl.txt" --mapping-file "map.txt" --instance-file "/root/ins.txt"
xdocker -a rplfile -d -c test --template-file "/root/tpl.txt" --mapping-file "map.txt"

xdocker -a putfile -c test --src-file "map.txt" --dst-file "/root/"
xdocker -a getfile -c test --src-file "/root/map.txt" --dst-file "./"

xdocker -a version -d -m test --version "1.0.1"
xdocker -a version -d -m "test:1.0.1" --version "1.0.2"
xdocker -a version -d -m "test:1.0.2"

xdocker -a vip -d --vip 166.88.0.3 -c test
xdocker -a port_mapping -d --port-mapping 19528:9528 -c test
xdocker -a volume -d --volume "/tmp:/tmp" --volume "/mnt/cdrom:/mnt/cdrom" -c test

xdocker -a to_container -d --from-image test --host test --net BRIDGE --vip 166.88.0.2 --port-mapping 19527:9527 --to-container test
xdocker -a to_image -d --from-container test --to-image test

xdocker -a copy_image -d --from-image test --to-image "test2:1.0.0"
xdocker -a del_image -d -m "test2:1.0.0"

xdocker -a export -d -m "test:1.1.0"
xdocker -a del_image -d -m "test:1.1.0"
xdocker -a import -d -m "test:1.1.0"

xdocker -a import -d --image-file test.xdockerimage
xdocker -a import -d --image-file rhel-7.4-x86_64.xdockerimage -m rhel-7.4
xdocker -a import -d --image-file rhel-7.4-gcc-x86_64.xdockerimage -m rhel-7.4-gcc

xdocker -a spush -m "test:1.0.0" -d
xdocker -a del_image -m "test:1.0.0" -d
xdocker -a spull -m "test:1.0.0" -d

xdocker -s ssearch --srepo "xdockerimages@192.168.6.74"
xdocker -s ssearch --match test

xdocker -a spush -m calvin=rhel-7.4-x86_64:1.0.0 -d -f

. xdocker_container_root.sh test
*/

/* for test
ps -ef | grep -v grep | grep xdockerinit | awk '{print $2}' | xargs kill -9

sudo cp ~/src/xdocker/src/xdockerinit/xdockerinit /var/xdocker/images/test/rlayer/bin/
sudo cp ~/src/xdocker/src/util/libxdocker_util.so /var/xdocker/images/test/rlayer/lib64/

ip netns del netnstest

echo "let S=0 ; while [ 1 ] ; do let S++; done" >test.sh

cp /home/calvin/src/xdocker/src/xdockerinit/xdockerinit /var/xdocker/images/calvin=rhel-7.4-x86_64/1.0.0/rlayer/usr/bin/xdockerinit
xdocker -a boot -c G6
xdocker -a shutdown -c G6
xdocker -a run -c "G6" --cmd "nohup /usr/sbin/sshd -D &"


xdocker -a create -m "calvin=rhel-7.4-x86_64,calvin=sshd,calvin=G6" --host G6 --net BRIDGE --vip 166.88.0.2 --port-mapping "8600:8600,2222:222" -c "G6" -b -e "/root/bin/G6 -f /root/etc/G6.conf --no-daemon" -d
xdocker -a boot -c "G6"
xdocker -a attach -c "G6"
xdocker -a boot -c "G6" -e "/root/bin/G6 -f /root/etc/G6.conf --no-daemon"
xdocker -a shutdown -c G6
xdocker -a shutdown -c G6 -f
xdocker -a destroy -c G6
xdocker -a destroy -c G6 -h
*/

#ifdef __cplusplus
}
#endif

#endif

