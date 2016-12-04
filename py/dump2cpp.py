PATTERN = """
class Binary {

public: // XXX
    std::vector<std::string> opcodes;
    std::string blob;

public:
    Binary() {
        setup();
    }

private:
    void setup();
};


void Binary::setup() {
%(INSTRUCTIONS)s
}
"""

blob = ""
indent = ' ' * 4
instructions = []

with open('dump.asm') as f:
    for line in f:
        pos    = line.index('\t')
        bin    = line[:pos].strip()
        opcode = line[pos:].strip()

        bin    = ''.join(['\\x%s' % b for b in bin.split()])
        blob  += bin

        #instructions.append(indent + 'opcodes.push_back("%s"); // %s' % (bin, opcode))
        instructions.append(indent + 'opcodes.push_back("%s");' % (opcode))

instructions.append(indent + 'blob = "%s";' % blob)

print PATTERN % {'INSTRUCTIONS': '\n'.join(instructions)}
