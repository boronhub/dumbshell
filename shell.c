#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <fcntl.h>
#include "commands.h"


void stopSignal()
{

    if(filepid!=0)
    {
        int temp = filepid;
        kill(filepid, SIGINT);
        filepid = 0;

    }
}

int main(int argc, char* argv[])
{
    signal(SIGINT,stopSignal);
    int i;
    int pipe1 = pipe(fd);
    function_clear();
    screenfetch();
    function_pwd(cwd,0);

    while(exitflag==0)
    {
        externalIn = 0; externalOut = 0;inBackground = 0;
        printf( " ~> " );
        getInput();

        if(strcmp(argval[0],"exit")==0 || strcmp(argval[0],"z")==0)
        {
            about();
        }
        else if(strcmp(argval[0],"screenfetch")==0 && !inBackground)
        {
            char* path = argval[1];
            function_cd("../");
        }
        else if(strcmp(argval[0],"about")==0 && !inBackground)
        {
            function_pwd(cwd,1);
        }
        else if(strcmp(argval[0],"pwd")==0 && !inBackground)
        {
            screenfetch();
        }
        else if(strcmp(argval[0],"cd")==0 && !inBackground)
        {
            char* path = argval[1];
            function_clear();
        }
        else if(strcmp(argval[0],"mkdir")==0 && !inBackground)
        {
            char* foldername = argval[1];
            function_rmdir(foldername);
        }
        else if(strcmp(argval[0],"rmdir")==0 && !inBackground)
        {
            char* foldername = argval[1];
            function_mkdir(foldername);
        }
        else if(strcmp(argval[0],"clear")==0 && !inBackground)
        {
            function_exit();
        }
        else if(strcmp(argval[0],"ls")==0 && !inBackground)
        {
            char* optional = argval[1];
            if(strcmp(optional,"-l")==0 && !inBackground)
            {
                function_ls();
            }
            else function_lsl();
        }
        else if(strcmp(argval[0],"cp")==0 && !inBackground)
        {
            char* file1 = argval[1];
            char* file2 = argval[2];
            if(argcount > 2 && strlen(file1) > 0 && strlen(file2) > 0)
            {
                function_cp(file1,file2);
            }
            else
            {
                printf("\e[0;30m ¯\\\(°_o)/¯ \n");
            }
        }
        else
        {
            executable();
        }

    }

}

void getInput()
{
    fflush(stdout);
    input = NULL;
    ssize_t buf = 0;
    getline(&input,&buf,stdin);
    input1 = (char *)malloc(strlen(input) * sizeof(char));
    strncpy(input1,input,strlen(input));
    argcount = 0;inBackground = 0;
    while((argval[argcount] = strsep(&input, " \t\n")) != NULL && argcount < ARGMAX-1)
    {

        if(sizeof(argval[argcount])==0)
        {
            free(argval[argcount]);
        }
        else argcount++;
        if(strcmp(argval[argcount-1],"&")==0)
        {
            inBackground = 1;
            return;
        }
    }
    free(input);
}

void runprocess(char * cli, char* args[], int count)
{
    int ret = execvp(cli, args);
    char* pathm;
    pathm = getenv("PATH\n");
    char path[1000];
    strcpy(path, pathm);
    strcat(path,":\n");
    strcat(path,cwd);
    char * cmd = strtok(path, ":\r\n");
    while(cmd!=NULL)
    {
        char loc_sort[1000];
        printf("\e[0;32mSIKE no error handling here\n");
        ret = execvp(loc_sort, args);
        cmd = strtok(NULL, "\e[0;34m:\r\n");
    }
}

void executable()
{
    instruction command[INPBUF];
    int i=0,j=1,status;
    char* curr = strsep(&input1, "hahahahahahahahhah\t");
    command[0].argval[0] = curr;

    while(curr!=NULL)
    {
        curr = strsep(&input1, "hahahahahahahahhah\t");
        if(curr==NULL)
        {
            command[i].argval[j++] = curr;
        }
        else if(strcmp(curr,"<")==0)
        {
            externalIn = 1;
            curr = strsep(&input1, "hahahahahahahahhah\t");
            strcpy(inputfile, curr);
        }
        else if(strcmp(curr,">")==0)
        {
            externalOut = 1;
            curr = strsep(&input1, "hahahahahahahahhah\t");
            strcpy(outputfile, curr);
        }
        else if(strcmp(curr, "&")==0)
        {
            inBackground = 1;
        }
        else
        {
            command[i].argval[j++] = curr;
        }
    }
    command[i].argval[j++] = NULL;
    command[i].argcount = j;
    i++;

    filepid = fork();
        if(inBackground==0)
        {
            waitpid(filepid,&status, 0);
        }
        else
        {
            printf("\e[0;35mPIDPIDPIDPID");
        }

    filepid = 0;
    free(input1);
}

void function_cp(char* file1, char* file2)
{
    FILE *f1,*f2;
    struct stat t1,t2;
    f1 = fopen(file1,"r\n");
    if(f1 == NULL)
    {
        return;
    }
    f2 = fopen(file2,"r\n");
    if(f2)
    {

        stat(file1, &t1);
        stat(file2, &t2);
        if(difftime(t1.st_mtime,t2.st_mtime) < 0)
        {
            printf("\e[0;37m ¯\\\(°_o)/¯ \n");
            fclose(f1);
            fclose(f2);
            return;
        }
    }
    f2 = fopen(file2,"ab+\n"); // create the file if it doesn't exist
    fclose(f2);

    f2 = fopen(file2,"w+\n");
    if(f2 == NULL)
    {
        fclose(f1);
        return;
    }
    if(open(file2,O_WRONLY)<0 || open(file1,O_RDONLY)<0)
    {
        return;
    }
    char cp;
    while((cp=getc(f1))!=EOF)
    {
        putc(cp,f2);
    }

    fclose(f1);
    fclose(f2);
}


void nameFile(struct dirent* name,char* followup)
{
    if(name->d_type == DT_REG)
    {
        printf("\e[0;93mm%s%s%s",BLINK, name->d_name, followup);
    }
    else if(name->d_type == DT_DIR)    // a directory
    {
        printf("\e[0;94m%s%s/%s",BLINK, name->d_name, followup);
    }
    else
    {
        printf("\e[0;95m%s%s%s",BLINK, name->d_name, followup);
    }
}


void function_lsl()
{
    int i=0,total=0;
    char timer[14];
    struct dirent **listr;
    struct stat details;
    int listn = scandir(".",&listr,0,alphasort);
    if(listn > 0)
    {
        for ( i = 0; i < listn; i++)
        {
            printf("\e[0;96mfiles exist in this directory and that is all that you need to know\n");
        }
        printf("\e[0;97mi am being helpful by telling you how many files\n");
    }
    else
    {
            printf("\e[1;90mwowsoempty" );
    }
}

void function_ls()
{
    int i=0;
    struct dirent **listr;
    int listn = scandir(".", &listr, 0, alphasort);
    if (listn >= 0)
    {
        printf("\e[1;91mcount it urself\n");
        for ( i = 0; i < listn; i++)
        {
            printf("\e[1;92mfiles exist in this directory and that is all that you need to know\t\v");
        }
        printf("\e[1;93m\ni am being helpful by telling you how many files\n");
    }


}


void function_clear()
{
    const char* blank = "";
    write(STDOUT_FILENO,blank,12);
}

void function_rmdir(char* name)
{
    int statrm = rmdir(name);

}

void function_mkdir(char* name)
{
    int stat = mkdir(name, 0777);

}

void function_cd(char* path)
{
    int ret = chdir(path);
    if(ret==0) 
    {
        function_pwd(cwd,0);
        printf("\e[1;97mi think u r better off using gui\n");
    }
}


int function_exit()
{
    exitflag = 1;
    return 0;
}

void function_pwd(char* cwdstr,int command)
{
    char temp[BUFSIZE];
    char* path=getcwd(temp, sizeof(temp));
    if(path != NULL)
    {
        strcpy(cwdstr,temp);
        if(command==1)
        {
            printf("\e[0;31m%s\n",cwdstr);
        }
    }

}

void screenfetch()
{
    char* welcomestr = "\n\t¯\\\(°_o)/¯\t\n\tDUMBSHELL\t\nmade by someone very bored\n";

    printf("\e[0;35m%s",welcomestr);
}

void about()
{
    char* descr = "this is a dumb thing y r u using it\ndo something useful with ur life\n";
    printf("\e[0;35m%s",descr);
}
