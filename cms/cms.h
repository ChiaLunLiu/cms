#ifndef __CMS_H__
#define __CMS_H__
#include "simclist.h"
struct _element;
typedef struct _element element_t;

typedef struct _conf_file{
        char* path;
        int need_update;
        list_t list; /* element list */
        int(*handler)();
}conf_file_t;

struct _element{
        char* name;
        char* value; /* all element must have value */
        conf_file_t* conf_file;
        int (*handler)();
};
typedef struct _cms{
        list_t conf_file;
        list_t var;
        list_t stack;
}cms_t;

conf_file_t* cms_create_conf_file(const char* filename);
void cms_free_conf_file(conf_file_t* cf);
element_t* cms_create_element(const char* name,const char* value);
void cms_free_element(element_t* e);


void cms_load_conf_dir(cms_t* cms,const char* dirpath);
void cms_load_conf_file(cms_t* cms,const char* filename);
void cms_init(cms_t* cms);
void cms_destroy(cms_t* cms);
void cms_print(cms_t* cms);
element_t* cms_search_var(cms_t* cms, const char* name);
#endif
