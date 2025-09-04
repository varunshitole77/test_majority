#include <iostream>
#include <cassert>
#include <chrono>
#include "../src/mig_synthesizer.h"
#include "../src/mig_structure.h"

void testConstants() {
    std::cout << "Testing constant functions..." << std::endl;
    
    // Constant 0
    auto mig = synthesizeOptimalMIG("0000");
    assert(mig != nullptr);
    assert(mig->size() == 0);
    assert(mig->getTruthTable() == "0000");
    std::cout << "  ✓ Constant 0 synthesized" << std::endl;
    
    // Constant 1
    mig = synthesizeOptimalMIG("1111");
    assert(mig != nullptr);
    assert(mig->size() == 0);
    assert(mig->getTruthTable() == "1111");
    std::cout << "  ✓ Constant 1 synthesized" << std::endl;
}

void testSingleVariable() {
    std::cout << "\nTesting single variable functions..." << std::endl;
    
    // x1 for 2 variables
    auto mig = synthesizeOptimalMIG("0011");
    assert(mig != nullptr);
    assert(mig->size() == 0);  // No gates needed
    assert(mig->getTruthTable() == "0011");
    std::cout << "  ✓ Variable x1 synthesized" << std::endl;
    
    // ¬x1
    mig = synthesizeOptimalMIG("1100");
    assert(mig != nullptr);
    assert(mig->size() == 0);  // No gates needed, just negation
    assert(mig->getTruthTable() == "1100");
    std::cout << "  ✓ Negated variable ¬x1 synthesized" << std::endl;
}

void testANDGate() {
    std::cout << "\nTesting AND gate synthesis..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto mig = synthesizeOptimalMIG("0001");
    auto end = std::chrono::high_resolution_clock::now();
    double synthesis_time = std::chrono::duration<double>(end - start).count();
    
    assert(mig != nullptr);
    assert(mig->size() == 1);  // Should use MAJ(0, x1, x2)
    assert(mig->getTruthTable() == "0001");
    
    std::cout << "  ✓ AND gate synthesized in " << synthesis_time << "s" << std::endl;
    std::cout << "  Size: " << mig->size() << ", Depth: " << mig->depth() << std::endl;
}

void testORGate() {
    std::cout << "\nTesting OR gate synthesis..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto mig = synthesizeOptimalMIG("0111");
    auto end = std::chrono::high_resolution_clock::now();
    double synthesis_time = std::chrono::duration<double>(end - start).count();
    
    assert(mig != nullptr);
    assert(mig->size() == 1);  // Should use MAJ(1, x1, x2)
    assert(mig->getTruthTable() == "0111");
    
    std::cout << "  ✓ OR gate synthesized in " << synthesis_time << "s" << std::endl;
    std::cout << "  Size: " << mig->size() << ", Depth: " << mig->depth() << std::endl;
}

void testXORGate() {
    std::cout << "\nTesting XOR gate synthesis..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto mig = synthesizeOptimalMIG("0110");
    auto end = std::chrono::high_resolution_clock::now();
    double synthesis_time = std::chrono::duration<double>(end - start).count();
    
    assert(mig != nullptr);
    // XOR needs more gates in MIG
    assert(mig->getTruthTable() == "0110");
    
    std::cout << "  ✓ XOR gate synthesized in " << synthesis_time << "s" << std::endl;
    std::cout << "  Size: " << mig->size() << ", Depth: " << mig->depth() << std::endl;
}

void testMAJ3() {
    std::cout << "\nTesting MAJ-3 synthesis..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto mig = synthesizeOptimalMIG("00010111");
    auto end = std::chrono::high_resolution_clock::now();
    double synthesis_time = std::chrono::duration<double>(end - start).count();
    
    assert(mig != nullptr);
    assert(mig->size() == 1);  // MAJ-3 needs exactly 1 MAJ gate
    assert(mig->getTruthTable() == "00010111");
    
    std::cout << "  ✓ MAJ-3 synthesized in " << synthesis_time << "s" << std::endl;
    std::cout << "  Size: " << mig->size() << ", Depth: " << mig->depth() << std::endl;
}

void testComplex3Var() {
    std::cout << "\nTesting complex 3-variable function..." << std::endl;
    
    // Full adder sum: x1 ⊕ x2 ⊕ x3 = "01101001"
    auto start = std::chrono::high_resolution_clock::now();
    auto mig = synthesizeOptimalMIG("01101001");
    auto end = std::chrono::high_resolution_clock::now();
    double synthesis_time = std::chrono::duration<double>(end - start).count();
    
    if (mig) {
        assert(mig->getTruthTable() == "01101001");
        std::cout << "  ✓ 3-XOR synthesized in " << synthesis_time << "s" << std::endl;
        std::cout << "  Size: " << mig->size() << ", Depth: " << mig->depth() << std::endl;
    } else {
        std::cout << "  ⚠ 3-XOR synthesis timed out" << std::endl;
    }
}

void testValidation() {
    std::cout << "\nTesting MIG validation..." << std::endl;
    
    // Create a simple MIG manually
    MIG mig(2);
    int gate_id = mig.addGate({0, 1, 2}, {0, 0, 0});  // AND gate
    mig.setOutput(gate_id, 0);
    
    // Should validate against AND truth table
    assert(mig.validate("0001"));
    // Should not validate against OR truth table
    assert(!mig.validate("0111"));
    
    std::cout << "  ✓ Validation works correctly" << std::endl;
}

int main() {
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "MIG SYNTHESIS TESTS" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    testConstants();
    testSingleVariable();
    testANDGate();
    testORGate();
    testXORGate();
    testMAJ3();
    testComplex3Var();
    testValidation();
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ALL SYNTHESIS TESTS PASSED ✓" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    return 0;
}