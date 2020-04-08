//
// Created by Dhruv on 4/9/2020.
//

#ifndef CSHELL_MASTER_COMMANDS_H
#define CSHELL_MASTER_COMMANDS_H

#define BUFSIZE 1000
#define INPBUF 100
#define ARGMAX 10
#define GREEN "\x1b[92m"
#define BLINK "\x1b[5m"
#define BLUE "\x1b[94m"
#define DEF "\x1B[0m"
#define CYAN "\x1b[96m"

struct _instr
{
    char * argval[INPBUF];
    int argcount;
};
typedef struct _instr instruction;

char *input,*input1;
int exitflag = 0;
int filepid,fd[2];
char cwd[BUFSIZE];
char* argval[ARGMAX];
int argcount = 0,inBackground = 0;
int externalIn=0,externalOut=0;
char inputfile[INPBUF],outputfile[INPBUF];
void screenfetch();
void about();
void getInput();
int function_exit();
void function_pwd(char*, int);
void function_cd(char*);
void function_mkdir(char*);
void function_rmdir(char*);
void function_clear();
void nameFile(struct dirent*, char*);
void function_ls();
void function_lsl();
void function_cp(char*, char*);
void executable();
// void pipe_dup(int, instruction*);
void run_process(int, int, instruction*);


#endif //CSHELL_MASTER_COMMANDS_H
