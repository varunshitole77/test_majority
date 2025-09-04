#ifndef MIG_SYNTHESIZER_H
#define MIG_SYNTHESIZER_H

#include "mig_structure.h"
#include <memory>
#include <string>

class MIGSynthesizer {
public:
    MIGSynthesizer(int timeout_ms = 60000) {}
    std::unique_ptr<MIG> synthesizeSizeOptimal(const std::string& truth_table);
};

// Wrapper
std::unique_ptr<MIG> synthesizeOptimalMIG(const std::string& truth_table);

#endif // MIG_SYNTHESIZER_H
