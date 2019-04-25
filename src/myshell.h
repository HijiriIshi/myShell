#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/times.h>
#include "error.h"

#define TRUE 1
#define FALSE 0

//行の最大文字数
#define MAX_LINE 1024

//同時に扱えるプロセス数
#define PIPE_BUFF_SIZE 1024

#define CANT_CREATE -1
#define REDIRECT_IN 0x1
#define REDIRECT_OUT 0x2
#define APPEND 0x20
#define STDERROR 0x40
#define STDOUT 0x80
#define PIPE_IN 0x8
#define PIPE_OUT 0x10
#define PIPE 0x4

//------myshell.c---------------
void myshell();
void cmdcase(int cmdnum, char *cd);
void signal_handler();
void Dispose();

//------buildin.c---------------
void buildin_cd();
void buildin_pushd(char *cd);
void buildin_popd(char *cd);

//------analysis.c--------------
int AnalysisAndExec(int inpipe, int putpipe);
void initToken(char *line);
void nextToken();

//------redirection.c-----------
int inRedirection();
int outRedirection();

//------pipe.c------------------
int DivideByPipe(char *line);
void pipefd_out2in();

//------processmanager.c--------
int CreateProcess(int flag);
void connectRdirectionIn();
void connectRdirectionOut(int flag);
void connectPipeIn();
void connectPipeOut(int flag);

//------stack.c-----------------
void Push(const char* data);
int Pop(char *out);
void StackClear();
void PrintStack();

//実行中のプロセス数
int processcount;
//活性状態のプロセスid
int activeprocessId;

//パイプで区切られたコマンド
char **commands;

//トークン
char *token;

//コマンドと引数の配列セット
char **cmdset;

char inputfile[MAX_LINE];
char outputfile[MAX_LINE];

//パイプ
int pipe_fd_in[2];
int pipe_fd_out[2];

struct rusage ru;

struct strlist
{
	char *str;
	struct  strlist *next;
};

typedef struct strlist STRLIST;