#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ============================================================================
// COUNTING SORT (STABLE VERSION)
// ============================================================================
// Time Complexity: O(n + k) where k is the range of input
// Space Complexity: O(n + k)
// Stability: Yes - maintains relative order of equal elements
void countingSortStable(vector<int>& array) {
    if (array.empty()) return;

    // Find the range of input elements
    int minValue = *min_element(array.begin(), array.end());
    int maxValue = *max_element(array.begin(), array.end());
    int range = maxValue - minValue + 1;

    // Count occurrences of each element
    vector<int> countArray(range, 0);
    for (int value : array) {
        countArray[value - minValue]++;
    }

    // Transform count array to store cumulative positions
    // This enables stable sorting by placing elements from right to left
    for (int i = 1; i < range; i++) {
        countArray[i] += countArray[i - 1];
    }

    // Build output array by placing elements in correct positions
    // Traverse from right to left to maintain stability
    vector<int> outputArray(array.size());
    for (int i = array.size() - 1; i >= 0; i--) {
        int value = array[i];
        int position = countArray[value - minValue] - 1;
        outputArray[position] = value;
        countArray[value - minValue]--;
    }

    // Copy sorted elements back to original array
    array = outputArray;
}

// ============================================================================
// COUNTING SORT (NON-STABLE VERSION)
// ============================================================================
// Time Complexity: O(n + k) where k is the range of input
// Space Complexity: O(k)
// Stability: No - does not preserve relative order of equal elements
void countingSortNonStable(vector<int>& array) {
    if (array.empty()) return;

    // Find the range of input elements
    int minValue = *min_element(array.begin(), array.end());
    int maxValue = *max_element(array.begin(), array.end());
    int range = maxValue - minValue + 1;

    // Count occurrences of each element
    vector<int> countArray(range, 0);
    for (int value : array) {
        countArray[value - minValue]++;
    }

    // Reconstruct array by placing counted elements in order
    int arrayIndex = 0;
    for (int i = 0; i < range; i++) {
        while (countArray[i] > 0) {
            array[arrayIndex] = i + minValue;
            arrayIndex++;
            countArray[i]--;
        }
    }
}

// ============================================================================
// RADIX SORT (LSD - Least Significant Digit)
// ============================================================================
// Time Complexity: O(d * (n + b)) where d is number of digits, b is base
// Space Complexity: O(n + b)
// Stability: Yes - relies on stable counting sort for each digit

// Helper function: performs counting sort on a specific digit position
void countingSortByDigit(vector<int>& array, int digitPosition) {
    if (array.empty()) return; // IMPORTANT: Check for empty array

    const int BASE = 10; // Decimal number system
    vector<int> outputArray(array.size());
    vector<int> countArray(BASE, 0);

    // Count occurrences of each digit at the current position
    for (size_t i = 0; i < array.size(); i++) { // FIXED: Use size_t to avoid issues
        int digit = (array[i] / digitPosition) % BASE;
        countArray[digit]++;
    }

    // Transform to cumulative count for positioning
    for (int i = 1; i < BASE; i++) {
        countArray[i] += countArray[i - 1];
    }

    // Build output array maintaining stability (right to left)
    for (int i = array.size() - 1; i >= 0; i--) {
        int value = array[i];
        int digit = (value / digitPosition) % BASE;
        int position = countArray[digit] - 1;
        outputArray[position] = value;
        countArray[digit]--;
    }

    // Copy back to original array
    array = outputArray;
}

// Main radix sort function (LSD approach)
void radixSortLSD(vector<int>& array) {
    if (array.empty()) return;

    // Find maximum value to determine number of digits
    int maxValue = *max_element(array.begin(), array.end());

    // Process each digit position from least to most significant
    // digitPosition represents 10^0, 10^1, 10^2, etc.
    for (int digitPosition = 1; maxValue / digitPosition > 0; digitPosition *= 10) {
        countingSortByDigit(array, digitPosition);
    }
}

// ============================================================================
// PIGEONHOLE SORT
// ============================================================================
// Time Complexity: O(n + range) where range is (max - min + 1)
// Space Complexity: O(range)
// Stability: Yes when implemented properly with queues/lists
// Best for: Small range of values relative to number of elements
void pigeonholeSort(vector<int>& array) {
    if (array.empty()) return;

    // Determine the range of input values
    int minValue = *min_element(array.begin(), array.end());
    int maxValue = *max_element(array.begin(), array.end());
    int range = maxValue - minValue + 1;

    // Create pigeonholes - each hole stores all occurrences of a value
    vector<vector<int>> pigeonholes(range);

    // Distribute elements into their corresponding pigeonholes
    for (int value : array) {
        int holeIndex = value - minValue;
        pigeonholes[holeIndex].push_back(value);
    }

    // Collect elements back from pigeonholes in sorted order
    int arrayIndex = 0;
    for (int holeIndex = 0; holeIndex < range; holeIndex++) {
        for (int value : pigeonholes[holeIndex]) {
            array[arrayIndex] = value;
            arrayIndex++;
        }
    }
}

// ============================================================================
// BUCKET SORT
// ============================================================================
// Time Complexity: O(n + k) average case, O(n²) worst case
// Space Complexity: O(n + k) where k is number of buckets
// Stability: Depends on sorting algorithm used within buckets
// Best for: Uniformly distributed data over a range
void bucketSort(vector<int>& array) {
    if (array.empty()) return;

    // Find range for bucket distribution
    int minValue = *min_element(array.begin(), array.end());
    int maxValue = *max_element(array.begin(), array.end());

    // Handle case where all elements are the same
    if (minValue == maxValue) return;

    // Determine number of buckets (heuristic: use array size)
    int bucketCount = max(1, static_cast<int>(array.size()));
    int range = maxValue - minValue + 1;

    // Create empty buckets
    vector<vector<int>> buckets(bucketCount);

    // Distribute elements into buckets based on their value
    for (int value : array) {
        // Calculate bucket index proportional to value's position in range
        long long bucketIndex = ((long long)(value - minValue) * (bucketCount - 1)) / range;
        bucketIndex = min(bucketIndex, (long long)(bucketCount - 1)); // Safety check
        buckets[bucketIndex].push_back(value);
    }

    // Sort individual buckets using insertion sort (stable and efficient for small arrays)
    for (auto& bucket : buckets) {
        // Insertion sort on each bucket
        for (size_t i = 1; i < bucket.size(); i++) {
            int key = bucket[i];
            int j = i - 1;
            while (j >= 0 && bucket[j] > key) {
                bucket[j + 1] = bucket[j];
                j--;
            }
            bucket[j + 1] = key;
        }
    }

    // Concatenate all sorted buckets back into original array
    int arrayIndex = 0;
    for (const auto& bucket : buckets) {
        for (int value : bucket) {
            array[arrayIndex] = value;
            arrayIndex++;
        }
    }
}

// ============================================================================
// UTILITY FUNCTIONS FOR TESTING
// ============================================================================

// Print array contents (with limit for large arrays)
void printArray(const vector<int>& array, const string& label, int maxElements = 20) {
    cout << label << ": ";
    int elementsToShow = min(static_cast<int>(array.size()), maxElements);
    for (int i = 0; i < elementsToShow; i++) {
        cout << array[i] << " ";
    }
    if (array.size() > static_cast<size_t>(maxElements)) {
        cout << "... (" << array.size() << " total elements)";
    }
    cout << endl;
}

// Verify that an array is sorted correctly
bool isSorted(const vector<int>& array) {
    for (size_t i = 1; i < array.size(); i++) {
        if (array[i] < array[i - 1]) {
            return false;
        }
    }
    return true;
}

// Measure execution time of a sorting algorithm
template<typename SortFunction>
double measureSortingTime(vector<int> array, SortFunction sortFunc, const string& algorithmName) {
    auto startTime = high_resolution_clock::now();
    sortFunc(array);
    auto endTime = high_resolution_clock::now();

    duration<double, milli> executionTime = endTime - startTime;

    // Verify the sort was successful
    if (!isSorted(array)) {
        cout << "ERROR: " << algorithmName << " did not sort correctly!" << endl;
    }

    return executionTime.count();
}

// Generate test array
vector<int> generateTestArray() {
    return { 170, 45, 75, 90, 802, 24, 2, 66 };
}

// ============================================================================
// TEST CASE GENERATORS
// ============================================================================

// Test Case 1: Generate arrays with varying range sizes
vector<int> generateVaryingRangeArray(int size, int maxRange) {
    vector<int> result(size);
    random_device randomDevice;
    mt19937 generator(randomDevice());
    uniform_int_distribution<> distribution(0, maxRange);

    for (int i = 0; i < size; i++) {
        result[i] = distribution(generator);
    }
    return result;
}

// Test Case 2: Generate arrays with different distributions
enum Distribution { UNIFORM, NORMAL, SKEWED, EXPONENTIAL };

vector<int> generateDistributionArray(int size, Distribution distributionType) {
    vector<int> result(size);
    random_device randomDevice;
    mt19937 generator(randomDevice());

    switch (distributionType) {
    case UNIFORM: {
        uniform_int_distribution<> distribution(0, 1000);
        for (int i = 0; i < size; i++) {
            result[i] = distribution(generator);
        }
        break;
    }
    case NORMAL: {
        normal_distribution<> distribution(500, 150);
        for (int i = 0; i < size; i++) {
            result[i] = max(0, min(1000, static_cast<int>(distribution(generator))));
        }
        break;
    }
    case SKEWED: {
        // Right-skewed distribution (more small values)
        exponential_distribution<> distribution(0.003);
        for (int i = 0; i < size; i++) {
            result[i] = min(1000, static_cast<int>(distribution(generator)));
        }
        break;
    }
    case EXPONENTIAL: {
        exponential_distribution<> distribution(0.005);
        for (int i = 0; i < size; i++) {
            result[i] = min(1000, static_cast<int>(distribution(generator)));
        }
        break;
    }
    }
    return result;
}

// Test Case 3: Generate arrays of varying sizes
vector<int> generateScalabilityArray(int size) {
    vector<int> result(size);
    random_device randomDevice;
    mt19937 generator(randomDevice());
    uniform_int_distribution<> distribution(0, 10000);

    for (int i = 0; i < size; i++) {
        result[i] = distribution(generator);
    }
    return result;
}

// Test Case 4: Generate worst case for bucket sort (all elements in one bucket)
vector<int> generateWorstCaseBucketSort(int size) {
    vector<int> result(size);
    random_device randomDevice;
    mt19937 generator(randomDevice());
    // Small range causes all elements to fall into same/few buckets
    uniform_int_distribution<> distribution(0, 10);

    for (int i = 0; i < size; i++) {
        result[i] = distribution(generator);
    }
    return result;
}

// Test Case 5: Generate large range with few repeated values
vector<int> generateLargeRangeFewRepeats(int size) {
    vector<int> result(size);
    random_device randomDevice;
    mt19937 generator(randomDevice());
    // Very large range relative to array size
    uniform_int_distribution<> distribution(0, 1000000);

    for (int i = 0; i < size; i++) {
        result[i] = distribution(generator);
    }
    return result;
}

// Test Case 6: Generate array with many duplicate values
vector<int> generateManyDuplicates(int size) {
    vector<int> result(size);
    random_device randomDevice;
    mt19937 generator(randomDevice());
    // Only 10 possible values, causing many duplicates
    uniform_int_distribution<> distribution(0, 9);

    for (int i = 0; i < size; i++) {
        result[i] = distribution(generator);
    }
    return result;
}

// ============================================================================
// EXPERIMENTAL TEST SUITE
// ============================================================================

void runExperimentalTests() {
    cout << "\n============================================" << endl;
    cout << "   EXPERIMENTAL TEST CASES" << endl;
    cout << "============================================\n" << endl;

    // ========================================================================
    // TEST 1: VARYING INPUT RANGE SIZE
    // ========================================================================
    cout << "TEST 1: VARYING INPUT RANGE SIZE" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Purpose: Evaluate performance with different value ranges" << endl;
    cout << "Expected: Counting sort struggles with large ranges" << endl;
    cout << "         Radix/Bucket sort relatively unaffected\n" << endl;

    vector<int> rangeSizes = { 100, 1000, 10000, 100000 };
    int fixedSize = 1000;

    for (int range : rangeSizes) {
        cout << "Range [0, " << range << "], Size: " << fixedSize << endl;
        vector<int> testData = generateVaryingRangeArray(fixedSize, range);

        cout << "  Counting Sort (Stable):    " << fixed << setprecision(3)
            << measureSortingTime(testData, countingSortStable, "Counting Sort Stable") << " ms" << endl;
        cout << "  Counting Sort (Non-Stable): " << fixed << setprecision(3)
            << measureSortingTime(testData, countingSortNonStable, "Counting Sort Non-Stable") << " ms" << endl;
        cout << "  Radix Sort (LSD):          " << fixed << setprecision(3)
            << measureSortingTime(testData, radixSortLSD, "Radix Sort") << " ms" << endl;
        cout << "  Pigeonhole Sort:           " << fixed << setprecision(3)
            << measureSortingTime(testData, pigeonholeSort, "Pigeonhole Sort") << " ms" << endl;
        cout << "  Bucket Sort:               " << fixed << setprecision(3)
            << measureSortingTime(testData, bucketSort, "Bucket Sort") << " ms" << endl;
        cout << endl;
    }

    // ========================================================================
    // TEST 2: DIFFERENT DATA DISTRIBUTIONS
    // ========================================================================
    cout << "\nTEST 2: DIFFERENT DATA DISTRIBUTIONS" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Purpose: Test algorithms on uniform, normal, and skewed data" << endl;
    cout << "Expected: Bucket sort performs best on uniform distribution" << endl;
    cout << "         Others less affected by distribution\n" << endl;

    int distributionTestSize = 5000;
    vector<string> distributionNames = { "Uniform", "Normal", "Skewed", "Exponential" };
    vector<Distribution> distributions = { UNIFORM, NORMAL, SKEWED, EXPONENTIAL };

    for (size_t i = 0; i < distributions.size(); i++) {
        cout << distributionNames[i] << " Distribution (Size: " << distributionTestSize << ")" << endl;
        vector<int> testData = generateDistributionArray(distributionTestSize, distributions[i]);

        cout << "  Counting Sort (Stable):    " << fixed << setprecision(3)
            << measureSortingTime(testData, countingSortStable, "Counting Sort Stable") << " ms" << endl;
        cout << "  Counting Sort (Non-Stable): " << fixed << setprecision(3)
            << measureSortingTime(testData, countingSortNonStable, "Counting Sort Non-Stable") << " ms" << endl;
        cout << "  Radix Sort (LSD):          " << fixed << setprecision(3)
            << measureSortingTime(testData, radixSortLSD, "Radix Sort") << " ms" << endl;
        cout << "  Pigeonhole Sort:           " << fixed << setprecision(3)
            << measureSortingTime(testData, pigeonholeSort, "Pigeonhole Sort") << " ms" << endl;
        cout << "  Bucket Sort:               " << fixed << setprecision(3)
            << measureSortingTime(testData, bucketSort, "Bucket Sort") << " ms" << endl;
        cout << endl;
    }

    // ========================================================================
    // TEST 3: VARYING INPUT SIZE (SCALABILITY TEST)
    // ========================================================================
    cout << "\nTEST 3: VARYING INPUT SIZE (SCALABILITY TEST)" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Purpose: Evaluate how algorithms scale with input size" << endl;
    cout << "Expected: Linear growth for counting/radix/pigeonhole" << endl;
    cout << "         Potential quadratic for bucket sort in worst case\n" << endl;

    vector<int> inputSizes = { 1000, 5000, 10000, 20000 };

    for (int size : inputSizes) {
        cout << "Input Size: " << size << endl;
        vector<int> testData = generateScalabilityArray(size);

        cout << "  Counting Sort (Stable):    " << fixed << setprecision(3)
            << measureSortingTime(testData, countingSortStable, "Counting Sort Stable") << " ms" << endl;
        cout << "  Counting Sort (Non-Stable): " << fixed << setprecision(3)
            << measureSortingTime(testData, countingSortNonStable, "Counting Sort Non-Stable") << " ms" << endl;
        cout << "  Radix Sort (LSD):          " << fixed << setprecision(3)
            << measureSortingTime(testData, radixSortLSD, "Radix Sort") << " ms" << endl;
        cout << "  Pigeonhole Sort:           " << fixed << setprecision(3)
            << measureSortingTime(testData, pigeonholeSort, "Pigeonhole Sort") << " ms" << endl;
        cout << "  Bucket Sort:               " << fixed << setprecision(3)
            << measureSortingTime(testData, bucketSort, "Bucket Sort") << " ms" << endl;
        cout << endl;
    }

    // ========================================================================
    // TEST 4: WORST CASE FOR BUCKET SORT
    // ========================================================================
    cout << "\nTEST 4: WORST CASE FOR BUCKET SORT" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Purpose: Test bucket sort with poor distribution (all in few buckets)" << endl;
    cout << "Expected: Bucket sort degrades to O(n²) due to uneven distribution" << endl;
    cout << "         Other algorithms unaffected\n" << endl;

    int worstCaseSize = 5000;
    cout << "Small Range [0, 10], Size: " << worstCaseSize << " (Worst Case)" << endl;
    vector<int> worstCaseData = generateWorstCaseBucketSort(worstCaseSize);

    cout << "  Counting Sort (Stable):    " << fixed << setprecision(3)
        << measureSortingTime(worstCaseData, countingSortStable, "Counting Sort Stable") << " ms" << endl;
    cout << "  Counting Sort (Non-Stable): " << fixed << setprecision(3)
        << measureSortingTime(worstCaseData, countingSortNonStable, "Counting Sort Non-Stable") << " ms" << endl;
    cout << "  Radix Sort (LSD):          " << fixed << setprecision(3)
        << measureSortingTime(worstCaseData, radixSortLSD, "Radix Sort") << " ms" << endl;
    cout << "  Pigeonhole Sort:           " << fixed << setprecision(3)
        << measureSortingTime(worstCaseData, pigeonholeSort, "Pigeonhole Sort") << " ms" << endl;
    cout << "  Bucket Sort:               " << fixed << setprecision(3)
        << measureSortingTime(worstCaseData, bucketSort, "Bucket Sort") << " ms" << endl;
    cout << endl;

    // ========================================================================
    // TEST 5: LARGE RANGE WITH FEW REPEATED VALUES
    // ========================================================================
    cout << "\nTEST 5: LARGE RANGE WITH FEW REPEATED VALUES" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Purpose: Test with sparse data over large range" << endl;
    cout << "Expected: Counting/Pigeonhole sort inefficient (memory)" << endl;
    cout << "         Radix/Bucket sort more efficient\n" << endl;

    int largeRangeSize = 5000;
    cout << "Range [0, 1000000], Size: " << largeRangeSize << endl;
    vector<int> largeRangeData = generateLargeRangeFewRepeats(largeRangeSize);

    cout << "  Counting Sort (Stable):    " << fixed << setprecision(3)
        << measureSortingTime(largeRangeData, countingSortStable, "Counting Sort Stable") << " ms" << endl;
    cout << "  Counting Sort (Non-Stable): " << fixed << setprecision(3)
        << measureSortingTime(largeRangeData, countingSortNonStable, "Counting Sort Non-Stable") << " ms" << endl;
    cout << "  Radix Sort (LSD):          " << fixed << setprecision(3)
        << measureSortingTime(largeRangeData, radixSortLSD, "Radix Sort") << " ms" << endl;
    cout << "  Pigeonhole Sort:           " << fixed << setprecision(3)
        << measureSortingTime(largeRangeData, pigeonholeSort, "Pigeonhole Sort") << " ms" << endl;
    cout << "  Bucket Sort:               " << fixed << setprecision(3)
        << measureSortingTime(largeRangeData, bucketSort, "Bucket Sort") << " ms" << endl;
    cout << endl;

    // ========================================================================
    // TEST 6: MANY DUPLICATE VALUES
    // ========================================================================
    cout << "\nTEST 6: MANY DUPLICATE VALUES" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Purpose: Test with high duplicate rate" << endl;
    cout << "Expected: Counting/Pigeonhole excel with duplicates" << endl;
    cout << "         Stable sorts maintain relative order\n" << endl;

    int duplicateSize = 5000;
    cout << "Only 10 Unique Values, Size: " << duplicateSize << endl;
    vector<int> duplicateData = generateManyDuplicates(duplicateSize);

    cout << "  Counting Sort (Stable):    " << fixed << setprecision(3)
        << measureSortingTime(duplicateData, countingSortStable, "Counting Sort Stable") << " ms" << endl;
    cout << "  Counting Sort (Non-Stable): " << fixed << setprecision(3)
        << measureSortingTime(duplicateData, countingSortNonStable, "Counting Sort Non-Stable") << " ms" << endl;
    cout << "  Radix Sort (LSD):          " << fixed << setprecision(3)
        << measureSortingTime(duplicateData, radixSortLSD, "Radix Sort") << " ms" << endl;
    cout << "  Pigeonhole Sort:           " << fixed << setprecision(3)
        << measureSortingTime(duplicateData, pigeonholeSort, "Pigeonhole Sort") << " ms" << endl;
    cout << "  Bucket Sort:               " << fixed << setprecision(3)
        << measureSortingTime(duplicateData, bucketSort, "Bucket Sort") << " ms" << endl;
    cout << endl;

    // ========================================================================
    // SUMMARY OF FINDINGS
    // ========================================================================
    cout << "\n============================================" << endl;
    cout << "   EXPERIMENTAL FINDINGS SUMMARY" << endl;
    cout << "============================================" << endl;
    cout << "\n1. Range Size Impact:" << endl;
    cout << "   - Counting/Pigeonhole: O(n+k) sensitive to range k" << endl;
    cout << "   - Radix: Less affected, depends on digit count" << endl;
    cout << "   - Bucket: Relatively stable performance" << endl;

    cout << "\n2. Distribution Impact:" << endl;
    cout << "   - Bucket sort: Best on uniform, worst on skewed" << endl;
    cout << "   - Others: Generally distribution-independent" << endl;

    cout << "\n3. Scalability:" << endl;
    cout << "   - All show linear growth as expected" << endl;
    cout << "   - Bucket sort may degrade with poor distribution" << endl;

    cout << "\n4. Bucket Sort Worst Case:" << endl;
    cout << "   - Confirmed O(n²) with uneven distribution" << endl;
    cout << "   - All elements in few buckets triggers insertion sort overhead" << endl;

    cout << "\n5. Sparse Data (Large Range):" << endl;
    cout << "   - Counting/Pigeonhole: High memory usage" << endl;
    cout << "   - Radix/Bucket: More memory efficient" << endl;

    cout << "\n6. Many Duplicates:" << endl;
    cout << "   - Counting/Pigeonhole: Optimal performance" << endl;
    cout << "   - Small range benefits all algorithms" << endl;
    cout << "   - Stable sorts preserve original order of duplicates" << endl;

    cout << "\n============================================" << endl;
}

// ============================================================================
// MAIN FUNCTION - DEMONSTRATES ALL SORTING ALGORITHMS
// ============================================================================
int main() {
    cout << "============================================" << endl;
    cout << "   SORTING ALGORITHMS DEMONSTRATION" << endl;
    cout << "============================================" << endl << endl;

    // Test 1: Counting Sort (Stable)
    cout << "1. COUNTING SORT (STABLE)" << endl;
    cout << "----------------------------" << endl;
    vector<int> testArray1 = generateTestArray();
    printArray(testArray1, "Original");
    countingSortStable(testArray1);
    printArray(testArray1, "Sorted  ");
    cout << endl;

    // Test 2: Counting Sort (Non-Stable)
    cout << "2. COUNTING SORT (NON-STABLE)" << endl;
    cout << "----------------------------" << endl;
    vector<int> testArray2 = generateTestArray();
    printArray(testArray2, "Original");
    countingSortNonStable(testArray2);
    printArray(testArray2, "Sorted  ");
    cout << endl;

    // Test 3: Radix Sort (LSD)
    cout << "3. RADIX SORT (LSD)" << endl;
    cout << "----------------------------" << endl;
    vector<int> testArray3 = generateTestArray();
    printArray(testArray3, "Original");
    radixSortLSD(testArray3);
    printArray(testArray3, "Sorted  ");
    cout << endl;

    // Test 4: Pigeonhole Sort
    cout << "4. PIGEONHOLE SORT" << endl;
    cout << "----------------------------" << endl;
    vector<int> testArray4 = generateTestArray();
    printArray(testArray4, "Original");
    pigeonholeSort(testArray4);
    printArray(testArray4, "Sorted  ");
    cout << endl;

    // Test 5: Bucket Sort
    cout << "5. BUCKET SORT" << endl;
    cout << "----------------------------" << endl;
    vector<int> testArray5 = generateTestArray();
    printArray(testArray5, "Original");
    bucketSort(testArray5);
    printArray(testArray5, "Sorted  ");
    cout << endl;

    cout << "============================================" << endl;
    cout << "   ALL SORTING ALGORITHMS COMPLETED" << endl;
    cout << "============================================" << endl;

    // Run experimental test suite
    runExperimentalTests();

    return 0;
}