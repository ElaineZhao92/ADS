#ifndef _STOP_WORD_H_
#define _STOP_WORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 100000
#define MAX_LENPATH 900
#define MAXTOKENS 256
#define MAXLINE 1024
#define MINLEN 3
#define STMINLEN 2


int n_file = -1;

struct Data
{
	char dir[500];
}stack[MAX_DATA];

struct tnode
{
	char *word;
	int count;
	struct tnode *left, *right;
};

void pretreatment(); 

int BuildFileList(); 

int BuildResultList();

struct tnode *talloc(void);

int Search(const char *dir);

char **split(char *, char *);

void freetree(struct tnode *);

struct tnode *addtree(struct tnode *, char *);

struct tnode *findstopword(struct tnode *, char *);

struct tnode *buildstoptree(char *fname, struct tnode *p); 
#endif


