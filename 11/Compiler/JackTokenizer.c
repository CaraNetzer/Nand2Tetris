#include <ctype.h>
#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <locale.h>
#include "JackTokenizer.h"

jack_tokenizer *tokenizer;

jack_tokenizer* open_file(char *in_file_path) {
    tokenizer = calloc(1, sizeof(jack_tokenizer));

    if(!tokenizer) {
        perror("calloc failed for open_file");
        exit(1);
    }

    tokenizer->file_path = in_file_path;
    tokenizer->max_tokens = 2000;
    tokenizer->tokens = calloc(tokenizer->max_tokens, sizeof(char*));
    tokenizer->tokenized_tokens = calloc(tokenizer->max_tokens, sizeof(token));
    tokenizer->next_index = 0;

    tokenizer->in_file = fopen(in_file_path, "r");
    if(!tokenizer->in_file) {
        perror(in_file_path);
        exit(1);
    }

    return tokenizer;
}

void append_token(char *token) {

    if (not_equal(token, "")) {
        tokenizer->tokens[tokenizer->next_index++] = strdup(token);
    }

    if(tokenizer->next_index > tokenizer->max_tokens) {
        char **new_tokens = reallocarray(tokenizer->tokens, tokenizer->max_tokens * 2, sizeof(char*));

        if(new_tokens) {
            tokenizer->tokens = new_tokens;
            tokenizer->max_tokens *= 2;
        } else {
            perror(*new_tokens);
            exit(1);
        }
    }
}

void append_tokenized_token(token *tok, int i) {

    if (not_equal(tok->item, "")) {
        tokenizer->tokenized_tokens[i] = tok;
    }

    if(i + 1 > tokenizer->max_tokens) {
        token **new_tokenized_tokens = reallocarray(tokenizer->tokens, tokenizer->max_tokens * 2, sizeof(token));

        if(new_tokenized_tokens) {
            tokenizer->tokenized_tokens = new_tokenized_tokens;
            tokenizer->max_tokens *= 2;
        } else {
            perror("new_tokenized_tokens");
            exit(1);
        }
    }
}

char* process_string_constant(char* line_position) {

    char *string_token = malloc(BUFSIZ);

    if (!strncmp(line_position, "\"", 1)) {
      strncat(string_token, line_position, 1);
      line_position += 1;

      while (strncmp(line_position, "\"", 1)) {
        strncat(string_token, line_position, 1);
        line_position += 1;
      }

      if (!strncmp(line_position, "\"", 1)) {
        strncat(string_token, line_position, 1);
        line_position += 1;
      }

      append_token(string_token);
    }


    return line_position;
}


void loop_through_words(char* line_position) {

  assert(line_position);
  bool string_appended;

  char *line = strdup(line_position);

  // region: words = line.split(" ")
  char *token = strtok(line_position, " ");

  while (token != NULL) {
    string_appended = false;
    char *sub_token = strdup(token);
    char string_sub_token[BUFSIZ];

      int token_length = strlen(sub_token);
      for(int i = 0; i < token_length + 1; i++) {

        if(!isspace(token[i])) {

          if(!strncmp(sub_token, "\"", 1)) { //tokenizes string including double quotation marks
            char *position_before, *position_after;
            int delta;
            position_before = line + (token - line_position) + i;
            position_after = process_string_constant(position_before);
            delta = position_after - position_before;
            i += delta;
            token = strtok(token + i, " ");
            string_appended = true;
          }
          else if (!isalnum(token[i])) {

            // string_sub_token has been built up
            if(not_equal(string_sub_token, "")) {
              append_token(string_sub_token);
            }

            string_sub_token[0] = '\0';

            // all special characters are added individually
            // 0 ascii is null character
            if(not_equal(sub_token, "") && not_equal(sub_token, "\n") && not_equal(sub_token, "\r\n") && 0 != token[i]) {
              append_token(strndup(sub_token, 1));
            }
            sub_token += 1;
          }
          else {
            strncat(string_sub_token, sub_token, 1);
            sub_token += 1;
          }

        }
      }

      if(!string_appended) {
        token = strtok(NULL, " "); // need to pass NULL to strtok after first call
      }
  }
  // #endregion
}

void tokenizer_execute(jack_tokenizer* tokenizer) {

    bool block_comment = false;

    char line[BUFSIZ];
    char* line_position;

    while(fgets(line, BUFSIZ - 1, tokenizer->in_file)) {

        line_position = line;

        // skip block comments
        if(block_comment) {
            if(strstr(line_position, "*/") != NULL) {
               block_comment = false;
            }
            continue;
        }

        while (isspace(*line_position)) { //trim
            line_position += 1;
        }

        if(strncmp("/*", line_position, 2) == 0) {
            block_comment = true;
            if(strstr(line_position, "*/") != NULL) {
               block_comment = false;
            }
            continue;
        }

        if(strncmp("//", line_position, 2) != 0) { // skip lines that are single line comments
            if(strstr(line_position, "/*") != NULL) {
                char* beginning_of_comment = strstr(line_position, "/*");
                *beginning_of_comment = '\0';
            }
            if(strstr(line_position, "//") != NULL) {
                char* beginning_of_comment = strstr(line_position, "//");
                *beginning_of_comment = '\0';
            }

            // create tokenizer->tokens list
            loop_through_words(line_position);

        }
    }


    for (int i = 0; i < tokenizer->next_index; i++) {
        token *tokenized_token = create_token(tokenizer->tokens[i]);
        append_tokenized_token(tokenized_token, i);
    }

    tokenizer->next_index = 0;
}
