#include "myshell.h"

int main(int argc, char *argv[])
{
	struct sigaction sa;
	//Ctr+cをフック
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = signal_handler;
    	sa.sa_flags |= SA_RESTART;
	if( sigaction( SIGINT, &sa, NULL ) < 0)
	{
		perror("sigaction");
    }
	activeprocessId = 1;
	processcount = 0;
	
	myshell();

	return 0;
}

void myshell()
{
	int i;
	char line[MAX_LINE];
	char cd[MAX_LINE];
	int cmdnum;
	
	while (TRUE)
	{
		getcwd(cd, sizeof(cd));
		printf("%s :%s> ", getlogin(), strrchr(cd, '/'));
		clearerr(stdin);
		if(fgets(line, sizeof(line), stdin) == NULL)
		{
			if(feof(stdin))
			{
				clearerr(stdin);//EOFを読み飛ばす
				printf("exit\n");
				Dispose();//とりあえず終了
			}
			else
				printf("myshell: input error\n");
			continue;
		}
		
		//パイプでコマンドを分ける
		cmdnum = DivideByPipe(line);
		if(cmdnum == 0) continue;
		cmdcase(cmdnum, cd);
		
		//解放
		for(i = 0;i < cmdnum;i++)
			free(commands[i]);
		free(commands);
	}
}

void cmdcase(int cmdnum, char *cd)
{
	int i;
	int pipein,pipeout;
	
	for (i = 0;i < cmdnum; i++)
	{
		initToken(commands[i]);
		if(token == NULL)continue;
		if(strcmp(token, "exit") == 0)
			Dispose();
		else if(strcmp(token, "cd") == 0) //ビルドインcdコマンド
			buildin_cd();
		else if(strcmp(token, "pushd") == 0)//pushd実装
			buildin_pushd(cd);
		else if(strcmp(token, "popd") == 0)
			buildin_popd(cd);
		else
		{
			if(token[0] == '&')//パイプの標準エラー出力記号は読みとばす
			{
				nextToken();
				if (token == NULL) continue;
			}
			
			pipein = FALSE;
			pipeout = 0;
			
			//入力パイプが存在=先頭コマンド以降
			if (i > 0)
			{
				pipefd_out2in();
				pipein = TRUE;
			}
			
			
			//自分以降にもパイプでつながれたコマンドが存在する場合、出力用パイプを作成
			if (cmdnum - 1 > i)
			{
				if (pipe(pipe_fd_out) < 0)
				{
					perror("pipe");
					exit(1);
				}
				pipeout |= PIPE_OUT;
				if(commands[i + 1][0] == '&')//次のコマンドの先頭が&だったら標準エラー出力へのパイプ
					pipeout |= STDERROR;
			}
			
			AnalysisAndExec(pipein,pipeout);
		}
	}
	
}

//Ctr+cの処理
void signal_handler()
{
	//子プロセスのキル
	if(activeprocessId != 1)
		kill(activeprocessId, 1);
	activeprocessId = 1;//1ならばkillしない
}

//終了処理
void Dispose()
{
	StackClear();//pushhdスタックのクリア
	exit(0);
}
