#include "myshell.h"

int AnalysisAndExec(int inpipe,int outpipe)
{
	STRLIST *start,*end = NULL,*pt = NULL;
	int argcount = 0;
	int i;
	int len;
	
	//出現フラグ
	int appearflag = 0;
	if(token == NULL)
	{
		printf("command not found\n");
		return 1;
	}
	
	if (inpipe == TRUE)
		appearflag |= PIPE_IN;
	
	appearflag |= outpipe;
	
	//リストの生成＆コマンドを入れる
	len = strlen(token) + 1;
	start = (STRLIST *)malloc(sizeof(STRLIST));
	if (start == NULL)
		malloc_error();
	
	start->str = (char *)calloc(len, sizeof(char));
	if(start->str == NULL)
		malloc_error();
	
	start->next = NULL;
	strcpy(start->str, token);
	end = start;
	argcount++;
	
	nextToken();
	
	while (token != NULL)
	{
		switch (token[0])
		{
			case '<':
				if(appearflag & REDIRECT_IN)
				{
					printf("リダイレクション'<'が複数存在します\n");
					return 1;
				}
				appearflag |= inRedirection();
				break;
			case '>':
				if(appearflag & REDIRECT_OUT)
				{
					printf("リダイレクション'>'が複数存在します\n");
				}
				appearflag |= outRedirection();
				break;
			default://コマンドライン引数
				len = strlen(token) + 1;
				
				//argvリストに追加
				end->next = (STRLIST *)malloc(sizeof(STRLIST));
				if(end->next == NULL)
					malloc_error();
				
				end = end->next;//進める
				end->str = (char *)calloc(len, sizeof(char));
				if(end->str == NULL)
					malloc_error();
				
				strcpy(end->str, token);
				end->next = NULL;
				
				argcount++;
				break;
		}
		nextToken();
	}

	//引数リストを配列にする
	cmdset = (char **)calloc(argcount+ 1, sizeof(char *));
	if(cmdset == NULL)
		malloc_error();
	
	//引数リストの最後はNULLでなければならない。
	cmdset[argcount] = NULL;
	
	for (pt = start, i = 0; pt != NULL;i++)
	{
		len = strlen(pt->str) + 1;
		cmdset[i] = (char *)calloc(len , sizeof(char));
		if(cmdset[i] == NULL)
			malloc_error();
		
		strncpy(cmdset[i], pt->str, len);
		
		free(pt->str);
		
		end = pt;
		pt = pt->next;
		free(end);
	}
	
	CreateProcess(appearflag);
	
	for (i = 0; i < argcount; i++)
	{
		free(cmdset[i]);
	}
	free(cmdset);
	return 0;
}

void initToken(char *line)
{
	token = strtok(line, " \n");
}

void nextToken()
{
	token = strtok(NULL, " \n");
}
