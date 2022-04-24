## 8-3 Safe Fruit (20 分)

There are a lot of tips telling us that some fruits must not be eaten with some other fruits, or we might get ourselves in serious trouble. For example, bananas can not be eaten with cantaloupe (哈密瓜), otherwise it will lead to kidney deficiency (肾虚).

Now you are given a long list of such tips, and a big basket of fruits. You are supposed to pick up those fruits so that it is safe to eat any of them. 

### Input Specification:

Each input file contains one test case. For each case, the first line gives two positive integers: N, the number of tips, and M, the number of fruits in the basket. both numbers are no more than 100.

Then two blocks follow. The first block contains N pairs of fruits which must not be eaten together, each pair occupies a line and there is no duplicated tips; and the second one contains M fruits together with their prices, again each pair in a line. To make it simple, each fruit is represented by a 3-digit ID number. A price is a positive integer which is no more than 1000. All the numbers in a line are separated by spaces.

### Output Specification:

For each case, first print in a line the maximum number of safe fruits. Then in the next line list all the safe fruits, in increasing order of their ID's. The ID's must be separated by exactly one space, and there must be no extra space at the end of the line. Finally in the third line print the total price of the above fruits. Since there may be many different solutions, you are supposed to output the one with a maximum number of safe fruits. In case there is a tie, output the one with the lowest total price. It is guaranteed that such a solution is unique.

### Sample Input:

```c
16 20
001 002
003 004
004 005
005 006
006 007
007 008
008 003
009 010
009 011
009 012
009 013
010 014
011 015
012 016
012 017
013 018
020 99
019 99
018 4
017 2
016 3
015 6
014 5
013 1
012 1
011 1
010 1
009 10
008 1
007 2
006 5
005 3
004 4
003 6
002 1
001 2
```

### Sample Output:

```c
12
002 004 006 008 009 014 015 016 017 018 019 020
239
```

### Grading Policy:

**Programming:** Write the program (10 pts.) **with sufficient comments**.

**Testing:** Provide a set of test cases to fill in a test report (3 pts.). Write analysis and comments (3 pts.).

**Documentation:** Chapter 1 (1 pt.), Chapter 2 (2 pts.), and finally a complete report (1 point for overall style of documentation).