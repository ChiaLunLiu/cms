#include <minimsg/minimsg.h>
#include <minimsg/util.h>
#include <stdio.h>
#include <event2/event.h>
#include <signal.h>
#include <string.h>
#include "../include/macro.h"
#include "simclist.h"
#include "cms.h"

/* global variable */
cms_t cms;

/* cb is callback function when a message is received through network
 * user does not need to care who sends the message, the server would handle it
 * input 
 *  arg 1 type:msg_t*
 * output
 * possible values
 *    1.  NULL  ( the user has nothing to send it back
 *    2.  msg_t* if the user wants to send message back
 * 
 * user has to free input argument.
 */ 
void* cb(void* arg)
{
	const char* key;
	const char* value;
	const char* filename;
	int number;
	char* tmp;
	int type;
	int i;
	msg_t* response_message;
	msg_t* m =(msg_t*)arg;
	printf("received\n");
	msg_print(m);
	response_message = msg_alloc();
	if(response_message == NULL){
		handle_error("msg_alloc fails");
		return NULL;
	}

	if(msg_number_of_frame(m) < 1) goto error;

	type = atoi( msg_content_at_frame(m,0) );	

	if(type ==CMSGETVALUE ){
		if(msg_number_of_frame(m) != 2) goto error;
		key =  msg_content_at_frame(m,1);	
		dbg("key=%s\n",key);	
		//TODO
		msg_append_string(response_message,"test");
		
	} 
	else if(type == CMSDELVALUE){
		dbg("cmsdelvalue\n");		
		if(msg_number_of_frame(m) != 2) goto error;
		key =  msg_content_at_frame(m,1);	
		dbg("key=%s\n",tmp);	
		//TODO		
		
	}
	else if(type == CMSWRITEPARAMNOW){
		dbg("cmswriteparamnow\n");		
		//TODO	
	}
	else if(type == CMSSETVALUE){
		dbg("cmssetvalue\n");		
		if(msg_number_of_frame(m) != 3) goto error;
		key = msg_content_at_frame(m,1);
		value = msg_content_at_frame(m,2);
		dbg("key=%s, value=%s\n",key,value);
	}
	else if(type == CMSPUSH){
		dbg("cmspushcmcmd\n");		
		if(msg_number_of_frame(m) != 3) goto error;
		key = msg_content_at_frame(m,1);
		value = msg_content_at_frame(m,2);
		dbg("key=%s, value=%s\n",key,value);
		
	}
	else if(type == CMSPOP){
		dbg("cmspopcmcmd\n");		
		if(msg_number_of_frame(m) != 2) goto error;
		key = msg_content_at_frame(m,1);
		
		msg_append_string(response_message,"pop value");
		
	}
	else if(type == CMSSETMULTIVALUE){
		dbg("cmssetmultivalue\n");		
		if(msg_number_of_frame(m) < 2) goto error;
		number = atoi(msg_content_at_frame(m,1) );
		if( ( msg_number_of_frame(m) - 2) != (2*number)  ) goto error;
		for(i=0;i<number;i++){
			key = msg_content_at_frame(m,2 + 2*i);
			value = msg_content_at_frame(m,2 + 2*i + 1);
			//TODO	
		}	
		
	}
	else if(type == CMSGETMULTIVALUE){
		dbg("cmsgetmultivalue\n");
		if(msg_number_of_frame(m) < 2) goto error;
		number = atoi( msg_content_at_frame(m,1));
		if(msg_number_of_frame(m) != ( number + 2) ) goto error;
		msg_append_string_f(response_message,"%d", number);
		for(i=0;i<number;i++){
			key = msg_content_at_frame(m,2 + i);
			msg_append_string_f(response_message,"%s-v",key);
		}	
				
		
	}
	else if(type == CMSDUMPFILE){
		dbg("cmsdumpfile\n");		
		if(msg_number_of_frame(m) != 2) goto error;	
		key = msg_content_at_frame(m,1);
		//TODO
		dbg("delete file %s\n",key);
	}
	else if(type == CMSADDVALUE){
		dbg("cmsaddvalue\n");
		if(msg_number_of_frame(m) != 4) goto error;	
		key = msg_content_at_frame(m,1);
		value = msg_content_at_frame(m,2);
		filename = msg_content_at_frame(m,3);
		dbg("key=%s,value=%s,filename=%s\n",key,value,filename);		
	}
	else{
		dbg("undefined type:%d\n",type);
		goto error;	
	}
	msg_free(m);
	msg_prepend_string_f(response_message,"%d",CMS_OK);
	msg_print(response_message);	
	return response_message;
error:
	dbg("error occurs\n");
	msg_free(m);
	msg_prepend_string_f(response_message,"%d",CMS_FAIL);
	
	return response_message;
}
int main()
{
    int port = CMS_PORT;
    msg_server_t* server;
    struct event_base *base;
    base = event_base_new();
    if (!base)
        return; /*XXXerr*/
    
    cms_init(&cms);
    cms_load_conf_dir(&cms,"/home/bendog/git/cms/cms/test/conf");
    
    cms_print(&cms);
    
    if(server = create_msg_server(base,port, cb,3)){
		puts("server is initiated");
    }
    else{
		puts("fail to start server, program will be terminated");
		return 0;
    }
    /* infinite loop */
    event_base_dispatch(base); 
    printf("the server is shut down\n");
    event_base_free(base);
    cms_destroy(&cms);
    return 0;
}
