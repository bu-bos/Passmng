#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "hash.h"


int main (){
    char* master_pass = (char*)malloc(1024);
    char* master_pass_cfg = (char*)malloc(1024);
    char* config_data = (char*)malloc(1024);
    char*** pass = (char***)malloc(1024 * sizeof(char**));
    int password_count;
    int *pass_count = &password_count;
    if(master_p(master_pass))
        return 1;
    if(w_w_file(master_pass, master_pass_cfg, config_data, pass, pass_count))
        return 1;
    while(1){
        printf("Enter command: ");
        char* com = (char*)malloc(1024);
        scanf("%s", com);
        if(strcmp(com, "master") == 0)
            master(master_pass, master_pass_cfg);
        else if(strcmp(com, "get") == 0){
            char* site_log = (char*)malloc(1024);
            scanf("%s", site_log);
            //hash_f(site_log);
            get(site_log, pass, pass_count, master_pass);
            free(site_log);
        }
        else if(strcmp(com, "add") == 0){
            char* site_log = (char*)malloc(1024);
            scanf("%s", site_log);
            //hash_f(site_log);
            add(site_log, pass, pass_count);
        }
        else if(strcmp(com, "delete") == 0){
            char* site_log = (char*)malloc(1024);
            scanf("%s", site_log);
            //hash_f(site_log);
            delete(site_log, pass, pass_count);
        }
        else if(strcmp(com, "check") == 0){
            char* site_log = (char*)malloc(1024);
            scanf("%s", site_log);
            //hash_f(site_log);
            check(site_log, pass, pass_count);
        }
        else if(strcmp(com, "help") == 0)
            help();
        else if(strcmp(com, "q") == 0 || strcmp(com, "quit") == 0 ||strcmp(com, "exit") == 0){
            printf("Success.\n");
            break;
        }
        else printf("Wrong command, try again.\n");
    }
    end(master_pass, master_pass_cfg, config_data, pass, pass_count);
    return 0;
}
