#ifndef _INDEX_WORD_H_
#define _INDEX_WORD_H_
#define TRUE 1
#define FALSE 0

#define MAXTOKENS 256
#define MAXLINE 1024
#define MINLEN 3
#define STMINLEN 2
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WORD_LENGTH 30
#define MAX_TITLE_LENGTH 100
#define MAX_ARTICLE_NUMBER 1000
#define MAX_WORD_NUMBER 1<<20
//part of trie tree
struct trie_node;
typedef struct trie_node* Tree;
Tree Trie;  //dummy root of the Trie tree
typedef struct trie_node {
    int ID; //ID == 0 means it is not the end of a letter
    char key;
    Tree FirstChild;
    Tree NextSibling;
} trie_node;
int wordIndex = 0;    //count how many words have appeared in total
int articleCount = 0; //count how many articles have appeared in total
int wordCount = 0;    //count how many words have appeared in a certain article
int findIndexOfWord(char word[]);    //search the index of the word
void insertIntoTrie(char word[]);

//part of inverted file index
struct article_node;
struct position_node;   //linked list for a word in a certain article
typedef struct article_node* article;
typedef struct position_node* position;
typedef struct article_node{
    int indexOfArticle;    //the title of the article
    int frequency;      //how many times the word occured in the article
    article nextArticle;    //when the word occured in more than one article, it points at the next article
    position firstPosition; //first occurence of the word in this article
} article_node;
typedef struct position_node{
    int place;      //where is the word in this article
    position next;  //next position of the word in a certain article
} position_node;
article wordList[MAX_WORD_NUMBER];       //dummy root for saving the linkedlist of invertedfile
char articleList[MAX_ARTICLE_NUMBER][MAX_TITLE_LENGTH];   //we suppose that there's no more than 10000 articles and each shorter than 300 letters
void ReadArticle(FILE *fp);     //read a article and establish index of words
void insertIntoArticle(char word[], int index);    
void findPositionOfWord(char word[]);

#define MAX_SEARCH_WORD 10  //we suppose that there's no more than 10 words within one search
#define END_MARK -1
#define INF 1<<30
void findPositionOfArray(char word[]);
static char* split(int index[], char word[]);
bool isArticleNull(article arrayWordList[], int count);
bool getCommonArticle(article arrayWordList[], int count, int result[]);
bool allArticleEqual(article arrayWordList[], int count);
int findMinArticleIndex(article arrayWordList[], int count);
bool findAdjPosition(article arrayWordList[], int count);
bool isPositionNull(position positionList[], int count);
bool allPositionEqual(position positionList[], int count);
int findMinPosition(position positionList[], int count);

#endif

/*build index for the passage*/
int Search(char word[]);
void insert(char word[]);

#ifndef _STOP_WORD_H_
#define _STOP_WORD_H_
/*stop words*/ 
struct tnode
{
	char *word;
	int count;
	struct tnode *left, *right;
};
struct tnode *buildstoptree(char *fname, struct tnode *p); 
struct tnode *addtree(struct tnode *, char *);
struct tnode *findstopword(struct tnode *, char *);
struct tnode *talloc(void);
void freetree(struct tnode *);
char **split(char *, char *);
#endif
