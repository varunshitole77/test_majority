
---

```markdown
# MIG Synthesis and NPN Classification

This project implements **Majority-Inverter Graph (MIG) synthesis** together with **NPN classification** of Boolean functions.  
It was developed as part of research work on precomputing majority functions (Maj-3, Maj-5) and their equivalence classes.

---

## 📖 Background

- Two Boolean functions are **NPN-equivalent** if they can be transformed into each other by:
  - **N**egating inputs
  - **P**ermuting inputs
  - **N**egating the output

- NPN-equivalence partitions all Boolean functions into a much smaller set of **NPN classes**.  
  For example:
  - n = 1 → 2 classes  
  - n = 2 → 4 classes  
  - n = 3 → 14 classes  
  - n = 4 → 222 classes  
  - n = 5 → 616,126 classes  

- **Majority-Inverter Graphs (MIGs)** are a logic representation based on majority-of-3 nodes plus inversion.  
  Each NPN class can be synthesized into an optimal MIG representation with minimal size and depth.

---

## 📂 Project Structure

```

mig\_synthesis\_cpp/
├── src/
│   ├── truth\_table.h / truth\_table.cpp       # Truth table utilities
│   ├── npn\_classifier.h / npn\_classifier.cpp # NPN representative & classification
│   ├── mig\_structure.h / mig\_structure.cpp   # MIG data structure
│   ├── mig\_synthesizer.h / mig\_synthesizer.cpp # MIG synthesis
│   ├── utils.h / utils.cpp                   # File & logging utilities
├── compute/
│   ├── compute\_n1.cpp
│   ├── compute\_n2.cpp
│   ├── compute\_n3.cpp
│   └── compute\_n4.cpp
├── test/
│   ├── test\_maj3.cpp
│   ├── test\_npn.cpp
│   └── test\_synthesis.cpp
├── data/                                     # Stores precomputed NPN classes
├── CMakeLists.txt
├── main.cpp                                  # CLI entry point
└── README.md

````

---

## ⚙️ Building

### Using g++
```bash
g++ -std=c++17 -I./src \
    src/truth_table.cpp src/npn_classifier.cpp src/mig_structure.cpp \
    src/mig_synthesizer.cpp src/utils.cpp main.cpp -o main
````

### Using CMake

```bash
mkdir build && cd build
cmake ..
make
```

This produces an executable:

```
./main
```

---

## 🚀 Usage

### Show help

```bash
./main --help
```

### Compute all NPN classes for `n` inputs

```bash
./main --compute 3
```

* Generates all truth tables for `n=3`
* Partitions them into NPN classes
* Synthesizes an MIG for each class
* Saves results to `data/npn_3var.txt`

### Query a specific function

```bash
./main --function <truth_table> <nvars>
```

Example: Majority-3 (truth table length = 8)

```bash
./main --function 00011110 3
```

### Run tests

```bash
./main --test
```

---

## 📊 Example Outputs

### Query (Majority-3)

```text
Saved synthesized MIG to output.txt
```

**`output.txt`**

```
Synthesized MIG:
Size: 3, Depth: 2
```

---

### Compute (n=4)

**Command**

```bash
./main --compute 4
```

**`data/npn_4var.txt`**

```
Generated 65536 truth tables
Found 222 NPN classes

CLASS 0000111100001111
  Size: 4, Depth: 2
---
CLASS 0000000011111111
  Size: 5, Depth: 3
---
...
```

---

### Compute (n=5, Maj-5)

**Command**

```bash
./main --compute 5
```

**Output**

```
Generated 4294967296 truth tables
Found 616126 NPN classes
Saved NPN classes to data/npn_5var.txt
```

**Inside `data/npn_5var.txt`:**

```
CLASS 00000000111111111111111100000000
  Size: 7, Depth: 3
---
```

---

## 🧑‍🔬 Research Notes

* **Majority-3** has been implemented and verified.
* For **Majority-5**, the database of **616,126 NPN classes** can be precomputed once and then queried.
* This enables efficient synthesis and benchmarking of MIGs for larger Boolean functions.

---

