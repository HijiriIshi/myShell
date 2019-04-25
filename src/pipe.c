#include "myshell.h"

//パイプでつながれたコマンドの分割　戻り値はコマンドの数
int DivideByPipe(char *line)
{
	int i;
	char *cmd;
	int count = 1;
	int len;
	
	//パイプの数をカウント
	for(i = 0;line[i] != '\0'; i++)
		if(line[i] == '|')
			count++;

	cmd = strtok(line, "|\n");

	
	if (cmd == NULL)
		return 0;
	commands = (char **)calloc(count, sizeof(char *));
	if(commands == NULL)
		malloc_error();
	
	for(i = 0; i < count; i++)
	{
		len = strlen(cmd) + 1;
		
		commands[i] = (char *)calloc(len, sizeof(char));
		if(commands[i] == NULL)
			malloc_error();
		
		strncpy(commands[i], cmd, len);
		cmd = strtok(NULL, "|\n");
	}
	
	return count;
}

//pipe_fd_outをpipe_fd_inに入れる
void pipefd_out2in()
{
	pipe_fd_in[0] = pipe_fd_out[0];
	pipe_fd_in[1] = pipe_fd_out[1];
}