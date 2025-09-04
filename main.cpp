#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include "truth_table.h"
#include "npn_classifier.h"
#include "mig_synthesizer.h"
#include "mig_structure.h"
#include "utils.h"

// Print usage information
void printUsage() {
    std::cout << "Options:\n";
    std::cout << "  --function <truth_table> <nvars>  Query a specific function\n";
    std::cout << "  --compute <n>                     Compute all NPN classes\n";
    std::cout << "  --test                            Run tests\n";
    std::cout << "  --help                            Show this help message\n";
}

// Query or synthesize MIG for a specific truth table
void queryMIG(const std::string& truth_table, int n_vars) {
    std::string representative = NPNClassifier::getNPNRepresentative(truth_table);

    std::string filename = "data/npn_" + std::to_string(n_vars) + "var.txt";
    try {
        std::string db = Utils::loadFromTxt(filename);
        size_t pos = db.find("CLASS " + representative);
        if (pos != std::string::npos) {
            size_t end = db.find("---", pos);
            std::string result = db.substr(pos, end - pos);
            std::ofstream fout("output.txt");
            fout << "Found in database:\n" << result << "\n";
            fout.close();
            std::cout << "Saved database result to output.txt\n";
            return;
        }
    } catch (...) {
        std::cout << "Database not found, synthesizing...\n";
    }

    std::cout << "Not found in database. Synthesizing...\n";
    auto mig = synthesizeOptimalMIG(truth_table);
    if (mig) {
        std::ofstream fout("output.txt");
        fout << "Synthesized MIG:\n";
        fout << "Size: " << mig->size() << ", Depth: " << mig->depth() << "\n";
        fout.close();
        std::cout << "Saved synthesized MIG to output.txt\n";
    } else {
        std::cout << "Failed to synthesize MIG\n";
    }
}

// Optimized computeNPN function
void computeNPN(int n_vars) {
    std::cout << "Computing NPN classes for n=" << n_vars << "...\n";

    int num_rows = 1 << n_vars;
    int num_functions = 1 << num_rows;  // 2^(2^n)
    std::set<std::string> npn_classes;

    for (int func = 0; func < num_functions; ++func) {
        // Generate truth table string
        std::string tt(num_rows, '0');
        for (int row = 0; row < num_rows; ++row) {
            tt[row] = ((func >> row) & 1) ? '1' : '0';
        }

        // Compute NPN representative
        std::string rep = NPNClassifier::getNPNRepresentative(tt);
        npn_classes.insert(rep);

        // Optional: progress indicator every 1000 functions
        if (func % 1000 == 0) {
            std::cout << "\rProcessed " << func << "/" << num_functions << " functions..." << std::flush;
        }
    }

    std::cout << "\nFound " << npn_classes.size() << " unique NPN classes.\n";

    // Save NPN classes to file
    std::string out_file = "data/npn_" + std::to_string(n_vars) + "var.txt";
    std::ofstream fout(out_file);

    int class_index = 1;
    for (const auto& cls_tt : npn_classes) {
        fout << "CLASS " << class_index << ": " << cls_tt << "\n";

        // Synthesize MIG for each class
        auto mig = synthesizeOptimalMIG(cls_tt);
        if (mig) {
            fout << "  Size: " << mig->size() << ", Depth: " << mig->depth() << "\n";
        }
        fout << "---\n";

        class_index++;
    }

    fout.close();
    std::cout << "Saved NPN classes to " << out_file << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];

    if (command == "--help") {
        printUsage();
        return 0;
    }

    if (command == "--function" && argc >= 4) {
        std::string truth_table = argv[2];
        int n_vars = std::stoi(argv[3]);
        queryMIG(truth_table, n_vars);
        return 0;
    }

    if (command == "--compute" && argc >= 3) {
        int n_vars = std::stoi(argv[2]);
        computeNPN(n_vars);
        return 0;
    }

    if (command == "--test") {
        system("./test_maj3");
        system("./test_npn");
        system("./test_synthesis");
        return 0;
    }

    printUsage();
    return 1;
}
