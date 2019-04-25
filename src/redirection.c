#include "myshell.h"

int inRedirection()
{
	int flag = 0;
	flag |= REDIRECT_IN;
	if (token[1] == '\0')
	{
		nextToken();
	}
	else
	{
		//文字列が連結している場合
		token = &(token[1]);
	}
	
	strcpy(inputfile, token);
	
	return flag;
}

int outRedirection()
{
	int flag = 0;
	flag |= REDIRECT_OUT;
	
	if (token[1] == '>')//追記
	{
		flag |= APPEND;
		token = &(token[1]);
	}
	
	if (token[1] == '&')//標準エラー出力
	{
		flag |= STDERROR;
		token = &(token[1]);
	}
	
	if (token[1] == '\0')
	{
		nextToken();
	}
	else
	{
		//文字列が連結している場合
		token = &(token[1]);
	}
	strcpy(outputfile, token);
	
	return flag;
}
