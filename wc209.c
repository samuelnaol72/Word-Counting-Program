/*
 * Program: wc209.c
 * Student Name: Naol Samuel Erega
 * Student ID: 20210889
 * 
 * This program is designed to count the number of characters, words, 
 * and lines in an input text while handling multiple types of comments 
 * (single-line and multi-line comments).
 * 
 * The finite state machine (FSM) approach is used, where the program 
 * transitions between states like SPACE, WORD, SINGLE_COMMENT, MULTI_COMMENT and SLASHes of
 * two kind(GEN_SLASH & SLASH_AFTER_WORD) based on the character being processed. The program 
 * keeps track of whether a multi-line comment is terminated or not using a flag variable.
 * 
 * The program starts by reading the input character by character, updating 
 * the current state accordingly and counting the characters, words, and lines 
 * at each step. If an unterminated multi-line comment is found, an error message 
 * is printed showing the line number where the issue occurred.
 */

/********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/********************************************************************************/

/********************************************************************************/
enum STATES {MULTI_COMMENT,SINGLE_COMMENT,SPACE,WORD,GEN_SLASH,SLASH_AFTER_WORD};
enum STATES current_state;
int count_char , count_word , count_line ;
int i=1;
int* flag=&i; /*Default value of flag is true otherwise
                it will be false to keep track of whether the
                block comment is terminated or not*/
int unterminated_line; // To print line number in unterminated block comment case
/********************************************************************************/


/********************************************************************************/
/*Helper Functions Declaration*/
enum STATES gen_slash(int c);
enum STATES slash_after_word(int c);
enum STATES space(int c);
enum STATES word(int c);
enum STATES single_comment(int c);
enum STATES multi_comment(int c);
/********************************************************************************/

int main() {
    count_char=0 , count_word=0 , count_line=0 ;
    int c = getchar();
    // State initialization using the first character.
    if (c == EOF) {
        printf("%d %d %d\n", count_line, count_word, count_char);
        return EXIT_SUCCESS;
    } 
    else if (isspace(c)) {
        count_char++;
        if (c == '\n')  count_line++;
        current_state = SPACE;
    } 
    else if (c == '/') {
        count_char++; count_word++;
        current_state = GEN_SLASH;
    } 
    else {
        count_char++; count_word++;
        current_state = WORD;
    }
    // State transitions control.
    while ((c = getchar()) != EOF) {
        switch (current_state) {
            case GEN_SLASH:
                current_state = gen_slash(c);
                break;
            case SLASH_AFTER_WORD:
                current_state = slash_after_word(c);
                break;
            case SPACE:
                current_state = space(c);
                break;
            case WORD:
                current_state = word(c);
                break;
            case MULTI_COMMENT:
                current_state = multi_comment(c);
                break;
            case SINGLE_COMMENT:
                current_state = single_comment(c);
                break;
        }
    } 
    if (*flag==1) printf("%d %d %d\n", count_line, count_word, count_char);
    else { //Unterminated comment
        fprintf(stderr, "Error: line %d: unterminated comment\n", unterminated_line);
        return EXIT_FAILURE; }
    return EXIT_SUCCESS;
}

/* Defnition of functions of each state*/

/********************************************************************************/
enum STATES gen_slash(int c) {
    if (c == '*') {
        count_word--;
        *flag=0;
        return MULTI_COMMENT;
    } else if (c == '/') {
        count_char--; count_word--;
        return SINGLE_COMMENT; 
    } else if (isspace(c)) {
        count_char++;
        if (c == '\n')  count_line++;
        return SPACE;
    } else {
        count_char++;
        return WORD;
    }
}
/********************************************************************************/

/********************************************************************************/
enum STATES slash_after_word(int c) {
    if (c == '*') {
        *flag=0; // termination flag is false up on entering multi-comment state
        return MULTI_COMMENT;
    } else if (c == '/') {
        count_char--; 
        return SINGLE_COMMENT;
    } else if (isspace(c)) {
        count_char++;
        if (c == '\n')  count_line++;
        return SPACE;
    } else {
        count_char++; 
        return WORD;
    }
}
/********************************************************************************/

/********************************************************************************/
enum STATES space(int c) {
    if (c == '/') {
        count_char++; count_word++;
        return GEN_SLASH;
    } else if (isspace(c)) {
        count_char++;
        if (c == '\n')  count_line++;
        return SPACE;
    } else {
        count_char++; count_word++;
        return WORD;
    }
}
/********************************************************************************/

/********************************************************************************/
enum STATES word(int c) {
    if (c == '/') {
        count_char++; 
        return SLASH_AFTER_WORD;}
    else if (isspace(c)) {
        count_char++;
        if (c == '\n')  count_line++;
        return SPACE;
    } else {
        count_char++;
        return WORD;
    }
}
/********************************************************************************/

/********************************************************************************/
enum STATES single_comment(int c) {
    if (c == '\n') {
        count_line++;count_char++;
        return SPACE;
    } else {
        return SINGLE_COMMENT;
    }
}
/********************************************************************************/

/********************************************************************************/
enum STATES multi_comment(int c) {
    unterminated_line=count_line+1;
    char prev_char = c;
    if (prev_char== '\n') {
            count_line++; count_char++;
        }
    while ((c = getchar()) != EOF && *flag!=1 ) {
        if (prev_char == '*' && c == '/') {
            *flag =1; // Multi-comment is terminated
        }
        if (c == '\n') {
            count_line++; count_char++;
        }
        prev_char = c;
    }
    if(c==EOF) return MULTI_COMMENT;
    else if (c == '/') {
            count_char++; count_word++;
            return GEN_SLASH;
    }   
    else if(isspace(c)){
        count_char++;
        if(c=='\n')  count_line++;
        return SPACE;
    }
    else{
        count_char++;count_word++;
        return WORD;
    }
}
/********************************************************************************/
