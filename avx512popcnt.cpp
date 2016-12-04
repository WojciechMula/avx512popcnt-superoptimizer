#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <bitset>

#include "binary.cpp"


enum Values {
  EMPTY  = 2222,
  SET    = 1111,

  LOAD_0 = 100,
  LOAD_1,
  LOAD_2,
  LOAD_3,
  LOAD_4,
  LOAD_5,
  LOAD_6,
  LOAD_7,
  LOAD_8,
  LOAD_9,
  LOAD_10,
  LOAD_11,
  LOAD_12,
  LOAD_13,
  LOAD_14,
  LOAD_15,
  LOAD_16,
  LOAD_17,
  LOAD_18,
  LOAD_19,
  LOAD_20,
  LOAD_21,
  LOAD_22,
  LOAD_23,
  LOAD_24,
  LOAD_25,
  LOAD_26,
  LOAD_27,
  LOAD_28,
  LOAD_29,
  LOAD_30,
  LOAD_31,

  ONES_STEP_0 = 200,
  ONES_STEP_1,
  ONES_STEP_2,
  ONES_STEP_3,
  ONES_STEP_4,
  ONES_STEP_5,
  ONES_STEP_6,
  ONES_STEP_7,
  ONES_STEP_8,
  ONES_STEP_9,
  ONES_STEP_10,
  ONES_STEP_11,
  ONES_STEP_12,
  ONES_STEP_13,
  ONES_STEP_14,
  ONES_STEP_15,
  ONES_STEP_16,

  TWOS_STEP_0 = 300,
  TWOS_STEP_1,
  TWOS_STEP_2,
  TWOS_STEP_3,
  TWOS_STEP_4,
  TWOS_STEP_5,
  TWOS_STEP_6,
  TWOS_STEP_7,
  TWOS_STEP_8,

  FOURS_STEP_0 = 400,
  FOURS_STEP_1,
  FOURS_STEP_2,
  FOURS_STEP_3,
  FOURS_STEP_4,
  FOURS_STEP_5,

  EIGHTS_STEP_0 = 500,
  EIGHTS_STEP_1,
  EIGHTS_STEP_2,
  EIGHTS_STEP_3,
  EIGHTS_STEP_4,

  SIXTEENS_SAVED = 600,
  SIXTEENS_FINAL = 700,

  __LAST__
};


class Register {

    int value;
public:
    Register() : value(EMPTY) {}

    void set(int v) {
        value = v;
    }

    int get() const {
        return value;
    }

    bool is(int v) const {
        return (value == v);
    }
};


class SharedRegister: public Register {

    int counter;

public:
    SharedRegister()
        : Register()
        , counter(0) {}

    bool is_free() const {
        return counter == 0;
    }

    void lock(int cnt) {
        assert(is_free());
        counter = cnt;
    }

    void release() {
        assert(counter > 0);
        counter -= 1;
    }
};

enum Registers {
    // temporary memory
    TMP,

    // loop variables
    ONES,
    TWOS,
    FOURS,
    EIGHTS,
    SIXTEENS,
    THIRTYTWOS,

    // register names
    ZMM10,
    ZMM11,
    ZMM12,
    ZMM13,
    ZMM14,
    ZMM15,
    ZMM16,
    ZMM17,
    ZMM18,
    ZMM19,
    ZMM20,
    ZMM21,
    ZMM22,
    ZMM23,
    ZMM24,
    ZMM25,

    ZMM_X,

    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    TOTAL,

    // shared registers
    ZMM30 = 0,
    ZMM31 = 1
};




class State {

    static const size_t reg_count = TOTAL;
    static const size_t shr_count = 2;

public:
    State() = default;
    State(const State&) = default;

    Register        registers[reg_count];
    SharedRegister  shared[shr_count];

public:
    void init() {
        for (unsigned i=0; i < reg_count; i++) {
            registers[i].set(EMPTY);
        }

        for (unsigned i=0; i < shr_count; i++) {
            shared[i].set(EMPTY);
        }

        registers[ONES].set(ONES_STEP_0);
        registers[TWOS].set(TWOS_STEP_0);
        registers[FOURS].set(FOURS_STEP_0);
        registers[EIGHTS].set(EIGHTS_STEP_0);
    }
};


class Instruction {
    
    std::string opcode;
public:
    virtual bool fullfills(const State& state) const = 0;
    virtual void execute(State& state) = 0;

public:
    void set_opcode(const std::string& op) {
        opcode = op;
    }

    const std::string& get_opcode() const {
        return opcode;
    }
};


// vmovdqa64 offset(%[data]), %%zmm{30,31}
class LoadData: public Instruction {
    int target;
    int value;

public:
    LoadData(int t, int v) : target(t), value(v) {}
public:
    virtual bool fullfills(const State& state) const override {
        if (!state.shared[target].is_free()) {
            return false;
        }

        return true;
    }

    virtual void execute(State& state) override {

        state.shared[target].set(value);
        state.shared[target].lock(2);
    }
};

// vmovdqa64      %[ones], %%zmm10
class CopyRegister: public Instruction {
    int source;
    int source_value;
    int target;
    int result;

public:
    CopyRegister(int src, int sv, int trg, int res)
        : source(src)
        , source_value(sv)
        , target(trg)
        , result(res) {}

    virtual bool fullfills(const State& state) const override {
        if (!state.registers[source].is(source_value)) {
            return false;
        }

        return true;
    }

    virtual void execute(State& state) override {

        state.registers[target].set(result);
    }
};


class TernaryLogicFromShared: public Instruction {
    int src1;
    int src1_value;

    int src2;
    int src2_value;

    int target;
    int target_value;

    int result;

public:
    TernaryLogicFromShared(int src1, int src1_value, int src2, int src2_value, int target, int target_value, int result)
        : src1(src1)
        , src1_value(src1_value)
        , src2(src2)
        , src2_value(src2_value)
        , target(target)
        , target_value(target_value)
        , result(result) {}

    virtual bool fullfills(const State& state) const override {

        if (state.shared[src1].is_free()) {
            return false;
        }

        if (state.shared[src2].is_free()) {
            return false;
        }

        if (!state.shared[src1].is(src1_value)) {
            return false;
        }

        if (!state.shared[src2].is(src2_value)) {
            return false;
        }

        if (!state.registers[target].is(target_value)) {
            return false;
        }

        return true;
    }

    virtual void execute(State& state) override {

        state.registers[target].set(result);
        state.shared[src1].release();
        state.shared[src2].release();
    }
};


class TernaryLogic: public Instruction {
    int src1;
    int src1_value;

    int src2;
    int src2_value;

    int target;
    int target_value;

    int result;

public:
    TernaryLogic(int src1, int src1_value, int src2, int src2_value, int target, int target_value, int result)
        : src1(src1)
        , src1_value(src1_value)
        , src2(src2)
        , src2_value(src2_value)
        , target(target)
        , target_value(target_value)
        , result(result) {}

    virtual bool fullfills(const State& state) const override {

        if (!state.registers[src1].is(src1_value)) {
            return false;
        }

        if (!state.registers[src2].is(src2_value)) {
            return false;
        }

        if (!state.registers[target].is(target_value)) {
            return false;
        }

        return true;
    }

    virtual void execute(State& state) override {

        state.registers[target].set(result);
    }
};


class PopCount: public Instruction {

    int target;
    int result;
    
public:
    PopCount(int trg, int res = SET)
        : target(trg)
        , result(res) {}

    virtual bool fullfills(const State& state) const override {

        if (!state.registers[TMP].is(SET)) {
            return false;
        }

        return true;
    }

    virtual void execute(State& state) override {

        state.registers[target].set(result);
    }
};


class XorTotal: public Instruction {

public:
    virtual bool fullfills(const State& state) const override {

        if (!state.registers[TOTAL].is(EMPTY)) {
            return false;
        }

        return true;
    }

    virtual void execute(State& state) override {

        state.registers[TOTAL].set(SET);
    }
};


class AddTotal: public Instruction {

    int source;

public:
    AddTotal(int src) : source(src) {}

    virtual bool fullfills(const State& state) const override {

        if (!state.registers[TOTAL].is(SET)) {
            return false;
        }

        if (!state.registers[source].is(SET)) {
            return false;
        }

        return true;
    }

    virtual void execute(State&) override {
        // doesn't change state
    }
};

class StoreSixteens: public Instruction {

public:
    virtual bool fullfills(const State& state) const override {

        if (!state.registers[SIXTEENS].is(EMPTY)) {
            return false;
        }

        if (!state.registers[TMP].is(EMPTY)) {
            return false;
        }

        return true;
    }

    virtual void execute(State& state) override {
        
        state.registers[SIXTEENS].set(SIXTEENS_SAVED);
        state.registers[TMP].set(SET);
    }
    
};


using Program = std::vector<std::unique_ptr<Instruction>>;

class Build5thHarleySeal {

    Program prog;
    Binary  bin;

public:
    Program capture() {

        // popcnt from previous iteration
        add<StoreSixteens>();
        add<PopCount>(R0);
        add<PopCount>(R1);
        add<PopCount>(R2);
        add<PopCount>(R3);
        add<PopCount>(R4);
        add<PopCount>(R5);
        add<PopCount>(R6);
        add<PopCount>(R7);
        add<XorTotal>();
        add<AddTotal>(R0);
        add<AddTotal>(R1);
        add<AddTotal>(R2);
        add<AddTotal>(R3);
        add<AddTotal>(R4);
        add<AddTotal>(R5);
        add<AddTotal>(R6);
        add<AddTotal>(R7);
 
        // HS
        CSA_ones(ONES_STEP_0,  LOAD_0,  LOAD_1,  ONES_STEP_1,  ZMM10);
        CSA_ones(ONES_STEP_1,  LOAD_2,  LOAD_3,  ONES_STEP_2,  ZMM11);
        CSA_ones(ONES_STEP_2,  LOAD_4,  LOAD_5,  ONES_STEP_3,  ZMM12);
        CSA_ones(ONES_STEP_3,  LOAD_6,  LOAD_7,  ONES_STEP_4,  ZMM13);
        CSA_ones(ONES_STEP_4,  LOAD_8,  LOAD_9,  ONES_STEP_5,  ZMM14);
        CSA_ones(ONES_STEP_5,  LOAD_10, LOAD_11, ONES_STEP_6,  ZMM15);
        CSA_ones(ONES_STEP_6,  LOAD_12, LOAD_13, ONES_STEP_7,  ZMM16);
        CSA_ones(ONES_STEP_7,  LOAD_14, LOAD_15, ONES_STEP_8,  ZMM17);
        CSA_ones(ONES_STEP_8,  LOAD_16, LOAD_17, ONES_STEP_9,  ZMM18);
        CSA_ones(ONES_STEP_9,  LOAD_18, LOAD_19, ONES_STEP_10, ZMM19);
        CSA_ones(ONES_STEP_10, LOAD_20, LOAD_21, ONES_STEP_11, ZMM20);
        CSA_ones(ONES_STEP_11, LOAD_22, LOAD_23, ONES_STEP_12, ZMM21);
        CSA_ones(ONES_STEP_12, LOAD_24, LOAD_25, ONES_STEP_13, ZMM22);
        CSA_ones(ONES_STEP_13, LOAD_26, LOAD_27, ONES_STEP_14, ZMM23);
        CSA_ones(ONES_STEP_14, LOAD_28, LOAD_29, ONES_STEP_15, ZMM24);
        CSA_ones(ONES_STEP_15, LOAD_30, LOAD_31, ONES_STEP_16, ZMM25);

        CSA_twos(TWOS_STEP_0, ZMM10, ONES_STEP_1,  ZMM11, ONES_STEP_2,  TWOS_STEP_1);
        CSA_twos(TWOS_STEP_1, ZMM12, ONES_STEP_3,  ZMM13, ONES_STEP_4,  TWOS_STEP_2);
        CSA_twos(TWOS_STEP_2, ZMM14, ONES_STEP_5,  ZMM15, ONES_STEP_6,  TWOS_STEP_3);
        CSA_twos(TWOS_STEP_3, ZMM16, ONES_STEP_7,  ZMM17, ONES_STEP_8,  TWOS_STEP_4);
        CSA_twos(TWOS_STEP_4, ZMM18, ONES_STEP_9,  ZMM19, ONES_STEP_10, TWOS_STEP_5);
        CSA_twos(TWOS_STEP_5, ZMM20, ONES_STEP_11, ZMM21, ONES_STEP_12, TWOS_STEP_6);
        CSA_twos(TWOS_STEP_6, ZMM22, ONES_STEP_13, ZMM23, ONES_STEP_14, TWOS_STEP_7);
        CSA_twos(TWOS_STEP_7, ZMM24, ONES_STEP_15, ZMM25, ONES_STEP_16, TWOS_STEP_8);

        CSA_fours(FOURS_STEP_0, ZMM10, TWOS_STEP_1, ZMM12, TWOS_STEP_2, FOURS_STEP_1);
        CSA_fours(FOURS_STEP_1, ZMM14, TWOS_STEP_3, ZMM16, TWOS_STEP_4, FOURS_STEP_2);
        CSA_fours(FOURS_STEP_2, ZMM18, TWOS_STEP_5, ZMM20, TWOS_STEP_6, FOURS_STEP_3);
        CSA_fours(FOURS_STEP_3, ZMM22, TWOS_STEP_7, ZMM24, TWOS_STEP_8, FOURS_STEP_4);

        CSA_eights(EIGHTS_STEP_0, ZMM10, FOURS_STEP_1, ZMM14, FOURS_STEP_2, EIGHTS_STEP_1);
        CSA_eights(EIGHTS_STEP_1, ZMM18, FOURS_STEP_3, ZMM22, FOURS_STEP_4, EIGHTS_STEP_2);

        add<CopyRegister>(SIXTEENS, SIXTEENS_SAVED, THIRTYTWOS, SIXTEENS_SAVED);
        add<TernaryLogic>(ZMM10, EIGHTS_STEP_1, ZMM18, EIGHTS_STEP_2, SIXTEENS,   SIXTEENS_SAVED, SIXTEENS_FINAL);
        add<TernaryLogic>(ZMM10, EIGHTS_STEP_1, ZMM18, EIGHTS_STEP_2, THIRTYTWOS, SIXTEENS_SAVED, SIXTEENS_FINAL);

        return std::move(prog);
    }

private:

    template <typename T, typename... TA>
    void add(TA&&... args) {
        const auto& opcode = bin.opcodes[prog.size()];
        prog.push_back(std::make_unique<T>(args...));
        prog.back()->set_opcode(opcode);
    }

private:
    // vmovdqa64      0x0000(%[data]), %%zmm30
    // vmovdqa64      0x0040(%[data]), %%zmm31
    // vmovdqa64      %[ones], TARGET
    // vpternlogd     $0x96, %%zmm30, %%zmm31, %[ones]
    // vpternlogd     $0xe8, %%zmm30, %%zmm31, TARGET
    void CSA_ones(int source_value, int loaded_value0, int loaded_value1, int target_value, int target) {

        add<LoadData>(ZMM30, loaded_value0);
        add<LoadData>(ZMM31, loaded_value1);
        add<CopyRegister>(ONES, source_value, target, source_value);
        add<TernaryLogicFromShared>(ZMM30, loaded_value0, ZMM31, loaded_value1, ONES, source_value, target_value);
        add<TernaryLogicFromShared>(ZMM30, loaded_value0, ZMM31, loaded_value1, target, source_value, target_value);
    }

    // vmovdqa64      TWOS, ZMM_X
    // vpternlogd     $0x96, src1,  src2, TWOS
    // vpternlogd     $0xe8, ZMM_X, src2, src1
    void CSA_twos(int twos_value, int src1, int src1_val, int src2, int src2_val, int target_value) {
        
        add<CopyRegister>(TWOS, twos_value, ZMM_X, twos_value);
        add<TernaryLogic>(src1,  src1_val,   src2, src2_val, TWOS, twos_value, target_value);
        add<TernaryLogic>(ZMM_X, twos_value, src2, src2_val, src1, src1_val,   target_value);
    }

    // vmovdqa64      FOURS, ZMM_X
    // vpternlogd     $0x96, src1,  src2, FOURS
    // vpternlogd     $0xe8, ZMM_X, src2, src1
    void CSA_fours(int fours_value, int src1, int src1_val, int src2, int src2_val, int target_value) {
        
        add<CopyRegister>(FOURS, fours_value, ZMM_X, fours_value);
        add<TernaryLogic>(src1,  src1_val,    src2, src2_val, FOURS, fours_value, target_value);
        add<TernaryLogic>(ZMM_X, fours_value, src2, src2_val, src1,  src1_val,    target_value);
    }

    // vmovdqa64      EIGHTS, ZMM_X
    // vpternlogd     $0x96, src1,  src2, EIGHTS
    // vpternlogd     $0xe8, ZMM_X, src2, src1
    void CSA_eights(int eights_value, int src1, int src1_val, int src2, int src2_val, int target_value) {
        
        add<CopyRegister>(EIGHTS, eights_value, ZMM_X, eights_value);
        add<TernaryLogic>(src1,  src1_val,     src2, src2_val, EIGHTS, eights_value, target_value);
        add<TernaryLogic>(ZMM_X, eights_value, src2, src2_val, src1,   src1_val,     target_value);
    }

};


Program make_program() {

    Build5thHarleySeal builder;

    return builder.capture();
}


void dump(const Program& prog) {

    for (size_t i=0; i < prog.size(); i++) {
        printf("%5lu: ", i);
        printf("%s\n", prog[i]->get_opcode().data());
    }
}


class Simulator {

    const Program& program;
    std::vector<State> state;

public:
    Simulator(const Program& prog)
        : program(prog) {

        State st;
        st.init();
        state.push_back(std::move(st));
    }

public:
    bool execute(const std::vector<size_t>& lookup) {

        const auto n  = program.size();

        for (size_t i = 0; i < n; i++) {

            const size_t ip = lookup[i];

            if (program[ip]->fullfills(state.back())) {
                state.push_back(state.back());
                program[ip]->execute(state.back());
            } else {
                //printf("break at instruction %lu\n", ip);
                //printf("opcode: %s\n", program[ip]->get_opcode().data());
                return false;
            }
        }

        //puts("OK");
        return true;
    }
};


class Generator {

    const Program& program;
    std::vector<size_t> lookup;

    struct Item {
        State cpu;
        size_t emitted_instruction;
        std::bitset<143> available;
        size_t index;
    };

    std::vector<Item> state;

public:
    Generator(const Program& prog)
        : program(prog) {

        state.resize(prog.size() + 1);

        for (size_t i=0; i < prog.size(); i++) {
            lookup.push_back(i);
        }
        
        state[0].cpu.init();
        state[0].available.set();
        state[0].index = 0;
    }

public:
    void execute(std::mt19937& gen) {

        std::shuffle(lookup.begin(), lookup.end(), gen);

        size_t index = 0;
        size_t last_printed = 0;

        while (true) {

            if (true || index > last_printed) {
                printf("index = %lu\n", index);
                last_printed = index;
            }

            if (try_advance(index)) {
                index += 1;
                if (index == program.size()) {
                    puts("THERE IS!!!");           
                    return;
                }
            } else {
                index -= 1;
            }
        }
    }

private:
    bool try_advance(size_t index) {
    
        Item& current = state[index];
    
        while (current.index < current.available.size()) {

            const auto ip = lookup[current.index];
            if (current.available.test(ip) && program.at(ip)->fullfills(current.cpu)) {

                Item& next = state.at(index + 1);

                next = current;

                next.index = 0;
                next.emitted_instruction = ip;
                next.available.reset(ip);

                program[ip]->execute(next.cpu);

                current.index++;
                return true;
            }

            current.index++;
        }

        return false;
    }
};



int main() {

    Program prog = make_program();
    
    Generator gen(prog);
    std::mt19937 g;
    g.seed(2);
    gen.execute(g);

#if 0
    std::vector<size_t> indices;
    for (size_t i=0; i < prog.size(); i++) {
        indices.push_back(i);
    }

    for (size_t j=0; j < 1000000; j++) {
        std::mt19937 g;
        g.seed(j);
        std::vector<size_t> v(indices);
        std::shuffle(v.begin(), v.end(), g);

        Simulator sim(prog);
        if (sim.execute(v)) {
            printf("j=%d\n", j);
        }
    }
#endif
}

