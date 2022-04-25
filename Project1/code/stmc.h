#include  <string.h> 
#define TRUE 1
#define FALSE 0
/*stemming words*/
void stem(int index, int position);
static int isConsonant(int index);
static int getMeasure();
static int vowelInStem();
static int isDoubleConsonant(int index);
static int cvc(int index);
static int ends(const char *value);
static void setTo(const char *value);
static void replace(const char *value);
static void step1ab();
static void step1c();
static void step2();
static void step3();
static void step4();
static void step5();
