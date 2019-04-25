//プロセスの生成
#include "myshell.h"

int CreateProcess(int flag)
{
	static int forknum;//生成した子プロセスの個数
	int index = 0;
	int status;
	int pid;

	signal(SIGCHLD, SIG_IGN);
	
	forknum++;

	pid =  fork();
		
	if (pid < 0)
	{
		perror("-myshell: forkに失敗しました\n");
		return CANT_CREATE;
	}
	else if (pid == 0)//子プロセ
	{
		if(flag & REDIRECT_IN)//入力リダイレクション
		{
			connectRdirectionIn();
		}
		
		if(flag & REDIRECT_OUT)//出力リダイレクション
		{
			connectRdirectionOut(flag);
		}
		
		if (flag & PIPE_IN)
		{
			connectPipeIn();
		}
		
		if (flag & PIPE_OUT)
		{
			connectPipeOut(flag);
		}
			
		execvp(cmdset[0], cmdset);
		printf("-myshell: %s: command not found\n", cmdset[0]);
		_exit(0);
	}
	else//親プロセス
	{
		activeprocessId = pid;
		//親プロセスのパイプをクローズ１
		if (flag & PIPE_IN)
		{
			close(pipe_fd_in[0]);
			close(pipe_fd_in[1]);
		}
		//子プロセス終了まで待つ(最後のコマンドのみ)
		if(!(flag & PIPE_OUT))
		{
			waitpid(pid, &status, 0);
		}
	}


	
	return index;
}

void connectRdirectionIn()
{
	int file_fd;
	
	file_fd = open(inputfile, O_RDONLY);
	if(file_fd < 0)
	{
		perror("open inputfile");
		close(file_fd);
		exit(1);
	}
	
	close(0);
	if (dup2(file_fd, 0) < 0)
	{
		perror("dup2 input");
		close(file_fd);
		_exit(1);
	}
	
	close(file_fd);
}

void connectRdirectionOut(int flag)
{
	int file_fd;
	int out = 1;

	if(flag & APPEND)//続きに書く
		file_fd = open(outputfile, O_WRONLY | O_CREAT | O_APPEND , S_IRWXU);
	else 
		file_fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	
	if(flag & STDERROR)//標準エラー出力
		out = 2;

	if(file_fd < 0)
	{
		perror("open outputfile");
		close(file_fd);
		exit(1);
	}
	
	close(out);
	if (dup2(file_fd, out) < 0)
	{
		perror("dup2 output");
		close(file_fd);
		_exit(1);
	}
	
	close(file_fd);
}

void connectPipeIn()
{
	close(pipe_fd_in[1]);
	close(0);
	if (dup2(pipe_fd_in[0], 0) < 0)
	{
		perror("dup2 pipeinfdasfs");
		exit(1);
	}
	close(pipe_fd_in[0]);
}

void connectPipeOut(int flag)
{
	int out = 1;
	
	if(flag & STDERROR)//標準エラー出力
		out = 2;
	
	close(pipe_fd_out[0]);
	
	close(out);
	if (dup2(pipe_fd_out[1], out) < 0)
	{
		perror("dup2 pipeout");
		exit(1);
	}
	close(pipe_fd_out[1]);
}
