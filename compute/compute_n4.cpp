#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>
#include "../src/truth_table.h"
#include "../src/npn_classifier.h"
#include "../src/mig_synthesizer.h"
#include "../src/utils.h"

void computeN4(int max_classes = -1) {
    Utils::Timer timer("compute_n4");
    
    int n_vars = 4;
    Utils::logMessage("Computing NPN classes for n=" + std::to_string(n_vars));
    
    auto all_tables = TruthTable::generateAllTruthTables(n_vars);
    std::cout << "Generated " << all_tables.size() << " truth tables" << std::endl;
    
    auto npn_classes = NPNClassifier::findNPNClasses(all_tables);
    std::cout << "Found " << npn_classes.size() << " NPN classes" << std::endl;
    
    if (max_classes > 0 && max_classes < npn_classes.size()) {
        npn_classes.resize(max_classes);
        std::cout << "Processing first " << max_classes << " classes only" << std::endl;
    }
    
    std::stringstream database;
    double total_time = 0;
    int failed_count = 0;
    int success_count = 0;
    std::map<int, int> size_distribution;
    
    for (size_t i = 0; i < npn_classes.size(); ++i) {
        const auto& rep = npn_classes[i];
        std::cout << "\n[" << (i+1) << "/" << npn_classes.size() << "] Processing: " << rep << std::endl;
        
        auto start_time = std::chrono::high_resolution_clock::now();
        auto mig = synthesizeOptimalMIG(rep);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        double synthesis_time = std::chrono::duration<double>(end_time - start_time).count();
        total_time += synthesis_time;
        
        if (mig) {
            int size = mig->size();
            size_distribution[size]++;
            success_count++;
            
            database << "CLASS " << rep << "\n";
            database << mig->toText();
            database << "TIME " << synthesis_time << "\n";
            database << "---\n";
            
            std::cout << "  ✓ Size: " << size << ", Depth: " << mig->depth() 
                     << ", Time: " << synthesis_time << "s" << std::endl;
            
            if (size == 7) {
                std::cout << "  >>> Found a 7-gate function!" << std::endl;
            }
        } else {
            failed_count++;
            std::cout << "  ✗ Failed to synthesize (timeout or too complex)" << std::endl;
        }
    }
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "SYNTHESIS COMPLETE" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Successfully synthesized: " << success_count << "/" << npn_classes.size() << std::endl;
    std::cout << "Failed: " << failed_count << std::endl;
    std::cout << "Total time: " << total_time << " seconds" << std::endl;
    std::cout << "Average time per class: " << total_time/npn_classes.size() << " seconds" << std::endl;
    
    std::cout << "\nSize distribution:" << std::endl;
    for (const auto& [size, count] : size_distribution) {
        std::cout << "  " << size << " gates: " << count << " functions" << std::endl;
    }
    
    Utils::saveToTxt(database.str(), "data/npn_" + std::to_string(n_vars) + "var.txt");
    Utils::logMessage("Completed n=" + std::to_string(n_vars) + ": " + 
                     std::to_string(success_count) + " classes in " + 
                     std::to_string(total_time) + "s");
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        int max_classes = std::stoi(argv[1]);
        computeN4(max_classes);
    } else {
        computeN4();
    }
    
    return 0;
}