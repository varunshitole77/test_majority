#include "mig_structure.h"

// ---------------- MIGGate ----------------

int MIGGate::evaluate(const std::vector<int>& input_values) const {
    assert(inputs.size() == polarities.size());
    int count = 0;
    for (size_t i = 0; i < inputs.size(); ++i) {
        int val = input_values[inputs[i] - 1]; // assuming inputs are 1-based
        if (polarities[i]) val = !val;
        count += val;
    }
    return (count >= 2) ? 1 : 0; // MAJ-3
}

std::string MIGGate::toText() const {
    std::stringstream ss;
    ss << "Gate " << id << ": MAJ(";
    for (size_t i = 0; i < inputs.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << (polarities[i] ? "¬" : "") << "x" << inputs[i];
    }
    ss << ")";
    return ss.str();
}

// ---------------- MIG ----------------

MIG::MIG(int vars) : n_vars(vars), output_gate(0), output_polarity(0) {}

int MIG::addGate(const std::vector<int>& inputs, const std::vector<int>& polarities) {
    int gate_id = static_cast<int>(gates.size()) + 1;
    gates.emplace_back(gate_id, inputs, polarities);
    return gate_id;
}

void MIG::setOutput(int gate_id, int polarity) {
    output_gate = gate_id;
    output_polarity = polarity;
}

int MIG::evaluate(const std::vector<int>& input_values) const {
    assert(!gates.empty());
    // simple MAJ-3 with single gate
    int val = gates.back().evaluate(input_values);
    return output_polarity ? !val : val;
}

std::string MIG::getTruthTable() const {
    int n = 1 << n_vars;
    std::string tt;
    for (int i = 0; i < n; ++i) {
        std::vector<int> inputs(n_vars);
        for (int j = 0; j < n_vars; ++j)
            inputs[j] = (i >> j) & 1;
        tt += std::to_string(evaluate(inputs));
    }
    return tt;
}

int MIG::depth() const { return 1; } // single MAJ-3

bool MIG::validate(const std::string& truth_table) const {
    return getTruthTable() == truth_table;
}

std::string MIG::toText() const {
    std::stringstream ss;
    for (const auto& g : gates)
        ss << g.toText() << "\n";
    return ss.str();
}
