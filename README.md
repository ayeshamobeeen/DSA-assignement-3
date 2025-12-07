# Non-Comparison Based Sorting Algorithms

A comprehensive C++ implementation and analysis of non-comparison based sorting algorithms with extensive performance testing and experimental analysis.

## Overview

This project implements and analyzes five efficient sorting algorithms that operate in linear or near-linear time by avoiding element comparisons. Each algorithm is thoroughly documented with complexity analysis, stability properties, and ideal use cases.

## Implemented Algorithms

### 1. Counting Sort (Stable & Non-Stable Versions)
- **Time Complexity:** O(n + k) where k is the range of input values
- **Space Complexity:** O(n + k)
- **Stability:** Configurable (both versions implemented)
- **Best for:** Small range of integer values with many duplicates

### 2. Radix Sort (LSD - Least Significant Digit)
- **Time Complexity:** O(d × (n + b)) where d is number of digits, b is base
- **Space Complexity:** O(n + b)
- **Stability:** Yes
- **Best for:** Integers with fixed number of digits, handles large ranges efficiently

### 3. Pigeonhole Sort
- **Time Complexity:** O(n + range)
- **Space Complexity:** O(range)
- **Stability:** Yes
- **Best for:** Small range relative to number of elements, many duplicates

### 4. Bucket Sort
- **Time Complexity:** O(n + k) average, O(n²) worst case
- **Space Complexity:** O(n + k)
- **Stability:** Depends on sub-sorting algorithm
- **Best for:** Uniformly distributed data over a known range

## Features

- **Complete Implementations:** Production-ready code with edge case handling
- **Performance Benchmarking:** High-resolution timing measurements for each algorithm
- **Comprehensive Testing:** Six experimental test suites analyzing different scenarios
- **Detailed Documentation:** Inline comments explaining algorithm mechanics
- **Correctness Verification:** Automated validation of sorting results

## Building and Running

### Prerequisites
- C++11 or higher
- Standard C++ compiler (g++, clang++, MSVC)

### Compilation
```bash
g++ -std=c++11 -O2 Source.cpp -o sorting_demo
```

### Execution
```bash
./sorting_demo
```

## Experimental Test Suite

The project includes six comprehensive test cases:

### Test 1: Varying Input Range Size
Evaluates performance across different value ranges (100 to 100,000) to identify memory efficiency characteristics.

**Key Findings:**
- Counting/Pigeonhole sort performance degrades with large ranges
- Radix and Bucket sort remain relatively stable

### Test 2: Different Data Distributions
Tests algorithms on uniform, normal, skewed, and exponential distributions.

**Key Findings:**
- Bucket sort performs optimally on uniform distributions
- Other algorithms show distribution-independent performance

### Test 3: Scalability Analysis
Measures how algorithms scale with input sizes from 1,000 to 20,000 elements.

**Key Findings:**
- All algorithms demonstrate linear growth as expected
- Confirms O(n + k) complexity in practice

### Test 4: Bucket Sort Worst Case
Deliberately creates poor bucket distributions to expose worst-case O(n²) behavior.

**Key Findings:**
- Bucket sort degrades significantly when all elements map to few buckets
- Other algorithms remain unaffected

### Test 5: Large Range with Sparse Data
Tests with range [0, 1,000,000] and only 5,000 elements.

**Key Findings:**
- Counting/Pigeonhole sort suffer from high memory usage
- Radix/Bucket sort handle sparse data more efficiently

### Test 6: Many Duplicate Values
Evaluates performance when only 10 unique values exist among 5,000 elements.

**Key Findings:**
- Counting/Pigeonhole sort excel with high duplicate rates
- Stable sorts correctly maintain relative ordering

## Sample Output

```
============================================
   SORTING ALGORITHMS DEMONSTRATION
============================================

1. COUNTING SORT (STABLE)
----------------------------
Original: 170 45 75 90 802 24 2 66 
Sorted  : 2 24 45 66 75 90 170 802 

[... additional demonstrations ...]

============================================
   EXPERIMENTAL TEST CASES
============================================

TEST 1: VARYING INPUT RANGE SIZE
------------------------------------------------------------
Range [0, 100], Size: 1000
  Counting Sort (Stable):    0.123 ms
  Radix Sort (LSD):          0.156 ms
  Bucket Sort:               0.089 ms
```

## Algorithm Selection Guide

| Scenario | Recommended Algorithm | Why? |
|----------|----------------------|------|
| Small range, many duplicates | Counting Sort | O(n + k) optimal, minimal overhead |
| Large integers, fixed digits | Radix Sort | Handles large ranges without memory bloat |
| Uniform distribution | Bucket Sort | Best average-case performance |
| Need stability | Counting (Stable) or Radix | Preserve relative order of equal elements |
| Sparse data over large range | Radix or Bucket | Memory efficient for sparse distributions |

## Key Insights

1. **Memory vs Speed Tradeoff:** Counting and Pigeonhole sorts trade memory for speed when the range is small
2. **Distribution Matters:** Bucket sort performance heavily depends on uniform distribution
3. **Stability Importance:** Stable algorithms are crucial for multi-key sorting scenarios
4. **Range Sensitivity:** Counting-based algorithms become inefficient with large sparse ranges
5. **Digit-Based Efficiency:** Radix sort's performance depends on number of digits, not value magnitude

## Performance Characteristics Summary

```
Algorithm         | Time (Avg)  | Space    | Stable | Range Sensitive
------------------|-------------|----------|--------|----------------
Counting (Stable) | O(n + k)    | O(n + k) | Yes    | High
Counting (Unstab) | O(n + k)    | O(k)     | No     | High
Radix (LSD)       | O(d(n + b)) | O(n + b) | Yes    | Low
Pigeonhole        | O(n + k)    | O(k)     | Yes    | High
Bucket            | O(n + k)*   | O(n + k) | Config | Medium

* O(n²) worst case with poor distribution
```

## Contributing

Contributions are welcome! Areas for enhancement:
- MSD (Most Significant Digit) Radix Sort implementation
- Negative number handling optimization
- Parallel sorting implementations
- Additional distribution test cases
- Visualization tools

## License

This project is provided as educational material for understanding non-comparison based sorting algorithms.

## References

- "Introduction to Algorithms" (CLRS) - Chapters on Linear-Time Sorting
- Analysis of bucket sort worst-case scenarios
- Stable sorting algorithm properties and applications

---

**Author:** Educational implementation for algorithm analysis  
**Last Updated:** December 2025
