PATTERN = """
#include <vector>
#include <string>

class Binary {

    struct Instruction {
        std::string opcode;
        std::string bin;

        Instruction(const char* opcode, const char* bin)
            : opcode(opcode)
            , bin(bin) {}
    };

public:
    std::vector<Instruction> program;
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

        instructions.append(indent + 'program.push_back(Instruction("%s", "%s"));' % (opcode, bin))

instructions.append(indent + 'blob = "%s";' % blob)

print PATTERN % {'INSTRUCTIONS': '\n'.join(instructions)}
