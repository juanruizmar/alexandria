import sys

class text:
    def __init__(self, raw_content):
        self.content = raw_content.splitlines()
    
    def __iter__(self):
        return iter(self.content)

    def get_content(self):
        return '\n'.join(self.content)+'\n'
    
    def get_lines(self):
        return self.content
    
    def get_line(self, idx):
        return self.content[idx]
    
    def count_lines(self):
        return len(self.content)
    
    def add_line(self, line):
        self.content.append(line)
    
    def remove_line(self, idx):
        self.content = self.content[:idx] + self.content[idx+1:]

# Those functions read and write from a specific path, returning and receiving text objects
def read_file(read_path):
    try:
        with open(read_path, 'r') as file:
            content = file.read()
        return text(content)
    except FileNotFoundError:
        print(f"The file {read_path} does not exist.")
        sys.exit(1)
    except Exception as e:
        print(f"An error occurred while reading the file: {e}")
        sys.exit(-1)

def write_file(write_path, content):
    content = content.get_content()
    try:
        with open(write_path, 'w') as file:
            file.write(content)
    except Exception as e:
        print(f"An error occurred while writing the file: {e}")
        sys.exit(-1)
