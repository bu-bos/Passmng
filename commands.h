#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "hash.h"



int master_p(char* master_pass){
    FILE *m_pass = fopen("master.txt", "r");
    if(m_pass == NULL){
        printf("File with master password is not found.\n");
        system("pause");
        return 1;
    }
    fscanf(m_pass, "%s", master_pass);
    fclose(m_pass);
    char* input = (char*)malloc(1024 );
    printf("Enter master password: ");
    scanf("%s", input);
    hash_f(input);
    if(strcmp(master_pass, input) == 0){
        printf("Success\n");
        return 0;
    }
    else{
        printf("Fail\n");
        system("pause");
        return 1;
    }
}

int w_w_file(char* master_pass, char* master_pass_cfg, char* config_data, char*** pass, int* pass_count){
    FILE *config = fopen("passmng.conf", "r");
    if(config == NULL){
        printf("File with config is not found.\n");
        system("pause");
        return 1;
    }
    fscanf(config, "%s", config_data);
    fclose(config);
    FILE *passwords = fopen(config_data, "r");
    if(passwords == NULL){
        printf("File with passwords is not found.\n");
        system("pause");
        return 1;
    }
    fscanf(passwords, "%s", master_pass_cfg);
    if(strcmp(master_pass, master_pass_cfg) != 0){
        printf("Master password and master password cfg don't match\n");
        system("pause");
        return 1;
    }
    fscanf(passwords, "%d", pass_count);
    for(int i = 0; i < *pass_count; i++){
        pass[i] = (char**)malloc(4096);
        pass[i][0] = (char*)malloc(2048);
        fscanf(passwords, "%s", pass[i][0]);
        pass[i][1] = (char*)malloc(2048);
        fscanf(passwords, "%s", pass[i][1]);
        vigenere_r(pass[i][1], master_pass);
        //printf("%s %s\n", pass[i][0], pass[i][1]);
    }
    fclose(passwords);
    //printf("%s\n %d\n", master_pass_cfg, *pass_count);
    return 0;
}


void master(char* master_pass, char* master_pass_cfg){
    memset(master_pass, '\0', strlen(master_pass));
    memset(master_pass_cfg, '\0', strlen(master_pass_cfg));
    printf("Enter master password: ");
    scanf("%s", master_pass);
    hash_f(master_pass);
    strncpy(master_pass_cfg, master_pass, strlen(master_pass));
    printf("Success\n");
}

void get(char* site_log, char*** pass, const int* pass_count, char* master_pass){
    for(int i = 0; i < *pass_count; i++){
        if(strcmp(site_log, pass[i][0]) == 0){
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(pass[i][1]) + 1 );
            memcpy(GlobalLock(hMem),pass[i][1], strlen(pass[i][1]) + 1);
            GlobalUnlock(hMem);
            OpenClipboard(0);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, hMem);
            CloseClipboard();
            printf("Success\n");
            return;
        }
    }
    printf("This SITE:LOGIN not found\n");
}

void add(char* site_log, char*** pass, int* pass_count){
    for(int i = 0; i < *pass_count; i++){
        if(strcmp(site_log, pass[i][0]) == 0){
            printf("Password for this SITE:LOGIN already exists. Do you want to overwrite it?(Y/N): ");
            while(1){
                char choose;
                scanf("%c", &choose);
                if(choose == 'Y')
                    break;
                else if(choose == 'N'){
                    printf("OK\n");
                    return;
                }
                else
                    printf("Wrong answer. Try again: ");
            }
            break;
        }
    }
    pass[*pass_count] = (char**)malloc(10000);
    pass[*pass_count][0] = (char*)malloc(1024);
    pass[*pass_count][1] = (char*)malloc(1024);
    pass[*pass_count][0] = site_log;
    printf("Enter password: ");
    scanf("%s", pass[*pass_count][1]);
    (*pass_count)++;
    printf("Success\n");
}

void delete(char* site_log, char*** pass, int* pass_count){
    for(int i = 0; i < *pass_count; i++){
        if(strcmp(site_log, pass[i][0]) == 0){
            (*pass_count)--;
            if(i != *pass_count){
                for(int j = 0; j < 2; j++){
                    memset(pass[i][j], '\0', strlen(pass[i][j]));
                    strncpy(pass[i][j], pass[*pass_count][j], strlen(pass[*pass_count][j]));
                }
            }
            free(pass[*pass_count]);
            printf("Success\n");
            return;
        }
    }
    printf("This SITE:LOGIN not found\n");
}

void check(char* site_log, char*** pass, const int* pass_count){
    for(int i = 0; i < *pass_count; i++){
        if(strcmp(site_log, pass[i][0]) == 0){
            printf("Password exists\n");
            return;
        }
    }
    printf("Password doesn't exist\n");
}

void help(){
    printf("master - set a new master password\n");
    printf("get SITE:LOGIN - copies to the clipboard the password for SITE and LOGIN\n");
    printf("add SITE:LOGIN - adds the password for SITE and LOGIN to the database\n");
    printf("del SITE:LOGIN - removes from the database the password for SITE and LOGIN\n");
    printf("chk SITE:LOGIN - checks if there is a password for SITE and LOGIN\n");
    printf("help - displays available commands and their description\n");
    printf("q / quit / exit - exits the program\n");
}

void end(char* master_pass, char* master_pass_cfg, char* config_data, char*** pass, const int* pass_count){
    FILE *m_pass = fopen("master.txt", "w");
    fprintf(m_pass, "%s", master_pass);
    fclose(m_pass);
    FILE *passwords = fopen(config_data, "w");
    FILE *open_base = fopen("open_base.txt", "w");
    fprintf(passwords, "%s\n", master_pass_cfg);
    free(master_pass_cfg);
    fprintf(passwords, "%d\n", *pass_count);
    fprintf(open_base, "%d\n", *pass_count);
    for(int i = 0; i < *pass_count; i++){
        fprintf(open_base, "%s %s\n", pass[i][0], pass[i][1]);
        vigenere(pass[i][1], master_pass);
        fprintf(passwords, "%s %s\n", pass[i][0], pass[i][1]);
        free(pass[i][0]);
        free(pass[i][1]);
        free(pass[i]);
    }
    fclose(passwords);
    fclose(open_base);
    free(pass);

}

#endif // COMMANDS_H_INCLUDED
