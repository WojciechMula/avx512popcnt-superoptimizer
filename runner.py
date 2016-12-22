import os
import sys
import stat
import time

from hashlib import md5
from os.path import join, isfile

class Application:
    
    def __init__(self, program):
        self.program        = program

        def mkdir(path):
            try:
                os.mkdir(path)
            except:
                pass

        self.input_dir      = '0_generated'
        self.output_valid   = '1_valid'
        self.output_invalid = '2_invalid'

        for dir in [self.input_dir, self.output_valid, self.output_invalid]:
            mkdir(dir)

        self.status = {}
        for name in os.listdir(self.output_valid):
            if name.endswith('.bin'):
                md5sum = name[:-4]
                self.status[md5sum] = True

        for name in os.listdir(self.output_invalid):
            if name.endswith('.bin'):
                md5sum = name[:-4]
                self.status[md5sum] = False


    def run(self):
        processed = True
        printed = True
        exe = self.program
        while True:
            if processed:
                printed = False
            else:
                if not printed:
                    print "awaiting for files in %s" % self.input_dir
                    printed = True
                time.sleep(1.0)

            processed = False
            files = os.listdir(self.input_dir)
            for name in files:
                if not name.endswith('.bin'):
                    continue

                path = join(self.input_dir, name)
                if not isfile(path):
                    continue
                
                processed = True
                sys.stdout.write("processing '%s'" % path)
                sys.stdout.flush()
                oldpath = path
                path = path + '.1'
                os.rename(oldpath, path)

                bin    = load_file(path)
                md5sum = md5(bin).hexdigest()
                if md5sum in self.status:
                    print "alredy checked"
                    os.remove(path)
                    continue

                bin_path = path
                exe_path = md5sum
                res_path = md5sum + ".result" # it should be passed to a program instance
                dst_name = md5sum
                
                if exe.run(bin, md5sum):
                    self.status[md5sum] = True
                    print "OK"
                    os.rename(bin_path, join(self.output_valid, dst_name + ".bin"))
                    os.rename(res_path, join(self.output_valid, dst_name + ".result"))
                else:
                    self.status[md5sum] = False
                    print "failed"
                    os.rename(bin_path, join(self.output_invalid, dst_name + ".bin"))
                    os.rename(res_path, join(self.output_invalid, dst_name + ".result"))

                # in either case remove executable, as it can be simply rebuilt
                os.remove(exe_path)


class Program:
    def __init__(self, binary_path, assembly):
        with open(binary_path, 'rb') as f:
            self.bytes = f.read()

        pos = self.bytes.index(assembly)
        self.before = self.bytes[:pos]
        self.after  = self.bytes[pos + len(assembly):]


    def run(self, bin_patch, name):
        with open(name, 'wb') as f:
            f.write(self.before)
            f.write(bin_patch)
            f.write(self.after)

        os.chmod(name, stat.S_IXUSR | stat.S_IWUSR | stat.S_IRUSR)
        ret = os.system("sde -cnl -- ./%s > %s.result" % (name, name))
        return ret == 0


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
    assembly = load_bin()
    program  = Program('feedback/basic_benchmark', assembly)

    app = Application(program)
    app.run()

if __name__ == '__main__':
    main()

