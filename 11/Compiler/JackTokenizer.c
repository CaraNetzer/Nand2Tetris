#include <ctype.h>
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
    tokenizer->next_index = 0;

    tokenizer->in_file = fopen(in_file_path, "r");
    if(!tokenizer->in_file) {
        perror(in_file_path);
        exit(1);
    }

    return tokenizer;
}

void append_token(char *token) {

    // printf("here append token");
    if (0 != strcmp(token, "")) {
        printf("token to be inserted: %s\n", token);
        printf("duplicate of that: %s\n", strdup(token));
        tokenizer->tokens[tokenizer->next_index++] = strdup(token);
        printf("next index: %d\n", tokenizer->next_index);
        printf("token[%d]: %s\n", tokenizer->next_index - 1, tokenizer->tokens[tokenizer->next_index - 1]);
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

//         tokens_and_words = re.split("(\\W)", word)
//         for token in tokens_and_words:
//             if token != '':
//                 token_list.append(token)

}


void tokenizer_execute(jack_tokenizer* tokenizer) {
    // char* tokens[2000];

    
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


            if(strstr(line_position, "\"") != NULL) { // handle string constants differently than other words
                printf("%s", line_position);
            //     words = line.split("\"")
            //     self.append_tokens(words[0].split(" "), tokens)
            //     tokens.append(words[1])
            //     self.append_tokens(words[2].split(" "), tokens)

            } else {
                setlocale(LC_ALL, "C");

                // region: words = line.split(" ")
                char *token = strtok(line_position, " ");
                // char *token = "class";

                while (token != NULL) {
                    char *sub_token = strdup(token);
                    char string_sub_token[BUFSIZ];

                    int token_length = strlen(sub_token);
                    // printf("token length: %d\n", token_length);
                    for(int i = 0; i < token_length + 1; i++) {
                        printf("i: %d\n", i);
                        printf("token[%d] = %c\n", i, token[i]);

                        printf("symbol? %c\n", sub_token[i]);
                        if(!isspace(token[i])) {

                            if (!isalnum(token[i])) {
                                if(0 != strcmp(string_sub_token, "")) {
                                    printf("\tyes\n");
                                    printf("\tstring sub token: %s\n", string_sub_token);
                                    append_token(string_sub_token);
                                }

                                string_sub_token[0] = '\0';

                                if(0 != strcmp(sub_token, "")) {
                                    printf("\tyes\n");
                                    printf("\tsub token: %c\n", *sub_token);
                                    append_token(strndup(sub_token, 1));
                                }
                            } else {
                                strncat(string_sub_token, sub_token, 1);
                                printf("sub token: %s\n", sub_token);
                                printf("string: %s\n", string_sub_token);
                            }
                        }
                        sub_token += 1;
                    }

                    // printf("end\n");
                    token = strtok(NULL, " ");
                    // printf("end 2\n");

                }
                // #endregion
            //     self.append_tokens(words, tokens)

            }
        }
    }

    // for token in tokens:
    //     self.current_token = token
    //
    //     newToken = Token.Token(token)
    //     self.tagged_tokens.append(newToken)
}
