//ビルドインコマンド
#include "myshell.h"

void buildin_cd()
{
	nextToken();
	if(chdir(token) == -1)
		printf("-myshell: cd: ディレクトリが正しくありません\n");
}

void buildin_pushd(char *cd)
{
	nextToken();
	if(token != NULL)
	{
		if(chdir(token) == -1)
			printf("-myshell: pushd: ディレクトリが正しくありません\n");	
		else
		{
			Push(cd);//カレントディレクトリをスタックに積む
		}
	}
	PrintStack();
}

void buildin_popd(char *cd)
{
	if(Pop(cd))
		chdir(cd);
}