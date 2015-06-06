#include "cmc.h"
#include "macro.h"
#include "minimsg/minimsg.h"
#include <string.h>
int CMSPush(int client, char *key_to_send,char *value_to_send)
{
	msg_t* m;
	int result;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSPUSH);
	msg_append_string(m,key_to_send);
	msg_append_string(m,value_to_send);

	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 1){
		msg_free(m);
		return CMS_FAIL;
	}
	result = atoi( msg_content_at_frame(m,0) );

	msg_free(m);
	return result;
}
int CMSPop(int client,char *key_to_send,char **value_to_recv)
{
	msg_t* m;
	
	int result;
	const char* value;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSPOP);
	msg_append_string(m,key_to_send);

	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 2){
		msg_free(m);
		return CMS_FAIL;
	}
	
	result = atoi(msg_content_at_frame(m,0) );
	value = msg_content_at_frame(m,1);
	
	
	if(result == CMS_OK){
			*value_to_recv = strdup(value);
			if( (*value_to_recv) == NULL) result = CMS_FAIL;
	}
	msg_free(m);
	return result;
}


int CMSWriteParam(int client)
{
	msg_t* m;
	int result=CMS_FAIL;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSWRITEPARAMNOW);

	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 1){
		msg_free(m);
		return CMS_FAIL;
	}

	result = atoi(msg_content_at_frame(m,0) );

	msg_free(m);
	return result;
}

int CMSAddParam(int client, char*fileName, char *key_to_send,char *value_to_send)
{
	msg_t* m;
	int result;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSADDVALUE);
	msg_append_string(m,key_to_send);
	msg_append_string(m,value_to_send);
	msg_append_string(m,fileName);

	/* recv content */
	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 1){
		msg_free(m);
		return CMS_FAIL;
	}

	result = atoi(msg_content_at_frame(m,0) );

	msg_free(m);
	return result;
}
int CMSDelValue(int client,char *key_to_send)
{
	msg_t* m;
	int result;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSDELVALUE);
	msg_append_string(m,key_to_send);

	/* recv content */
	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 1){
		msg_free(m);
		return CMS_FAIL;
	}

	result = atoi(msg_content_at_frame(m,0) );

	msg_free(m);
	return result;
}

int CMSGetValue(int client,char *key_to_send,char **value_to_recv)
{
	msg_t* m;
	const char* value;
	int result;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSGETVALUE);
	msg_append_string(m,key_to_send);

	/* recv content */
	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 2){
		msg_free(m);
		return CMS_FAIL;
	}

	result = atoi(msg_content_at_frame(m,0) );
	value = msg_content_at_frame(m,1);

	if(result == CMS_OK){
		if(value){
			*value_to_recv = strdup(value);
			if( (*value_to_recv) == NULL )result = CMS_FAIL;
		}
	}
	msg_free(m);
	return result;
}
int CMSSetValue(int client, char *key_to_send,char *value_to_send)
{
	msg_t* m;
	int result;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSSETVALUE);
	msg_append_string(m,key_to_send);
	msg_append_string(m,value_to_send);

	msg_print(m);
	/* recv content */
	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;
	if( msg_number_of_frame(m) != 1){
		msg_free(m);
		return CMS_FAIL;
	}

	result = atoi(msg_content_at_frame(m,0) );

	msg_free(m);
	return result;
}
int CMSSetMultiValue(int client, char **  keys_to_send, char **  values_to_send, int number)
{
	msg_t* m;
	int result;
	int i;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSSETMULTIVALUE);
	msg_append_string_f(m,"%d",number);
	for(i=0;i<number;i++){
		msg_append_string(m,keys_to_send[i]);
		msg_append_string(m,values_to_send[i]);
	}
	/* recv content */
	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 1){
		msg_free(m);
		return CMS_FAIL;
	}

	result = atoi(msg_content_at_frame(m,0) );

	msg_free(m);
	return result;
}
int CMSGetMultiValue(int client, char **  keys_to_send, char *** values_to_recv, int number)
{
	msg_t* m;
	int result;
	char** ret;
	int i,j;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSGETMULTIVALUE);
	msg_append_string_f(m,"%d",number);
	for(i=0;i<number;i++){
		msg_append_string(m,keys_to_send[i]);
	}
	/* recv content */
	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) < 2){
		msg_free(m);
		return CMS_FAIL;
	}

	result = atoi(msg_content_at_frame(m,0) );

	printf("result = %d\n",result);
	if(result == CMS_FAIL){
		msg_free(m);
		return CMS_FAIL;	
	}

	number =  atoi( msg_content_at_frame(m,1));

	if(msg_number_of_frame(m) != (2 + number)){
		msg_free(m);
		return CMS_FAIL;
	}
	ret = (char**) malloc(sizeof(char*) * number);
 
	for(i=0;i<number;i++){
		ret[i] = strdup( msg_content_at_frame(m,2+i));
		if(!ret[i]){
			for(j=0;j<i;j++){
				free(ret[i]);
			}
			free(ret);
			result = CMS_FAIL;
			break;				
		}
	}	
	*values_to_recv = ret;
	msg_free(m);
	return result;
}
int CMSSetMultiValuef(int  client, int number, ...)
{
	int i;
	int err;
	char** keys_to_send;
	char** values_to_send;
	char* reply;
	va_list num_list;
	int result;	
	keys_to_send = malloc( sizeof(char*) *number);
	values_to_send=malloc( sizeof(char*)*number);

	if(!keys_to_send || !values_to_send){
		if(keys_to_send)free(keys_to_send);
		if(values_to_send)free(values_to_send);
		return CMS_FAIL;
	}


	// make sure we can allocate all the memory we need.
	va_start(num_list, number);
	for(i = 0 ;i < number ; i++){
		keys_to_send[i] =  va_arg(num_list, char*) ;
		values_to_send[i]= va_arg(num_list, char*) ;
	}
	va_end(num_list);

	result=CMSSetMultiValue(client,keys_to_send,values_to_send,number);
	free(keys_to_send);
	free(values_to_send);
	return result;
}

int CMSGetMultiValuef(int client, char*** values_to_recv,int number, ...)
{
	int i;
	int err;
	char** keys_to_send;
	char** values;
	int64_t more;
	va_list num_list;
	int result;
        size_t more_size = sizeof (more);
	keys_to_send = malloc( sizeof(char*) *number);

	if(!keys_to_send) return CMS_FAIL;	
	
	// make sure we can allocate the memory we need
	va_start(num_list, number);
	for(i = 0 ;i < number ; i++){
		keys_to_send[i] =  va_arg(num_list, char*) ;
	}
	va_end(num_list);
	
	result=CMSGetMultiValue(client,keys_to_send,values_to_recv,number);
	
	free(keys_to_send);
	return result;
}
int CMSDumpFile(int client, char *filename)
{
	msg_t* m;
	int result;
	m = msg_alloc();
	if(!m)return CMS_FAIL;	

	msg_append_string_f(m,"%d",CMSDUMPFILE);
	msg_append_string(m,filename);

	/* recv content */
	if(msg_send(client,m) !=MINIMSG_OK) return CMS_FAIL;

	if(msg_recv(client,&m) != MINIMSG_OK) return CMS_FAIL;

	if( msg_number_of_frame(m) != 1){
		msg_free(m);
		return CMS_FAIL;
	}
	result = atoi( msg_content_at_frame(m,0) );

	msg_free(m);
	return result;
}

