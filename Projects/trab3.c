/*Joao Guilherme Araujo - 9725165							 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-----------------Trie and its functions--------------------*/
//Trie data structure definition
struct Trie{
  char ending; //1 if a terminating character of a word
  struct Trie *node[26];
};
typedef struct Trie trie;

//Trie constructor
trie *create(){
  trie *ret = calloc(1, sizeof(trie));
  return ret;
}

//Trie destructor
void destroy(trie *t){
  for(int i = 0; i < 26; ++i)
  if((t->node)[i] != NULL)
  destroy((t->node)[i]);
  free(t);
}

//Insert word in Trie
void insert(trie *dict, char *word){
  int pos = (word[0] <= 'Z')?(word[0] - 'A'):(word[0] - 'a');
  if((dict->node)[pos] == NULL) (dict->node)[pos] = create();
  if(word[1] == '\0') (dict->node)[pos]->ending = 1;
  else  insert((dict->node)[pos], word + 1);
}

//Searches word in Trie
//Returns 1 if found, 0 otherwise
int search(trie *dict, char *word){
  int pos = (word[0] <= 'Z')?(word[0] - 'A'):(word[0] - 'a');
  if((dict->node)[pos] == NULL) return 0;
  if(word[1] == '\0') return (dict->node)[pos]->ending;
  return search((dict->node)[pos], word + 1);
}


/*-----------------Auxiliar function--------------------*/
//Removes non alphabetic characters from word
void clean_word(char **word){
  for(int i = 0; (*word)[i] != '\0'; ++i){
    if(!isalpha((*word)[i])){
      (*word)[i] = '\0';
      break;
    }
  }
}


/*------------Dictionary and Json processing------------*/

//Loads Dictionary to trie
void load_dict(trie *dict, FILE *in){
  char *word, *token;
  word = calloc(128, sizeof(char));
  char delimeters[64] = " _(),!#\'-\".@?&“0123456789"; //delimeters for strtok
  while(fscanf(in, "%s", word) != EOF){ //reads each word of dictionary
    token = strtok(word, delimeters); //breaks compoud words
    while (token != NULL){
      clean_word(&token);
      if(isalpha(token[0]))
        insert(dict, token);
      token = strtok(NULL, delimeters);
    }
  }
  free(word);
}

void correct_tweets(trie *dict, FILE *in){
  size_t size = 0;
  int has_tweets = 0;
  char *line, *pos, *word;
  char needle[16] = "text\":"; //tag to search
  char delimeters[64] = " _(),!#\'-\".@?&“0123456789"; //delimeters for strtok
  while(getline(&line, &size, in) != -1){ //reads all the json's lines
    pos = strstr(line, needle);
    if(pos != NULL){ //if the text tag was found
      has_tweets = 1;
      pos += 8; //jumps to beggining of tweet
      word = strtok(pos, delimeters); //breaks line in constituent words
      while(word != NULL){
        clean_word(&word);
        if(isalpha(word[0]) && search(dict, word) == 0) //if it's a word and it's not in the dictionary
          printf("%s ", word); //print it
        word = strtok(NULL, delimeters);
      }
      printf("\n");
    }
  }
  if(!has_tweets) printf("No tweets to check\n");
  if(line != NULL)  free(line);
}

int main(){
  trie *dict;
  char aux[32];
  scanf("%s", aux);
  FILE *in;
  dict = create();
  in = fopen(aux, "r");
  load_dict(dict, in);
  fclose(in);

  scanf("%s", aux);
  in = fopen(aux, "r");
  correct_tweets(dict, in);
  fclose(in);
  destroy(dict);

  return 0;
}
