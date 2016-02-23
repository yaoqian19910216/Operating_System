/* getword.h - header file for the getword() function used in
   CS570 Fall 2015
   Instructor Carroll, due date 0916
   Qian Yao   Red ID 815260362
   San Diego State University
*/

#include "getword.h"
int getword(char *w){
   char c;
   int n = 0;
   int flag = 1;
   int i;
   for (i = 0; i < STORAGE; i++)
          w[i] = '\0';
   /*char s[STORAGE];*/
  
   while((c = getchar())!= EOF){
          if(c == ' ' && n == 0){
             continue;
          } /* ignoring blanks at the beginning of file*/
          if(c == ' ' && n != 0){
              ungetc(c,stdin);
              return n;
          }/* identify the metacharacter blank, if met it, return the size of existing character*/
 /*         if(c == '\n' && n == 0){*/
   /*           printf("%d,%s",n,s);*/
       /*       return n;*/
          /*}if met the new line character and there is no word, we should return 0*/
          if(c == '\n'){/*if met the new line character, and there is word, we should return the size of word*/
     /*         printf("%d,%s",n,s);*/
             if(n != 0){
                ungetc(c, stdin);
             }
              return n;
          }
          if(c == '\\'){/*if met back slash sign, we need to check whether the word before it is nonmetacharacter*/
             if(flag != 1){
                 ungetc(c, stdin);
                 return n;
             }
             else if ((c = getchar()) == EOF || c == '\n'){/*if the word before it is nonmetacharacter, then we need to check what word is following it, if there are end of file or new character, we should ignore it*/
                 ungetc(c, stdin);
                 continue;
             }
             else{/*if the word before it and following it are both nonmetacharacter, we should add it*/
                 w[n] = c;
                 n++;
                 continue;
             }
          }
          if((c == '>' || c == '<' || c == '&' || c == '|') && n == 0){/*if the metacharacter is the first in this word, we should keep it, and change the flag to 0*/
             flag = 0;
             w[n] = c;
             n++;
             continue;
          }
          if((c == '>' || c == '<' || c == '&' || c == '|') && flag == 0){/*if the word before the current metacharacter is metacharacter, we should keep it*/
             w[n] = c;
             n++;  
             continue;
          }
          if((c == '>' || c == '<' || c == '&' || c == '|') && flag == 1){/*if the word before the current metacharacter is nonmetacharacter, we should stop it, return the size of character, and restart.*/
             ungetc(c,stdin);
             return n;
          }
          if(c != '>' && c != '<' && c != '&' && c != '|' && flag == 0){/*if the word before the current nonmetacharacter is metacharater, we should stop it, return the size of character, unget, the current one*/
             ungetc(c,stdin);
             return n;
          }
/*          s[n] = c;*/
         w[n] = c; 
          n++;

         if(n == STORAGE - 1) {
             n = 0;
             return STORAGE - 1;
          }
}
   if(n > 0){
       /*printf("%d,%s",n,s);*/
   return n;
   }else{
      /* printf("%d,%s",-1,s);*/
   return -1;
   }
}












