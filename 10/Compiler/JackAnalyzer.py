import sys
import os
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
    out_file_name = in_file_name[0: -4] + "xml"
    out_file_path = os.path.join(in_dirname, out_file_name)

    tokenizer = JackTokenizer.JackTokenizer(in_file_name, out_file_path)
    # execute()

main()