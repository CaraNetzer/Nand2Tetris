#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "JackTokenizer.h"

typedef struct {
    FILE *in_file;
} jack_tokenizer;


jack_tokenizer* open_file(char *in_file_path) {
    jack_tokenizer *tokenizer;
    tokenizer = calloc(1, sizeof(jack_tokenizer));

    if(!tokenizer) {
        perror("calloc failed for open_file");
        exit(1);
    }

    tokenizer->in_file = fopen("test", "r");
    return tokenizer;
}


void tokenizer_execute() {
    char tokens[2000];
    bool block_comment = false;

    // for line in self.in_file:
    //
    //     # skip block comments
    //     if block_comment:
    //         if line.__contains__("*/"):
    //             block_comment = False
    //         continue
    //     if line.strip().startswith("/*"):
    //         block_comment = True
    //         if line.__contains__("*/"):
    //             block_comment = False
    //         continue
    //
    //     # all other lines
    //     if not line.startswith("//"):
    //         line = re.split("(/\\*)|(//)", line)[0].strip()
    //         if line.__contains__("\""):
    //             words = line.split("\"")
    //             self.append_tokens(words[0].split(" "), tokens)
    //             tokens.append(words[1])
    //             self.append_tokens(words[2].split(" "), tokens)
    //         else:
    //             words = line.split(" ")
    //             self.append_tokens(words, tokens)
    //
    //
    // for token in tokens:
    //     self.current_token = token
    //
    //     newToken = Token.Token(token)
    //     self.tagged_tokens.append(newToken)
}
