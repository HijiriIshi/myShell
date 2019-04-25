#include "myshell.h"

#define N 8


STRLIST* top = NULL;

void Push(const char* data) {
	STRLIST *newnode;
	int len;
	len = strlen(data) + 1;
	
	newnode = (STRLIST *)malloc(sizeof(STRLIST));
	if(newnode == NULL)
		malloc_error();
		
	newnode->str = (char *)calloc(len, sizeof(char *));
	if(newnode->str == NULL)
		malloc_error();
	
	strncpy(newnode->str , data, len);
	newnode->next = NULL;
	
	if(top == NULL)
		top = newnode;
	else
	{
		newnode->next = top;
		top = newnode;
	}
}

void PrintStack()
{
	STRLIST *now;
	int i = 0;
	
	if(top == NULL)
	{
		printf("-myshell: ディレクトリ・スタックが空です\n");
		return;
	}
	
	now = top;
	
	while(now != NULL)
	{
		printf("%d:%s\n", i++, now->str);
		now = now->next;
	}
}

void freeNode(STRLIST *node)
{
	free(node->str);
	free(node);
}

int Pop(char *out) {
	STRLIST *old;
	
	if (top == NULL)
	{
		printf("-myshell: pophd: ディレクトリ・スタックが空です\n");
        return FALSE;
    }
	
	old = top;
	strncpy(out , top->str, strlen(top->str) + 1);
	top = top->next;
	
	freeNode(old);
	
	printf("%s\n",out);
	return TRUE;
}

void StackClear()
{
	STRLIST *now;
	while(top != NULL)
	{
		now = top->next;
		freeNode(top);
		top = now;
	}
}