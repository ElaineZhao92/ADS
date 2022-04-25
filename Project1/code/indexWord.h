#ifndef _INDEX_WORD_H_
#define _INDEX_WORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WORD_LENGTH 20      //max length of a single word
#define MAX_TITLE_LENGTH 100    //max length of a title
#define MAX_ARTICLE_NUMBER 1000 //max number of input articles
#define MAX_WORD_NUMBER 1<<20   //max number of different words after stopping words are deleted
//typedef enum{false, true} bool;
//part of trie tree for indexing the words
struct trie_node;   //struct definition of the trie node
typedef struct trie_node* Tree;
Tree Trie;  //dummy root of the Trie tree
typedef struct trie_node {
    int ID;     //marks the index of a word
                //ID == 0 means it is not the end of a letter
    char key;   //the letter stored in this node
    Tree FirstChild;    //in practice it marks the next letter in a certain word
    Tree NextSibling;   //in practice it marks the other choice in this position of words with the same prefix
} trie_node;
int wordIndex = 0;    //count how many distinct words have appeared in total
int articleCount = 0; //count how many articles have appeared in total
int wordCount = 0;    //count how many words have appeared in a certain article (with repeated words included)
int findIndexOfWord(char word[]);   //search the index of the word
void insertIntoTrie(char word[]);   //insert a new word into the trie tree to build its index

//part of inverted file index
struct article_node;    //linked list for an artile which have a certain word
struct position_node;   //linked list for a word in a certain article
typedef struct article_node* article;
typedef struct position_node* position;
typedef struct article_node{
    int indexOfArticle;     //the title of the article
    int frequency;          //how many times the word occured in the article
    article nextArticle;    //when the word occured in more than one article, it points at the next article
    position firstPosition; //first occurence of the word in this article
} article_node;
typedef struct position_node{
    int place;      //where is the word in this article
    position next;  //next position of the word in a certain article
} position_node;
article wordList[MAX_WORD_NUMBER];       //dummy root for saving the linkedlist of invertedfile
char articleList[MAX_ARTICLE_NUMBER][MAX_TITLE_LENGTH];   //we suppose that there's no more than 10000 articles and each shorter than 300 letters
void ReadTitle(FILE *fp);
void ReadArticle(FILE *fp);     //read a article and establish index of words
void insertIntoArticle(char word[], int index);    //record a new word of an article with its location
//array-search part
#define MAX_SEARCH_WORD 30  //we suppose that there's no more than 10 words within one search
void findPositionOfArray(char word[]);  //find the position of a phrase(or simply a word is OK as well)
char* split(int index[], char word[], bool *isStopWord);   //split a phrase into several words (by calling iteratively)

bool getCommonArticle(article arrayWordList[], int count);    //search whether all words in the phrase appeared in a common article
bool isArticleNull(article arrayWordList[], int count);     //judge whether we have finished searching all articles the word appears
bool allArticleEqual(article arrayWordList[], int count);   //judge whether the pointers in the 'article' array are pointing towards the same atricle
int findMinArticleIndex(article arrayWordList[], int count);//find the min file_index of the articles which the arrays are pointing at

int findAdjPosition(article arrayWordList[], int count);    //search in this article, whether that phrase exists
bool isPositionNull(position positionList[], int count);    //judge whether we have finished the search all positions of a word within an article
bool allPositionEqual(position positionList[], int count);  //judge whether the pointers in the 'position' array are pointing towards the same(adjacent) position
int findMinPosition(position positionList[], int count);    //find the min position of the words which the arrays are pointing at

article findNewArticle(article result, int index, int times_in_this_article); //when we find a new article, insert it into the result linked list in descending order, return the head node
void printResults(article result);		//print the result in descending order, and print at most 50 titles
void titlePrint(char * title);
#endif
