#include "cms.h"
#include "macro.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ftw.h>

/* global variable */
static cms_t* g_cms;

int var_comparator(const void *a, const void *b);
int var_seeker(const void *el, const void *key);

int var_comparator(const void *a, const void *b) {
    /* compare areas */
    fprintf(stderr,"%s\n",__func__);
    const element_t *A = (element_t *) a;
    const element_t *B = (element_t *) b;

    return strcmp(A->name,B->name);
}
int ftw_handler(const char *file, const struct stat *sb, int flag)
{
        if (flag == FTW_F) {
      		printf("tracing %s\n", file);
		cms_load_conf_file(g_cms,file);
        }

        return 0;
}

void cms_load_conf_dir(cms_t* cms, const char* dirpath)
{
	g_cms = cms;
	ftw(dirpath, ftw_handler, 8);
}
/*
 *  build conf_file_t 
 *  build element in conf file
 */
void cms_load_conf_file(cms_t* cms,const char* filename)
{
	const char delimiter[]="=; \t\n";
	FILE* fp = fopen(filename,"r");
	char* saveptr;
	char* key;
	char* value;
	char buf[LINE_BUF_SIZE];	
	element_t* e;
	conf_file_t* cf;
	
	if(fp){
		
		cf = cms_create_conf_file(filename);
		if(!cf) handle_error("cms_create_conf_file fails\n");
		list_append(&cms->conf_file,cf);
		
		while(fgets(buf,LINE_BUF_SIZE,fp)){
			if(!strcmp(buf,"\n") )continue;
			key  =  strtok_r(buf,delimiter,&saveptr);
			value = strtok_r(NULL,delimiter,&saveptr);
			if(key){
				if(!value)value="";
				e = cms_create_element(key,value);
				
				if(!e) handle_error("cms_create_elment fails\n");
				e->conf_file = cf;
				dbg("add %s,%s\n",key,value);			
				list_append(&cms->var,e);
			}
			else dbg(" key  is messing in %s\n",filename);	
		}
		fclose(fp);
	}
	else dbg("fail to open file '%s'\n",filename);
}


void cms_init(cms_t* cms)
{
        list_init(&cms->var);
        list_init(&cms->stack);
        list_init(&cms->conf_file);
        list_attributes_comparator(&cms->var, var_comparator);
		list_attributes_seeker(&cms->var, var_seeker);
}
void cms_destroy(cms_t* cms)
{
		list_destroy(&cms->conf_file);
        list_destroy(&cms->var);
        list_destroy(&cms->stack);
}

int var_seeker(const void *el, const void *key) {
    /* let's assume el and key being always != NULL */
    const element_t *e = (const element_t *)el;

	if(!strcmp(e->name,key))return 1;
	else return 0;
}

conf_file_t* cms_create_conf_file(const char* filename)
{
	conf_file_t* cf;
	cf = (conf_file_t*)malloc( sizeof( conf_file_t));
	if(!cf) return NULL;
	cf->path = strdup( filename);
	if(!cf->path){
		free(cf);
		return NULL;
	}
	cf->need_update = 0;
	list_init(&cf->list);
	cf->handler = NULL;
	return cf;
}
void cms_free_conf_file(conf_file_t* cf)
{
	free(cf->path);
	free(cf);
}
element_t* cms_create_element(const char* name,const char* value)
{
	element_t* e;
	e = malloc( sizeof( element_t) );
	if(!e)return NULL;
	
	e->name = strdup(name);
	e->value = strdup(value);
	
	if( e->name == NULL || e->value == NULL){
		if(e->name)free(e->name);
		if(e->value)free(e->value);
		free(e);
		return NULL;
	}
	e->conf_file = NULL;
	e->handler = NULL;
	return e;
}
void cms_free_element(element_t* e)
{
	free(e->name);
	free(e->value);
	free(e);
}
/*
 *  print all monitored conf files, element and stack
 */
void cms_print(cms_t* cms)
{
	conf_file_t* cf;
	element_t* e;
	
    printf("starting printing element\n");
    list_iterator_start(&cms->var);        /* starting an iteration "session" */

    while (list_iterator_hasnext(&cms->var)) { /* tell whether more values available */
		e = list_iterator_next(&cms->var);
		printf("%s='%s' in [%s]\n",e->name,e->value,(e->conf_file) ? e->conf_file->path:"memory");
    }
    list_iterator_stop(&cms->var); 
    printf("end printing element\n");
    
	
	
	printf("start printing conf files\n");
	list_iterator_start(&cms->conf_file);        /* starting an iteration "session" */
    while (list_iterator_hasnext(&cms->conf_file)) { /* tell whether more values available */
		cf = list_iterator_next(&cms->conf_file);
		printf("cf: %s\n",cf->path);
    }
    list_iterator_stop(&cms->conf_file); 
    printf("end printing conf files\n");
    
   
    printf("starting printing stack\n");
    list_iterator_start(&cms->stack);        /* starting an iteration "session" */
    while (list_iterator_hasnext(&cms->stack)) { /* tell whether more values available */
		e = list_iterator_next(&cms->stack);
		printf("%s\n",e->name);
    }
    list_iterator_stop(&cms->stack); 
    printf("end printing stack\n");
}

element_t* cms_search_var(cms_t* cms, const char* name)
{
	
}
