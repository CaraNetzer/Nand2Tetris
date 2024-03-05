import sys
import os
import re
import JackTokenizer
import CompilationEngine

def main():
    in_file_path = sys.argv[1:][0]

    if os.path.isdir(in_file_path):
        files = os.listdir(in_file_path)

        for file_name in files:
            if file_name.endswith(".jack"):
                process_file(file_name, os.path.dirname(in_file_path))

    else:
        file_name = os.path.basename(in_file_path)
        process_file(file_name, os.path.dirname(in_file_path))


def process_file(in_file_name, in_dirname):
    out_file_name = in_file_name[0: -4] + "test.xml"
    out_file_path = os.path.join(in_dirname, out_file_name)

    tokenizer = JackTokenizer.JackTokenizer(os.path.join(in_dirname, in_file_name), out_file_path)
    execute(tokenizer)

def append_tokens(words, token_list):
    for word in words:
        tokens_and_words = re.split("(\W)", word)
        # print("token_words: " + " ".join(token_words))
        for token in tokens_and_words:
            if token != '':
                token_list.append(token)


def execute(tokenizer):
    tokens = []
    current_token = ""
    block_comment = False

    for line in tokenizer.in_file:
        if block_comment:
            if line.__contains__("*/"):
                block_comment = False
            continue
        if line.strip().startswith("/*"):
            block_comment = True
            if line.__contains__("*/"):
                block_comment = False
            continue

        if not line.startswith("//"):
            line = re.split("(/\*)|(//)", line)[0].strip()
            if line.__contains__("\""):
                words = line.split("\"")
                append_tokens(words[0].split(" "), tokens)
                tokens.append(words[1])
                append_tokens(words[2].split(" "), tokens)
            else:
                words = line.split(" ")
                append_tokens(words, tokens)

    tokenizer.out_file.write("<tokens>\n")

    print(tokenizer.out_file)

    for token in tokens:
        tokenizer.current_token = token

        if tokenizer.token_type() == "KEYWORD":
            tokenizer.out_file.write(f"<keyword> {tokenizer.current_token} </keyword>\n")
        if tokenizer.token_type() == "SYMBOL":
            tokenizer.out_file.write(f"<symbol> {tokenizer.symbol()} </symbol>\n")
        if tokenizer.token_type() == "IDENTIFIER":
            tokenizer.out_file.write(f"<identifier> {tokenizer.current_token} </identifier>\n")
        if tokenizer.token_type() == "INT_CONST":
            tokenizer.out_file.write(f"<integerConstant> {tokenizer.current_token} </integerConstant>\n")
        if tokenizer.token_type() == "STRING_CONST":
            tokenizer.out_file.write(f"<stringConstant> {tokenizer.current_token} </stringConstant>\n")

    tokenizer.out_file.write("</tokens>\n")

main()