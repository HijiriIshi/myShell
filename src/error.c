/*エラーメッセージ処理*/
#include "myshell.h"
#include "error.h"

void malloc_error()
{
	perror("メモリが確保できません\n");
	exit(1);
}