import sys
from os.path import basename

class Program:
    def __init__(self, binary_path, assembly):
        with open(binary_path, 'rb') as f:
            self.bytes = f.read()

        pos = self.bytes.index(assembly)
        self.before = self.bytes[:pos]
        self.after  = self.bytes[pos + len(assembly):]
        self.assembly = assembly


    def patch(self, bin_patch, name):
        assert len(bin_patch) == len(self.assembly)
        with open(name, 'wb') as f:
            f.write(self.before)
            f.write(bin_patch)
            f.write(self.after)


def load_file(path):
    with open(path, 'rb') as f:
        return f.read()

def load_bin():
    assembly = ''
    with open('dump.asm') as f:
        for line in f:
            pos    = line.index('\t')
            bin    = line[:pos].strip()
            bin    = ''.join([chr(int(b, 16)) for b in bin.split()])
            assembly += bin

    return assembly


def main():
    path     = sys.argv[1];

    assembly = load_bin()
    patch    = load_file(path)
    program  = Program('feedback/basic_benchmark', assembly)

    program.patch(patch, basename(path + ".exe"))

if __name__ == '__main__':
    main()


