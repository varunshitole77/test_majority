#include <iostream>
#include <cassert>
#include "../src/mig_structure.h"
#include "../src/npn_classifier.h"
#include "../src/mig_synthesizer.h"
#include "../src/truth_table.h"

void testMAJ3Basic() {
    std::cout << "Testing MAJ-3 basic functionality..." << std::endl;
    
    // MAJ(x1, x2, x3) truth table
    std::string maj3_tt = "00010111";
    
    // Test evaluation
    assert(TruthTable::evaluateTruthTable(maj3_tt, {0, 0, 0}) == 0);
    assert(TruthTable::evaluateTruthTable(maj3_tt, {0, 1, 1}) == 1);
    assert(TruthTable::evaluateTruthTable(maj3_tt, {1, 0, 1}) == 1);
    assert(TruthTable::evaluateTruthTable(maj3_tt, {1, 1, 1}) == 1);
    
    std::cout << "  ✓ Truth table evaluation correct" << std::endl;
}

void testMAJ3MIGConstruction() {
    std::cout << "\nTesting MAJ-3 MIG construction..." << std::endl;
    
    // Create MIG for MAJ(x1, x2, x3)
    MIG mig(3);
    
    // Add single majority gate with inputs x1, x2, x3
    int gate_id = mig.addGate({1, 2, 3}, {0, 0, 0});
    mig.setOutput(gate_id, 0);
    
    // Verify it computes MAJ-3
    std::string expected_tt = "00010111";
    std::string actual_tt = mig.getTruthTable();
    
    assert(actual_tt == expected_tt);
    assert(mig.size() == 1);
    assert(mig.depth() == 1);
    
    std::cout << "  ✓ Manual MIG construction correct" << std::endl;
    std::cout << "  ✓ Size: " << mig.size() << ", Depth: " << mig.depth() << std::endl;
}

void testMAJ3Synthesis() {
    std::cout << "\nTesting MAJ-3 synthesis..." << std::endl;
    
    std::string maj3_tt = "00010111";
    
    // Synthesize
    auto mig = synthesizeOptimalMIG(maj3_tt);
    
    if (mig) {
        std::cout << "  ✓ Synthesis successful" << std::endl;
        std::cout << "  ✓ Size: " << mig->size() << ", Depth: " << mig->depth() << std::endl;
        
        // Validate
        assert(mig->validate(maj3_tt));
        assert(mig->size() == 1);
        
        // Print structure
        std::cout << "  Gate structure: " << mig->getGates()[0].toText() << std::endl;
    } else {
        std::cout << "  ✗ Synthesis failed" << std::endl;
        assert(false);
    }
}

void testANDORWithMAJ() {
    std::cout << "\nTesting AND/OR realization with MAJ..." << std::endl;
    
    // AND(x1, x2) = MAJ(0, x1, x2)
    MIG mig_and(2);
    int gate_id = mig_and.addGate({0, 1, 2}, {0, 0, 0});
    mig_and.setOutput(gate_id, 0);
    
    std::string and_tt = mig_and.getTruthTable();
    assert(and_tt == "0001");
    std::cout << "  ✓ AND(x1,x2) = MAJ(0,x1,x2) = " << and_tt << std::endl;
    
    // OR(x1, x2) = MAJ(1, x1, x2) = MAJ(¬0, x1, x2)
    MIG mig_or(2);
    gate_id = mig_or.addGate({0, 1, 2}, {1, 0, 0});
    mig_or.setOutput(gate_id, 0);
    
    std::string or_tt = mig_or.getTruthTable();
    assert(or_tt == "0111");
    std::cout << "  ✓ OR(x1,x2) = MAJ(1,x1,x2) = " << or_tt << std::endl;
}

int main() {
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "MAJ-3 COMPREHENSIVE TESTS" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    testMAJ3Basic();
    testMAJ3MIGConstruction();
    testMAJ3Synthesis();
    testANDORWithMAJ();
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ALL MAJ-3 TESTS PASSED ✓" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    return 0;
}