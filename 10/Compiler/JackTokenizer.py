class JackTokenizer:

    def __init__(self, in_file_path, out_file_path):
        self.in_file = open(in_file_path, "rt")
        self.out_file = open(out_file_path, "at", buffering = 1024)

    def hasMoreTokens(self):
        return self.in_file.read()
