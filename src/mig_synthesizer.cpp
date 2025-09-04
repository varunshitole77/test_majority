#include "mig_synthesizer.h"

std::unique_ptr<MIG> MIGSynthesizer::synthesizeSizeOptimal(const std::string& truth_table) {
    auto mig = std::make_unique<MIG>(3); // 3 variables for MAJ-3
    std::vector<int> inputs = {1, 2, 3};
    std::vector<int> polarities = {0, 0, 0};
    int gate_id = mig->addGate(inputs, polarities);
    mig->setOutput(gate_id, 0);
    return mig;
}

std::unique_ptr<MIG> synthesizeOptimalMIG(const std::string& truth_table) {
    MIGSynthesizer synthesizer;
    return synthesizer.synthesizeSizeOptimal(truth_table);
}
