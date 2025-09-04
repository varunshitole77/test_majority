#ifndef MIG_STRUCTURE_H
#define MIG_STRUCTURE_H

#include <vector>
#include <string>
#include <sstream>
#include <cassert>

class MIGGate {
public:
    int id;
    std::vector<int> inputs;
    std::vector<int> polarities;

    MIGGate(int gate_id, const std::vector<int>& in, const std::vector<int>& pol = {})
        : id(gate_id), inputs(in), polarities(pol) {}

    int evaluate(const std::vector<int>& input_values) const;
    std::string toText() const;
};

class MIG {
public:
    int n_vars;
    std::vector<MIGGate> gates;
    int output_gate;
    int output_polarity;

    MIG(int vars = 3);

    int addGate(const std::vector<int>& inputs, const std::vector<int>& polarities = {});
    void setOutput(int gate_id, int polarity);

    int evaluate(const std::vector<int>& input_values) const;
    std::string getTruthTable() const;
    int size() const { return static_cast<int>(gates.size()); }
    int depth() const;
    bool validate(const std::string& truth_table) const;
    std::string toText() const;

    const std::vector<MIGGate>& getGates() const { return gates; }
};

#endif // MIG_STRUCTURE_H
