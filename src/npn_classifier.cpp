#include "npn_classifier.h"
#include "truth_table.h"
#include <algorithm>
#include <numeric>

std::string NPNClassifier::applyNegationPattern(const std::string& tt, 
                                               const std::vector<int>& neg_inputs, 
                                               int neg_output) {
    int n_vars = TruthTable::getNumVars(tt);
    int num_rows = 1 << n_vars;
    std::string new_tt(num_rows, '0');
    
    for (int i = 0; i < num_rows; ++i) {
        std::vector<int> inputs(n_vars);
        int temp = i;
        for (int j = n_vars - 1; j >= 0; --j) {
            inputs[j] = temp & 1;
            temp >>= 1;
        }
        for (int j = 0; j < n_vars; ++j) {
            if (neg_inputs[j]) inputs[j] = 1 - inputs[j];
        }
        int new_index = 0;
        for (int j = 0; j < n_vars; ++j) {
            new_index += inputs[j] * (1 << (n_vars - 1 - j));
        }
        int output_val = tt[new_index] - '0';
        if (neg_output) output_val = 1 - output_val;
        new_tt[i] = '0' + output_val;
    }
    return new_tt;
}

std::string NPNClassifier::applyPermutation(const std::string& tt, 
                                           const std::vector<int>& perm) {
    int n_vars = TruthTable::getNumVars(tt);
    int num_rows = 1 << n_vars;
    std::string new_tt(num_rows, '0');
    
    for (int i = 0; i < num_rows; ++i) {
        std::vector<int> inputs(n_vars);
        int temp = i;
        for (int j = n_vars - 1; j >= 0; --j) {
            inputs[j] = temp & 1;
            temp >>= 1;
        }
        std::vector<int> permuted_inputs(n_vars);
        for (int j = 0; j < n_vars; ++j) {
            permuted_inputs[j] = inputs[perm[j]];
        }
        int new_index = 0;
        for (int j = 0; j < n_vars; ++j) {
            new_index += permuted_inputs[j] * (1 << (n_vars - 1 - j));
        }
        new_tt[i] = tt[new_index];
    }
    return new_tt;
}

std::string NPNClassifier::getNPNRepresentative(const std::string& tt) {
    int n_vars = TruthTable::getNumVars(tt);
    std::string min_tt = tt;
    uint64_t min_value = TruthTable::truthTableToBinary(tt);

    std::vector<int> perm(n_vars);
    std::iota(perm.begin(), perm.end(), 0);

    do {
        for (int neg_mask = 0; neg_mask < (1 << n_vars); ++neg_mask) {
            std::vector<int> neg_inputs(n_vars);
            for (int i = 0; i < n_vars; ++i) {
                neg_inputs[i] = (neg_mask >> i) & 1;
            }
            for (int neg_output = 0; neg_output <= 1; ++neg_output) {
                std::string transformed = applyPermutation(tt, perm);
                transformed = applyNegationPattern(transformed, neg_inputs, neg_output);
                uint64_t value = TruthTable::truthTableToBinary(transformed);
                if (value < min_value) {
                    min_value = value;
                    min_tt = transformed;
                }
            }
        }
    } while (std::next_permutation(perm.begin(), perm.end()));
    
    return min_tt;
}

bool NPNClassifier::areNPNEquivalent(const std::string& tt1, const std::string& tt2) {
    return getNPNRepresentative(tt1) == getNPNRepresentative(tt2);
}

std::vector<std::string> NPNClassifier::findNPNClasses(const std::vector<std::string>& truth_tables) {
    std::set<std::string> reps;
    for (const auto& tt : truth_tables) {
        reps.insert(getNPNRepresentative(tt));
    }
    return std::vector<std::string>(reps.begin(), reps.end());
}

bool NPNClassifier::checkSymmetry(const std::string& tt, int var1, int var2) {
    int n_vars = TruthTable::getNumVars(tt);
    std::vector<int> perm(n_vars);
    std::iota(perm.begin(), perm.end(), 0);
    std::swap(perm[var1], perm[var2]);
    std::string swapped = applyPermutation(tt, perm);
    return swapped == tt;
}

std::vector<std::pair<int, int>> NPNClassifier::getSymmetricVariables(const std::string& tt) {
    int n_vars = TruthTable::getNumVars(tt);
    std::vector<std::pair<int,int>> sym_pairs;
    for (int i = 0; i < n_vars; ++i) {
        for (int j = i+1; j < n_vars; ++j) {
            if (checkSymmetry(tt, i, j)) {
                sym_pairs.push_back({i,j});
            }
        }
    }
    return sym_pairs;
}
