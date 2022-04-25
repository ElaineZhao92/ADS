//#include "head.h"
#include "stopwords.h"
#include "indexWord.h"

static int n_total_word_st = 0;//total word number (with stopwords, with duplicates) 
static int n_total_word = 0;   //total word number (without stopwords, with duplicates) 
static char b[100];
static int k;
static int k0;
/* j is a general offset into the string */ 
static int j;
struct tnode *root = buildstoptree("..\\documents//testfile&output//stopwords.txt", root);
void stem(int index,int position);
int main()
{
	//Doing pretreatment - stop words filtering & word stemming
	pretreatment();
	
    FILE *fp;
    int i,n;
    char * title_print;
    fp = fopen("..\\documents//testfile&output//resultlist.txt", "r");
    if(!fp)
	{
		printf("| #ERROR# Failed to open resultlist.txt                                           |\n");
		printf("+---------------------------------------------------------------------------------+\n");
		system("pause");
		return -1;
	}
    Trie = (Tree) malloc (sizeof(trie_node));
    Trie->ID = 0;;
    Trie->FirstChild = Trie->NextSibling = NULL;
    ReadTitle(fp);
    fclose(fp);
	
    char title[MAX_TITLE_LENGTH];

    FILE *fpTitle;
    fpTitle = fopen("..\\documents//testfile&output//resultlist.txt", "r");
    printf("+---------------------------------------------------------------------------------+\n"); 
    printf("|                          [SYSTEM] Loading files...                              |\n");
    printf("+---------------------------------------------------------------------------------+\n"); 
    	
    while(!feof(fpTitle))
    {
        fgets(title, MAX_TITLE_LENGTH, fpTitle);
        if(title[strlen(title)-1] == '\n')
            title[strlen(title)-1] = '\0';
        printf("|[SYSTEM] Reading :");
        fp = fopen(title, "r");
        titlePrint(title);
        
                   
        ReadArticle(fp);
        fclose(fp);
    }
    int len_blank;
	int len_figure = 0;
	int tmp;
    //output word count 
    printf("+---------------------------------------------------------------------------------+\n"); 
    printf("|                          [SYSTEM] Word Count Result                             |\n");
    printf("+---------------------------------------------------------------------------------+\n"); 
    printf("| [SYSTEM] With Stop Words  --- There is a total of %d words.", n_total_word_st);
    tmp = n_total_word_st;
    while(tmp > 0)
    {
    	len_figure++;
    	tmp /= 10;
	}
	len_blank  = 23 - len_figure;
	for(int i = 0; i < len_blank; i++) printf(" ");
	printf("|\n");
    printf("| [SYSTEM] Without Stop Words - There is a total of %d words.", n_total_word);
    len_figure = 0;
    tmp = n_total_word;
    while(tmp > 0)
    {
    	len_figure++;
    	tmp /= 10;
	}
	len_blank  = 23 - len_figure;
	for(int i = 0; i < len_blank; i++) printf(" ");
	printf("|\n");
    printf("| [SYSTEM] Distinct Words  ---- There is a total of %d words.", wordIndex);
    len_figure = 0;
    tmp = wordIndex;
    while(tmp > 0)
    {
    	len_figure++;
    	tmp /= 10;
	}
	len_blank  = 23 - len_figure;
	for(int i = 0; i < len_blank; i++) printf(" ");
	printf("|\n");
    
    
    //Build BST of stop words to handle user's input
    printf("+---------------------------------------------------------------------------------+\n");
    printf("|                     [SYSTEM] Building stop words tree...                        |\n");
	printf("|                                 Please wating...                                |\n");    
    //Reading stop words from stopwords.txt
    printf("|                                     Finished!                                   |\n");   
    printf("+---------------------------------------------------------------------------------+\n");
    printf("|                   [SYSTEM] You have completed the inputs!                       |\n");
	printf("+ --------------------------------------------------------------------------------+\n");
	printf("|                      Now you can start your query machine                       |\n");
	printf("|                tip: enter \"exit\"if you want to stop the program.                |\n");
	printf("+ --------------------------------------------------------------------------------+\n");
    char word[MAX_WORD_LENGTH];
    printf("  Enter the word/phrase: ");
    gets(b);
	printf("+ --------------------------------------------------------------------------------+\n");
    while(strcmp(b,"@exit"))
    {	
        findPositionOfArray(b);
        printf("+ --------------------------------------------------------------------------------+\n");
        printf("  Enter the word/phrase:");
        gets(b);
        printf("+ --------------------------------------------------------------------------------+\n");
    }
    
    //When exit, delete stop words tree and print information
    printf("|                     [SYSTEM] Deleting stop words tree...                        |\n");
	printf("|                                 Please wating...                                |\n");
	freetree(root);
	printf("|                                    Finished!                                    |\n"); 
	printf("+---------------------------------------------------------------------------------+\n"); 
    printf("|                         [SYSTEM] Thank you for using!                           |\n");
	printf("+ --------------------------------------------------------------------------------+\n");	
	system("pause");
    return 0;
}
void titlePrint(char * title)
{
	char s[MAX_TITLE_LENGTH];
	int len_s;
	int len_blank;
	strcpy(s,title);
	int i=0,n=strlen(s);
	for(i=0;i<n;i++)
		if(s[i]=='\\')
			break;
	for(j=i;j<=n;j++)
		s[j-i]=s[j+1];
	len_s = strlen(s);
	s[len_s] = '\0';
	printf("%s",s);
	    len_blank = 63 - len_s;
	for(i = len_blank; i > 0; i--) printf(" ");
	printf("|\n");
	return;
}

void ReadTitle(FILE *fp)
{
    if(feof(fp))    return ;
    char input[MAX_TITLE_LENGTH];
    char title[MAX_TITLE_LENGTH];
    int n;
    while(!feof(fp))	//copy from the last char until we get the first '\' 
    {
        fgets(input, MAX_TITLE_LENGTH, fp);
        n = strlen(input);
        if(input[n-1] == '\n')
            input[--n] = '\0';
        while(input[n] != '\\')
        {
            title[n] = input[n];
            n--;
        }
        strcpy(articleList[++articleCount], title+n+1);
    }
    articleCount = 0;
}

void ReadArticle(FILE* fp)
{
    if(feof(fp))    return ; 
    wordCount = 0;  //a new article, and no word has been read
    char word[MAX_WORD_LENGTH];
    articleCount++;

    int index;
    while(!feof(fp))    //while the article is not read thoroughly
    {
        fscanf(fp, "%s", word);      
        wordCount++;    //we have read a new word
        n_total_word++;
        index = findIndexOfWord(word);
        if(!index)  //the word haven't appeared in the trie database
        {
            insertIntoTrie(word);
            index = wordIndex;
        }
        insertIntoArticle(word, index);
    }  
}

//return the index of the word if found
//if not found, return 0
int findIndexOfWord(char word[])
{
    int i;
    Tree P = Trie->FirstChild;  //node Trie is dummy
    Tree parent = P;
    for(i = 0; word[i]; i++)
    {
        while(P && P->key != word[i])
            P = P->NextSibling;     //search whether this letter exists
        if(!P)   return 0;  //when the word's postfix lacks, P is NULL

        parent = P;     //update P's parent
        P = P->FirstChild;  //go to search the next letter
    }

    if(word[i] == '\0') //when the word is a prefix of the other word, P is behind the last letter
        return parent->ID;  //since P moves as we search, it is parent that points towards the last letter
    else return 0;  //not found
}

//insert a NEW word into the trie
void insertIntoTrie(char word[])
{
    Tree P = Trie->FirstChild;  //node Trie is dummy
    Tree ptr, pre, parent = Trie;
    int i, j;
    for(i = 0; word[i]; i++)
    {
        pre = P;
        //search word[i]
        while(P && P->key != word[i])
        {
            pre = P;
            P = P->NextSibling;
        }
        //word[i] doesn't exist
        if(!P)  //the letter not exist in the trie
        {
            ptr = (Tree) malloc (sizeof(trie_node));
            ptr->ID = 0;
            ptr->key = word[i];
            ptr->FirstChild = ptr->NextSibling = NULL;

            if(pre) //add a new letter in a single line
                pre->NextSibling = ptr;
            else parent->FirstChild = ptr;   //we come into a new line
            
            P = ptr;    //go to insert the next letter
        }
        //go to search the next line
        parent = P;
        P = P->FirstChild;
    }
    parent->ID = ++wordIndex;   //update the index
}

void insertIntoArticle(char word[], int index)
{
    article particle;
    position pposition;
    if(wordList[index] == NULL) //the word haven't appeared
    {
        //malloc the dummy root
        wordList[index] = (article) malloc (sizeof(article_node));
        wordList[index]->firstPosition = NULL;
        wordList[index]->nextArticle = NULL;
        wordList[index]->frequency = 0; //in dummy head, we count the number of articles
        wordList[index]->indexOfArticle = 0;
        //malloc the new article
        particle = (article) malloc (sizeof(article_node));
        particle->firstPosition = NULL;
        particle->frequency = 0;
        particle->nextArticle = NULL;
        particle->indexOfArticle = articleCount;
        
        wordList[index]->nextArticle = particle;
        wordList[index]->frequency++;
    }
    else    //the word has appeared, so we need to search whether it appeared in this article
    {
        //travel the linkedlist to test whether the word has appeared in this article
        //since the article is read in order, the article to insert into is always the first node except the dummy head
        if(wordList[index]->nextArticle->indexOfArticle != articleCount)    //the article didn't have this word before
        {
            //insert a new article
            particle = (article) malloc (sizeof(article_node));
            particle->firstPosition = NULL;
            particle->frequency = 0;
            particle->nextArticle = NULL;
            particle->indexOfArticle = articleCount;
            particle->nextArticle = wordList[index]->nextArticle;
            
            wordList[index]->nextArticle = particle;
            wordList[index]->frequency++;
        }
        else    //the article already has this word
            particle = wordList[index]->nextArticle;
    }
    //now we insert the word into the article
    particle->frequency++;
    pposition = (position) malloc (sizeof(position_node));
    pposition->place = wordCount;
    pposition->next = particle->firstPosition;
    particle->firstPosition = pposition;
}

void findPositionOfArray(char word[])
{
    int i = 0;
    while(word[i] == ' ' || word[i] == '\n')	//delete the empty chars
        i++;
    if(word[i] == '\0')
    {
        printf("| #ERROR# Null Input!                                                             |\n");   
        return ;
    }
    
    //turning all the words into lowercase
	i = 0;
	while(word[i] != '\0')
	{
		if(word[i]>='A' && word[i]<='Z') word[i]+=32;
		i++; 
	} 
    
    int index[MAX_SEARCH_WORD]; //store the index of the inputting words
    article arrayWordList[MAX_SEARCH_WORD]; //pointers towards the word records

    int count = 0;  //mark the position after the last word

    //call iteratively and store the index of word into array index
    char* pword = word;
    bool isStopWord; 
    while(pword)
    {
        pword = split(index+(count++), pword, &isStopWord);
        if(isStopWord)  count--;
    }
	
	if(count == 0)
	{
		printf("| #ERROR# The input are all STOP WORDS!                                           |\n");
        return;
	}
	
    for(i = 0; i < count; i++)
    {
        if(!index[i])   //if some words are not in any article, return not found directly
        {
            printf("| #ERROR# Not Found!                                                              |\n");
            return;
        }
        arrayWordList[i] = wordList[index[i]]->nextArticle;     //load the words
    }

    bool found = getCommonArticle(arrayWordList, count);    //find the phrase
    if(!found)
	{
		printf("| #ERROR# Not Found!                                                              |\n");    //specially deal with not found
	}
}

//store the index of the first word in arrat into index[0], and return the pointer to the first letter of the next word
//if no word anymore after split, return NULL
char* split(int index[], char word[], bool *isStopWord)
{
	int i = 0;
    char result[MAX_WORD_LENGTH];
    while(word[i] != ' ' && word[i] == '\n')
        i++;
    //copy the first word
    while(word[i] != ' ' && word[i] != '\0')
    {
        result[i] = word[i];
        i++;
    }
    result[i] = '\0';
    struct tnode *querry = NULL;
	querry = findstopword(root, result);
    if(querry == NULL)
    {
        *isStopWord = 0;
        int n = strlen(result), t;
        strcpy(b, result);
        stem(0, n-1);
        for(t = 0; t <= k; t++)
            result[t] = b[t];
        result[t] = '\0';
        index[0] = findIndexOfWord(result);
    }
    else
    {
    //	printf("%s",result);
		*isStopWord = 1;
	}
    if(word[i] == '\0') return NULL;    //the search array has been fully splited
    else return word+i+1;   //return the pointer to the next word

}

//search the phrase and return whether we succeed
bool getCommonArticle(article arrayWordList[], int count)
{
	article result = NULL;
    bool found = false; //not found at first
    int min_fileindex;
    int i;
    int times_in_this_article;

    while(!isArticleNull(arrayWordList, count))    //in case the last article of a certain word is a common article
    {
        //if the words are not in the same article now, loop until they have the same article
        while(!allArticleEqual(arrayWordList, count))
        {
            min_fileindex = findMinArticleIndex(arrayWordList, count);  //find the minimum one to save time
            for(i = 0; i < count; i++)
            {
                while(arrayWordList[i] && arrayWordList[i]->indexOfArticle > min_fileindex)
                    arrayWordList[i] = arrayWordList[i]->nextArticle;
                //when one word's appearances has been searched thoroughly, we exit the function
                if(!arrayWordList[i])   
                {
                    printResults(result); 
                    return found;       //the search of one word is finished, and no common article following
                }
            }
        }
        //if all the words are in the same article, we go to search whether their position are adjacent
        times_in_this_article = findAdjPosition(arrayWordList, count);  //update the found status

        found = found == true ? true : times_in_this_article;
        if(times_in_this_article > 0)
            result = findNewArticle(result, arrayWordList[0]->indexOfArticle, times_in_this_article);

        for(i = 0; i < count; i++)
            arrayWordList[i] = arrayWordList[i]->nextArticle;   //search whether there's next article having the word array
    }
    printResults(result);	//the memory has already been freed
    return found;
}

//when we find a new article, insert it into the result linked list in descending order
article findNewArticle(article result, int index, int times_in_this_article)
{
    if(result == NULL)	//null list
    {
        result = (article) malloc (sizeof(article_node));
        result->firstPosition = NULL;
		result->nextArticle = NULL;
        result->frequency = 0;	//used to record the number of search results
    }
    article ptr;	//new article node
    ptr = (article) malloc (sizeof(article_node));
    ptr->frequency = times_in_this_article;
    ptr->nextArticle = NULL;
	ptr->firstPosition = NULL;
    ptr->indexOfArticle = index;
    article p = result;
    //now search where to insert into
    while(p->nextArticle && p->nextArticle->frequency > ptr->frequency)
        p = p->nextArticle;
    ptr->nextArticle = p->nextArticle;
    p->nextArticle = ptr;
    result->frequency++;	//find a new article
}

//thresholding, print top at most 50 results
void printResults(article result)
{
    int i = 0;
    int printCount = 0; 
    int len_blank;
    int len_figure;
    article ptr = result->nextArticle;

    while(printCount++ < 50 && ptr)	//have results remaining and no more than 50 results
    {
        if(ptr->frequency > 0)
        {
			printf("| In article %s, for %d times.", articleList[ptr->indexOfArticle], ptr->frequency);
			len_figure = 0;
	        while(ptr->frequency)
	        {
	        	len_figure++;
	        	ptr->frequency /= 10;
			}
	        len_blank = 56 - strlen(articleList[ptr->indexOfArticle]) - len_figure;
			for(i = len_blank; i > 0; i--) printf(" ");
			printf("|\n");
		} 

        result->nextArticle = ptr->nextArticle;
        free(ptr);
        ptr = result->nextArticle;
    }
    while(ptr)	//free the remaining search results
    {
        result->nextArticle = ptr->nextArticle;
        free(ptr);
        ptr = result->nextArticle;
    }
    free(result);	//free the root
}

//judge whether the article array has NULL value
bool isArticleNull(article arrayWordList[], int count)
{
    int i;
    for(i = 0; i < count; i++)
        if(!arrayWordList[i])
            return true;
    return false;
}

//judge whether the article array has the same article
bool allArticleEqual(article arrayWordList[], int count)
{
    int i;
    int index = arrayWordList[0]->indexOfArticle;
    for(i = 0; i < count; i++)
        if(arrayWordList[i]->indexOfArticle != index)
            return false;
    return true;
}

//find the min article index of all
int findMinArticleIndex(article arrayWordList[], int count)
{
    int min = arrayWordList[0]->indexOfArticle;
    int i;
    for(i = 1; i < count; i++)
        min = arrayWordList[i]->indexOfArticle < min ? arrayWordList[i]->indexOfArticle : min;
    return min;
}

//search whether the words have adjacent position
int findAdjPosition(article arrayWordList[], int count)
{
    position positionList[count];
    int i, min_index, times = 0;
    for(i = 0; i < count; i++)
        positionList[i] = arrayWordList[i]->firstPosition;
    //check in case one word's position is the last one
    while(!isPositionNull(positionList, count))
    {
        while(!allPositionEqual(positionList, count))
        {
            min_index = findMinPosition(positionList, count);
            for(i = 0; i < count; i++)
            {
                while(positionList[i] && positionList[i]->place-i > min_index)
                    positionList[i] = positionList[i]->next;
                //one word's position is over, and return the result
                if(!positionList[i])
                    return times;
            }
        }
        //finally we found one¡­¡­
        times++;
        for(i = 0; i < count; i++)
            positionList[i] = positionList[i]->next;
    }
    return times;
}

//judge whether the position array has NULL value
bool isPositionNull(position positionList[], int count)
{
    int i;
    for(i = 0; i < count; i++)
        if(!positionList[i])
            return true;
    return false;
}

//judge whether the position array has the adjacent position
bool allPositionEqual(position positionList[], int count)
{
    int i;
    int index = positionList[0]->place;
    for(i = 0; i < count; i++)
        if(positionList[i]->place != index+i)
            return false;
    return true;
}

//find the min position index of all words
int findMinPosition(position positionList[], int count)
{
    int min = positionList[0]->place;
    int i;
    for(i = 1; i < count; i++)
        min = positionList[i]->place-i < min ? positionList[i]->place-i : min;
    return min;
}


/*It's used to judge whether the letter is consonant, because the word stemming operations have regular rules
which has some relation with whether the letter is consonant.
If the letter is 'a' 'e' 'i' 'o' 'u' then return FALSE
else return TRUE 
*/
static int isConsonant(int index)
{
  switch (b[index]) 
  {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return false;
    case 'y':
      return (index == k0) ? true : !isConsonant(index - 1);
    default:
      return true;
  }
}

/* Measure the number of consonant sequences between
 * `k0` and `j`.  If C is a consonant sequence and V
 * a vowel sequence, and <..> indicates arbitrary
 * presence:
 *
 *   <C><V>       gives 0
 *   <C>VC<V>     gives 1
 *   <C>VCVC<V>   gives 2
 *   <C>VCVCVC<V> gives 3
 *   ....
 */
static int getMeasure()
{
  int position;
  int index;

  position = 0;
  index = k0;

  while (true)
  {
    if (index > j)
      return position;
    if (!isConsonant(index))
      break;
    index++;
  }

  index++;
  while(true)
  {
    while (true)
    {
      if (index > j)
        return position;
      if (isConsonant(index)) 
        break;
      index++;
    }
    index++;
    position++;

    while (true)
    {
      if (index > j)
        return position;
      if (!isConsonant(index)) 
        break;

      index++;
    }
    index++;
  }
}

/* `TRUE` when `k0, ... j` contains a vowel. */
static int vowelInStem()
{
  int index;

  index = k0 - 1;
  while (++index <= j)
  {
    if (!isConsonant(index))
    {
      return true;
    }
  }
  return false;
}

/* `TRUE` when `j` and `(j-1)` are the same consonant. */
static int isDoubleConsonant(int index)
{
  if (b[index] != b[index - 1])
  {
    return false;
  }
  return isConsonant(index);
}

/* `TRUE` when `i - 2, i - 1, i` has the form
 * `consonant - vowel - consonant` and also if the second
 * C is not `"w"`, `"x"`, or `"y"`. this is used when
 * trying to restore an `e` at the end of a short word.
 *
 * `cav(e)`, `lov(e)`, `hop(e)`, `crim(e)`, but `snow`,
 * `box`, `tray`.
 */
static int cvc(int index)
{
  int character;
  if (index < k0 + 2 || !isConsonant(index) || isConsonant(index - 1) || !isConsonant(index - 2))
    return false;
  character = b[index];
  if (character == 'w' || character == 'x' || character == 'y')
    return false;
  return true;
}

/* `ends(s)` is `TRUE` when `k0, ...k` ends with `value`. */
static int ends(const char *value)
{
  int length = value[0];
  /* Tiny speed-up. */
  if (value[length] != b[k])
    return false;
  if (length > k - k0 + 1)
    return false;
  if (memcmp(b + k - length + 1, value + 1, length) != 0)
    return false;
  j = k - length;

  return true;
}

/* `setTo(value)` sets `(j + 1), ...k` to the characters in
 * `value`, readjusting `k`. */
static void setTo(const char *value)
{
  int length = value[0];
  memmove(b + j + 1, value + 1, length);
  k = j + length;
}

/* Set string. */
static void replace(const char *value)
{
  if (getMeasure() > 0)
    setTo(value);

}

/* `step1ab()` gets rid of plurals, `-ed`, `-ing`.
 *
 * Such as:
 *
 *   caresses  ->  caress
 *   ponies    ->  poni
 *   ties      ->  ti
 *   caress    ->  caress
 *   cats      ->  cat
 *
 *   feed      ->  feed
 *   agreed    ->  agree
 *   disabled  ->  disable
 *
 *   matting   ->  mat
 *   mating    ->  mate
 *   meeting   ->  meet
 *   milling   ->  mill
 *   messing   ->  mess
 *
 *   meetings  ->  meet
 */
static void step1ab()
{
  int character;

  if (b[k] == 's') 
  {
    if (ends("\04" "sses"))
    {
      k -= 2;
    } 
    else if (ends("\03" "ies"))
    {
      setTo("\01" "i");
    } 
    else if(b[k - 1] != 's')
    {
      k--;
    }
  }

  if (ends("\03" "eed"))
  {
    if (getMeasure() > 0)
      k--;
      return;
  }
  else if ((ends("\02" "ed") || ends("\03" "ing")) && vowelInStem())
  {
    k=j;
    if (ends("\02" "at"))
    {
      setTo("\03" "ate");
    } 
    else if (ends("\02" "bl"))
    {
      setTo("\03" "ble");
    } 
    else if (ends("\02" "iz"))
    {
      setTo("\03" "ize");
    } 
    else if (isDoubleConsonant(k))
    {
      k--;
      character = b[k];
      if (character == 'l' || character == 's' || character == 'z'|| character == 'r') {
        k++;
      }
    }
    else if (getMeasure() == 1 && cvc(k))
    {
      setTo("\01" "e");
    }
  }
}

/* `step1c()` turns terminal `"y"` to `"i"` when there
 * is another vowel in the stem. */
static void step1c()
{
  if (ends("\01" "y") && vowelInStem())
  {
    b[k] = 'i';
  }
}

/* `step2()` maps double suffices to single ones.
 * so -ization ( = -ize plus -ation) maps to -ize etc.
 * note that the string before the suffix must give
 * getMeasure() > 0. */
static void step2()
{
  switch (b[k - 1])
  {
    case 'a':
      if (ends("\07" "ational"))
      {
        replace("\03" "ate");
        break;
      }
      if (ends("\06" "tional"))
      {
        replace("\04" "tion");
        break;
      }
      break;
    case 'c':
      if (ends("\04" "enci")) {
        replace("\04" "ence");
        break;
      }

      if (ends("\04" "anci"))
      {
        replace("\04" "ance");
        break;
      }

      break;
    case 'e':
      if (ends("\04" "izer"))
      {
        replace("\03" "ize");
        break;
      }
      break;
    case 'l':
      /* --DEPARTURE--: To match the published algorithm,
       * replace this line with:
       *
       * ```
       * if (ends("\04" "abli")) {
       *     replace("\04" "able");
       *
       *     break;
       * }
       * ```
       */
      if (ends("\03" "bli")) {
        replace("\03" "ble");
        break;
      }

      if (ends("\04" "alli")) {
        replace("\02" "al");
        break;
      }

      if (ends("\05" "entli")) {
        replace("\03" "ent");
        break;
      }

      if (ends("\03" "eli")) {
        replace("\01" "e");
        break;
      }

      if (ends("\05" "ousli")) {
        replace("\03" "ous");
        break;
      }

      break;
    case 'o':
      if (ends("\07" "ization")) {
        replace("\03" "ize");
        break;
      }

      if (ends("\05" "ation")) {
        replace("\03" "ate");
        break;
      }

      if (ends("\04" "ator")) {
        replace("\03" "ate");
        break;
      }

      break;
    case 's':
      if (ends("\05" "alism")) {
        replace("\02" "al");
        break;
      }

      if (ends("\07" "iveness")) {
        replace("\03" "ive");
        break;
      }

      if (ends("\07" "fulness")) {
        replace("\03" "ful");
        break;
      }

      if (ends("\07" "ousness")) {
        replace("\03" "ous");
        break;
      }

      break;
    case 't':
      if (ends("\05" "aliti")) {
        replace("\02" "al");
        break;
      }

      if (ends("\05" "iviti")) {
        replace("\03" "ive");
        break;
      }

      if (ends("\06" "biliti")) {
        replace("\03" "ble");
        break;
      }

      break;
    /* --DEPARTURE--: To match the published algorithm, delete this line. */
    case 'g':
      if (ends("\04" "logi")) {
        replace("\03" "log");
        break;
      }
  }
}

/* `step3()` deals with -ic-, -full, -ness etc.
 * similar strategy to step2. */
static void step3()
{
  switch (b[k])
  {
    case 'e':
      if (ends("\05" "icate")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\05" "ative")) {
        replace("\00" "");
        break;
      }

      if (ends("\05" "alize")) {
        replace("\02" "al");
        break;
      }

      break;
    case 'i':
      if (ends("\05" "iciti")) {
        replace("\02" "ic");
        break;
      }

      break;
    case 'l':
      if (ends("\04" "ical")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\03" "ful")) {
        replace("\00" "");
        break;
      }

      break;
    case 's':
      if (ends("\04" "ness")) {
        replace("\00" "");
        break;
      }
      break;
  }
}

/* `step4()` takes off -ant, -ence etc., in
 * context <c>vcvc<v>. */
static void step4()
{
  switch (b[k - 1])
  {
    case 'a':
      if (ends("\02" "al"))
        break;
      return;
    case 'c':
      if (ends("\04" "ance")) {
        break;
      }

      if (ends("\04" "ence")) {
        break;
      }

      return;
    case 'e':
      if (ends("\02" "er")) {
        break;
      }

      return;
    case 'i':
      if (ends("\02" "ic")) {
        break;
      }

      return;
    case 'l':
      if (ends("\04" "able")) {
        break;
      }

      if (ends("\04" "ible")) {
        break;
      }

      return;
    case 'n':
      if (ends("\03" "ant")) {
        break;
      }

      if (ends("\05" "ement")) {
        break;
      }

      if (ends("\04" "ment")) {
        break;
      }

      if (ends("\03" "ent")) {
        break;
      }

      return;
    case 'o':
      if (ends("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't')) {
        break;
      }

      /* takes care of -ous */
      if (ends("\02" "ou")) {
        break;
      }

      return;
    case 's':
      if (ends("\03" "ism")) {
        break;
      }

      return;
    case 't':
      if (ends("\03" "ate")) {
        break;
      }

      if (ends("\03" "iti")) {
        break;
      }

      return;
    case 'u':
      if (ends("\03" "ous")) {
        break;
      }

      return;
    case 'v':
      if (ends("\03" "ive")) {
        break;
      }

      return;
    case 'z':
      if (ends("\03" "ize")) {
        break;
      }
      return;
    default:
      return;
  }

  if (getMeasure() > 1)
  {
    k = j;
  }
}

/* `step5()` removes a final `-e` if `getMeasure()` is
 * greater than `1`, and changes `-ll` to `-l` if
 * `getMeasure()` is greater than `1`. */
static void step5()
{
  int a;

  j = k;

  if (b[k] == 'e') {
    a = getMeasure();

    if (a > 1 || (a == 1 && !cvc(k - 1))) {
      k--;
    }
  }

  if (b[k] == 'l' && isDoubleConsonant(k) && getMeasure() > 1) {
    k--;
  }
}

/* In `stem(i, j)`
 * Typically, `i` is zero and `j` is the offset to the
 * last character of a string, `(b[j + 1] == '\0')`.
 * The stemmer adjusts the characters `b[i]` ... `b[j]`
 * and returns the new end-point of the string, `k`.
 *
 * Stemming never increases word length, so `i <= k <= j`.
 *
 * To turn the stemmer into a module, declare 'stem' as
 * extern, and delete the remainder of this file. */
void stem(int index, int position)
{
  k = position;
  k0 = index;
  /* With this line, strings of length 1 or 2 don't
   * go through the stemming process, although no
   * mention is made of this in the published
   * algorithm. Remove the line to match the published
   * algorithm. */
  step1ab();

  if (k > k0)
  {
    step1c();
    step2();
    step3();
    step4();
    step5();
  }
}

/* read stoplist into binary tree, expects one entry per line */
struct tnode *buildstoptree(char *fname, struct tnode *p)
{
	FILE *fp = {0};
	char line[MAXLINE];
	int len = 0, lcount = 0;

	fp = fopen(fname, "r");
	if(fp == NULL)
	{
  		printf("+---------------------------------------------------------------------------------+\n");
        printf("| #ERROR# Fail to open stopwords.txt                                              |\n");
        printf("+---------------------------------------------------------------------------------+\n");
  		return NULL;
 	} 
	while(fgets(line, MAXLINE, fp) != NULL)
	{
  		len = strlen(line);
  		if(len < STMINLEN)  continue;
		else                lcount++;

		if(line[len - 1] == '\n')  line[--len] = '\0';

		p = addtree(p, line);
 	}
	if(lcount == 0)
	{
  		printf("+---------------------------------------------------------------------------------+\n");
        printf("| #ERROR# Zero stopwords..                                                        |\n");
        printf("+---------------------------------------------------------------------------------+\n");
  		return NULL;
 	}

	fclose(fp);
	return p;
}

/* split string into tokens, return token array */
char **split(char *string, char *delim)
{ 
	//split the whole line into single words(tokens)
	char **tokens = NULL;
	char *working = NULL;
	char *token = NULL;
	int idx = 0;

	tokens  = (char**)malloc(sizeof(char *) * MAXTOKENS);
	if(tokens == NULL)  return NULL;
	
	working = (char*)malloc(sizeof(char) * strlen(string) + 1);
	if(working == NULL)  return NULL;

	/* to make sure, copy string to a safe place */
	strcpy(working, string);
	
	//transfer all the capital letters in string "working" into its lowercase
	int lenwork = strlen(working);
	for(int i = 0 ; i < lenwork; i++)
	{
		if(working[i] >= 'A' && working[i] <= 'Z') working[i] += 32;
	} 
	
	for(idx = 0; idx < MAXTOKENS; idx++)  tokens[idx] = NULL;
	//split string "working" into single words, string "delim" is the punctuations omitted
	token = strtok(working, delim);
	idx = 0;

 /* always keep the last entry NULL terminated */
	while((idx < (MAXTOKENS - 1)) && (token != NULL))
	{
		tokens[idx] = (char*)malloc(sizeof(char) * strlen(token) + 1);
		if(tokens[idx] != NULL)
		{
			strcpy(tokens[idx], token);
			idx++;
   			token = strtok(NULL, delim);
  		}
 	}

	free(working);
	return tokens;
}

/* install word in binary tree */
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if(p == NULL)
	{
		p = talloc();
		p->word = strdup(w);
  		p->count = 1;
  		p->left = p->right = NULL;
 	}
	else if((cond = strcmp(w, p->word)) == 0)  p->count++;
	else if(cond < 0)                          p->left = addtree(p->left, w);
 	else                                       p->right = addtree(p->right, w);

 	return p;
}

/* make new tnode */
struct tnode *talloc(void)
{
	return(struct tnode *)malloc(sizeof(struct tnode));
}

/* find value w in binary tree */
struct tnode *findstopword(struct tnode *p, char *w)
{
	struct tnode *temp;
	int cond = 0;

	temp = p;

	while(temp != NULL)
	{
  		if((cond = strcmp(temp->word, w)) == 0)  return temp;
  		else if(cond > 0)                        temp = temp->left;
  		else                                     temp = temp->right;
 	}

	return NULL;
}

/* free binary tree */
void freetree(struct tnode *p)
{
	if(p != NULL)
	{
		free(p->left);
		free(p->right);
	
		free(p->word);
		free(p);
 	}
}

/* return the result of buidling filelist 0 - failed, 1 -successful*/
int BuildFileList()
{
	FILE*fp;
	//the relative path form code to source files
	char path[MAX_LENPATH] = "..\\documents\\source";
	char *dir = path;
	printf("| [SYSTEM] Begin building filelist...                                             |\n");
		
	Search(dir);
	
	//write menu in fileliset.txt
	fp = fopen("..\\documents\\testfile&output\\filelist.txt", "w"); 
	if (fp == NULL)
	{
		return 0;
	}
	while (n_file > -1)
	{
		fprintf(fp, "..\\%s\n", stack[n_file].dir+3);
		n_file--;
	}
	
	fclose(fp);

	return 1;
}

/* return the result of buidling resultlist 0 - failed, 1 -successful*/
int BuildResultList()

{
	FILE*fp;
	//the relative path form code to result files
	char path[MAX_LENPATH] = "result";
	char *dir = path;

	printf("| [SYSTEM] Begin building resultlist...                                           |\n");
	Search(dir);
	
	//write menu in resultliset.txt
	fp = fopen("..\\documents\\testfile&output\\resultlist.txt", "w"); 
	if (fp == NULL)
	{
		return 0;
	}
	while (n_file > 0)
	{
		fprintf(fp, "..\\documents\\%s\n", stack[n_file].dir);
		n_file--;
	}
	fprintf(fp, "..\\documents\\%s", stack[n_file].dir);
	
	fclose(fp);

	return 1;
}

/* scan the folder pointed by dir recurssively*/
int Search(const char *dir)
{
	long handle;
	struct _finddata_t findData;
	char dirNew[MAX_LENPATH];
	
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");
	
	if ((handle = _findfirst(dirNew, &findData)) == -1L)
	{
		printf("| #ERROR# Failed to find 'findfirst' file!                                        |\n");
		printf("+---------------------------------------------------------------------------------+\n");
	}
	
	while (!_findnext(handle, &findData))
	{
		if (findData.attrib & _A_SUBDIR)
		{
			//exclude .. & . 
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0) continue;
			
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name); 
			// scan recurssively
			Search(dirNew);
		}
		else
		{
			//push current path into the stack
			if (++n_file < MAX_DATA)
			{
				strcpy(stack[n_file].dir , dir);
				strcat(stack[n_file].dir, "\\");
				strcat(stack[n_file].dir, findData.name);
			} 
		} 
	}
	_findclose(handle);
}

void pretreatment() 
{
	printf("+---------------------------------------------------------------------------------+\n");
    printf("|                         [SYSTEM] Doing pretreatment ...                         |\n");
	printf("|                                 Please wating...                                |\n");
	printf("+---------------------------------------------------------------------------------+\n");
 	
 	//delete those char when sccaning
	char *delim = ".,:;`\"+-_(){}[]<>*&^%$#@!?~/|\\= \t\r\n1234567890";
	char **tokens = NULL;
	struct tnode *querry = {0};
	char line[MAXLINE];
	int i = 0;
	
	//build filelist
	int build = BuildFileList();
	if(!build)
	{
		printf("| #ERROR# Failed to build filelist!                                               |\n");
		printf("+---------------------------------------------------------------------------------+\n");
	}
	else printf("| [SYSTEM] Build filelist successfully!                                           |\n");
		 printf("+---------------------------------------------------------------------------------+\n");
	
	//build the BST of stop words
	
	if(root == NULL) 
	{
		printf("| #ERROR# Empty stop words tree!                                                  |\n");
		printf("+---------------------------------------------------------------------------------+\n");
	}
	
	
	// handle files in FOLDER 'source' one by one, output the handled files in FOLDER 'result'
	//change the current work path to FOLDER project
	char path[MAX_LENPATH] = "\0";
	getcwd(path, MAX_LENPATH);
	int lenpath = strlen(path);
	while(path[lenpath-1] != '\\') lenpath--;
	lenpath--;
	path[lenpath] = '\0';
	strcat(path, "\\documents");
	chdir(path); 
		
	//create FOLDER 'result'
	int status = mkdir("result");
	if(status)
	{
		printf("| #WARNNING# FOLDER 'result' already exists! Delete before loading new files.     |\n");
		printf("+---------------------------------------------------------------------------------+\n");
	}
	else 
	{
		printf("| [SYSTEM] Create FOLDER 'result' successfully!                                   |\n");
		printf("+---------------------------------------------------------------------------------+\n");
	}
	
	// handle those files in the order printed in filelist.txt
	FILE *list = fopen("..\\documents//testfile&output//filelist.txt", "r");
	if(!list)
	{
		printf("| #ERROR# Failed to open filelist.txt!                                            |\n");
		printf("+---------------------------------------------------------------------------------+\n");
	}
	fgets(path, MAXLINE, list);
	while(!feof(list))
	{
		// delete '\n' at the end of the line 
		lenpath = strlen(path);
		path[lenpath-1] = '\0';
		lenpath--; 
		
		//open source file
		FILE * fp1 = fopen(path, "r");
		if(!fp1)
		{
			printf("| #ERROR# Failed to open source file!                                             |\n");
		    printf("+---------------------------------------------------------------------------------+\n");
		}
		
		//create the result file which has the same name as the source file
		//resul file name - res_name
		i = lenpath-1;
		while(path[i] != '\\') i--;
		char res_name[80] = "\0";
		char res_addr[80] = "\0";
		strcpy(res_name, path+i+1);
		strcpy(res_addr,"result//");
		strcat(res_addr, res_name);
		
		//write result file
		FILE * fp2 = fopen(res_addr, "w");
		if(!fp2)
		{
			printf("| #ERROR# Failed to build resultlist!                                             |\n");
		 	printf("+---------------------------------------------------------------------------------+\n");
		} 
		
		
		//handle fp1 (when the filelist is not at the end)
		while(!feof(fp1))
		{
			fgets(line, MAXLINE, fp1);
			if(strlen(line) < MINLEN) continue;
			//spilt line into single tokens(words)
			tokens = split(line, delim);
			for(i = 0; tokens[i] != NULL; i++)
			{
				n_total_word_st++;
		   		querry = findstopword(root, tokens[i]);
				
				//if the token is not a stop word, then do word stemming, print it in result file
		   		if(querry == NULL) 
				{
				   	strcpy(b,tokens[i]);
				   	int t = 0, n = strlen(b);
					stem(0,n-1);
					for(t=0;t<=k;t++)
					    fprintf(fp2,"%c",b[t]);

					fprintf(fp2,"%c",32);
				}
		  	}
		
		  	for(i = 0; tokens[i] != NULL; i++) free(tokens[i]);
		  	
		  	fprintf(fp2, "\n");
		  	fgets(line, MAXLINE, fp1);
	 	}
		fclose(fp1);
		fclose(fp2);
		fgets(path, MAXLINE, list);
	}
	printf("| [SYSTEM] Stop words has been filtered!                                          |\n");
	printf("| [SYSTEM] Word stemming is done!                                                 |\n");
	printf("+---------------------------------------------------------------------------------+\n");
	
	//freetree(root);
	
	//build resultlist
	build = BuildResultList();
	if(!build)
	{
		printf("| #ERROR# Failed to build resultlist!                                             |\n");
		printf("+---------------------------------------------------------------------------------+\n");
	}
	else 
	{
		printf("| [SYSTEM] Build resultlist successfully!                                         |\n");
	}
	
	getcwd(path, MAX_LENPATH);
	strcat(path, "//code");
	chdir(path);
}
