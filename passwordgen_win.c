#define _CRT_RAND_S
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* GenerateRandomString(int min_length, int max_length, int min_lcase_cnt, int min_ucase_cnt, int min_num_cnt, int min_special_cnt){
  char lc_chars[] = "abcdefgijkmnopqrstwxyz";
  char uc_chars[] = "ABCDEFGHJKLMNPQRSTWXYZ";
  char num_chars[] = "23456789";
  char spec_chars[] = "*$-+?_&=!%%{}/";
  int total_chars = strlen(lc_chars) + strlen(uc_chars) + strlen(num_chars) + strlen(spec_chars);
 
  //Generate a random seed 
  errno_t err;
  unsigned int seed;
  err = rand_s(&seed);

  //Seed has been generated, now we use it to decide the password length
  int length = (rand() % (max_length - min_length + 1)) + min_length;
    
  // Allocate memory for the random string
  char* rand_string = (char*)malloc((length + 1) * sizeof(char)); 
  if (!rand_string) return NULL;
  rand_string[length] = '\0'; 

  int req_char_left = min_lcase_cnt + min_ucase_cnt + min_num_cnt + min_special_cnt;

  for (int i = 0; i < length; i++){
    char selectable_chars[total_chars+1];
    selectable_chars[0] = '\0';
    int selectable_length = 0;

    //Building the selectable characters string
    if (req_char_left < length - i){ //If we have enough space 
      //Include all characters
      strcat(selectable_chars, lc_chars);
      strcat(selectable_chars, uc_chars);
      strcat(selectable_chars, num_chars);
      strcat(selectable_chars, spec_chars);
      selectable_length = total_chars;
    } 
      
    else{ //If we dont have any more space
      //Build the selectable string with only required characters
      if (min_lcase_cnt > 0){
        strcat(selectable_chars, lc_chars);
        selectable_length += strlen(lc_chars);
      }
      if (min_ucase_cnt > 0){
        strcat(selectable_chars, uc_chars);
        selectable_length += strlen(uc_chars);
      }
      if (min_num_cnt > 0){
        strcat(selectable_chars, num_chars);
        selectable_length += strlen(num_chars);
      }
      if (min_special_cnt > 0){
        strcat(selectable_chars, spec_chars);
        selectable_length += strlen(spec_chars);
      }
      
    }

    if (selectable_length == 0){
      free(rand_string);
      return NULL;
    }

    //Select a random character from the selectable chars
    int schars_pos = rand() % selectable_length;
    
    //Assign the selected value to the password string
    rand_string[i] = selectable_chars[schars_pos];

    //Find where the selected char is from, then reduce the requirements 
    if (strchr(lc_chars, rand_string[i]) != NULL) min_lcase_cnt--;
    else if (strchr(uc_chars, rand_string[i]) != NULL) min_ucase_cnt--;
    else if (strchr(num_chars, rand_string[i]) != NULL) min_num_cnt--;
    else min_special_cnt--;

    req_char_left = min_lcase_cnt + min_ucase_cnt + min_num_cnt + min_special_cnt;
  }

  return rand_string;
}

int main() {
  char* random_str = GenerateRandomString(8, 16, 2, 2, 2, 2);
  if (random_str){
    printf("Generated random string: %s\n", random_str);
    free(random_str);
  }
  else
    printf("Error generating random string\n");
 
  return 0;
}