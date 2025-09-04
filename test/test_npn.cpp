#include <iostream>
#include <cassert>
#include "../src/npn_classifier.h"
#include "../src/truth_table.h"

void testBasicNPN() {
    std::cout << "Testing basic NPN operations..." << std::endl;
    
    // Test constant functions
    assert(NPNClassifier::getNPNRepresentative("0000") == "0000");
    assert(NPNClassifier::getNPNRepresentative("1111") == "0000");  // Complement of 0
    
    // Test single variable
    assert(NPNClassifier::getNPNRepresentative("0011") == "0011");  // x1
    assert(NPNClassifier::getNPNRepresentative("1100") == "0011");  // ¬x1
    
    std::cout << "  ✓ Basic NPN tests passed" << std::endl;
}

void testPermutation() {
    std::cout << "\nTesting permutation..." << std::endl;
    
    // AND(x1, x2) = "0001"
    std::string and_tt = "0001";
    
    // AND(x2, x1) should be same due to commutativity
    std::vector<int> perm = {1, 0};  // Swap x1 and x2
    std::string permuted = NPNClassifier::applyPermutation(and_tt, perm);
    assert(permuted == "0001");
    
    // XOR(x1, x2) = "0110"
    std::string xor_tt = "0110";
    std::string permuted_xor = NPNClassifier::applyPermutation(xor_tt, perm);
    assert(permuted_xor == "0110");
    
    std::cout << "  ✓ Permutation tests passed" << std::endl;
}

void testNegation() {
    std::cout << "\nTesting negation..." << std::endl;
    
    // AND(x1, x2) = "0001"
    std::string and_tt = "0001";
    
    // AND(¬x1, x2) = "0100"
    std::string neg_x1 = NPNClassifier::applyNegationPattern(and_tt, {1, 0}, 0);
    assert(neg_x1 == "0100");
    
    // ¬AND(x1, x2) = "1110"
    std::string neg_out = NPNClassifier::applyNegationPattern(and_tt, {0, 0}, 1);
    assert(neg_out == "1110");
    
    std::cout << "  ✓ Negation tests passed" << std::endl;
}

void testNPNClassesN2() {
    std::cout << "\nTesting NPN classes for n=2..." << std::endl;
    
    auto all_tt = TruthTable::generateAllTruthTables(2);
    auto classes = NPNClassifier::findNPNClasses(all_tt);
    
    assert(classes.size() == 4);
    
    // Expected representatives (sorted):
    // 0000 - constant 0
    // 0001 - AND
    // 0011 - x1
    // 0110 - XOR
    
    std::vector<std::string> expected = {"0000", "0001", "0011", "0110"};
    assert(classes == expected);
    
    std::cout << "  ✓ Found correct " << classes.size() << " NPN classes" << std::endl;
    std::cout << "  Representatives: ";
    for (const auto& c : classes) std::cout << c << " ";
    std::cout << std::endl;
}

void testNPNClassesN3() {
    std::cout << "\nTesting NPN classes for n=3..." << std::endl;
    
    auto all_tt = TruthTable::generateAllTruthTables(3);
    auto classes = NPNClassifier::findNPNClasses(all_tt);
    
    assert(classes.size() == 14);
    
    // Check that MAJ-3 is among them
    std::string maj3_rep = NPNClassifier::getNPNRepresentative("00010111");
    bool found = false;
    for (const auto& c : classes) {
        if (c == maj3_rep) {
            found = true;
            break;
        }
    }
    assert(found);
    
    std::cout << "  ✓ Found correct " << classes.size() << " NPN classes" << std::endl;
}

void testSymmetry() {
    std::cout << "\nTesting symmetry detection..." << std::endl;
    
    // AND is symmetric in both variables
    std::string and_tt = "0001";
    auto sym_pairs = NPNClassifier::getSymmetricVariables(and_tt);
    assert(sym_pairs.size() == 1);
    assert(sym_pairs[0] == std::make_pair(0, 1));
    
    // MAJ-3 is symmetric in all three variables
    std::string maj3_tt = "00010111";
    sym_pairs = NPNClassifier::getSymmetricVariables(maj3_tt);
    assert(sym_pairs.size() == 3);
    
    std::cout << "  ✓ Symmetry detection passed" << std::endl;
}

void testEquivalence() {
    std::cout << "\nTesting NPN equivalence..." << std::endl;
    
    // AND and OR should not be equivalent
    assert(!NPNClassifier::areNPNEquivalent("0001", "0111"));
    
    // Function should be equivalent to itself
    assert(NPNClassifier::areNPNEquivalent("0001", "0001"));
    
    std::cout << "  ✓ Equivalence checking passed" << std::endl;
}

int main() {
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "NPN CLASSIFICATION TESTS" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    testBasicNPN();
    testPermutation();
    testNegation();
    testNPNClassesN2();
    testNPNClassesN3();
    testSymmetry();
    testEquivalence();
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "ALL NPN TESTS PASSED ✓" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    return 0;
}