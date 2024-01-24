import Parser
import CodeWriter
import sys
import os

# python3 main.py ../../ProgramFlow/BasicLoop/BasicLoop.vm

def execute(parser, code_writer):

    # code_writer.write_init()

    for current_line in parser.file:
        current_line = current_line.strip()

        # if the line is not empty or a comment
        if current_line and not current_line.startswith("/"):
            print("line: " + current_line)
            parser.current_line = current_line
            command_type = parser.command_type()


            if command_type == "C_ARITHMETIC":
                code_writer.write_arithmetic(parser.arg1())
            elif command_type == "C_PUSH" or command_type =="C_POP":
                code_writer.write_push_pop(command_type, parser.arg1(), parser.arg2())
            elif command_type == "C_LABEL":
                code_writer.write_label(parser.arg1())
            elif command_type == "C_GOTO":
                code_writer.write_goto(parser.arg1())
            elif command_type == "C_IF":
                code_writer.write_if(parser.arg1())
            elif command_type == "C_CALL":
                code_writer.write_call(parser.arg1(), parser.arg2())
            elif command_type == "C_RETURN":
                code_writer.write_return()
            elif command_type == "C_FUNCTION":
                code_writer.write_function(parser.arg1(), parser.arg2())
            else:
                print("command_type error: " + current_line + ", command_type = " + command_type)

    parser.file.close()
    code_writer.file.close()


def main():
    in_file_path = sys.argv[1:][0]

    if os.path.isdir(in_file_path):
        files = os.listdir(in_file_path)

        in_file_name_array = in_file_path.split("/")
        file_name = in_file_name_array[-2]
        out_file_name = file_name + ".asm"
        out_file_path = "../outFiles/" + out_file_name

        for file_name in files:
            if file_name.endswith("vm"):
                parser = Parser.Parser(in_file_path + file_name)
                code_writer = CodeWriter.CodeWriter(out_file_path, file_name)
                execute(parser, code_writer)

    else:
        in_file_name_array = in_file_path.split("/")
        file_name = in_file_name_array[-1]
        out_file_name = file_name[0:-2] + "asm"
        out_file_path = "../out_files/" + out_file_name

        parser = Parser.Parser(in_file_path)
        code_writer = CodeWriter.CodeWriter(out_file_path, file_name)
        execute(parser, code_writer)


main()