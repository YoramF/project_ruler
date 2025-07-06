/*
https://projecteuler.net/problem=114

This problem is basically a set partitioning problem. 
I used the help of Google Gemini to find a solution to the following question:

Given two sets of n elements. one with non distinct grey items and one with non distinct red. 
We need to count how many ways we can order a new set of n elements were it has none or several red 
elements sets.

Elements can be of the following ways: 
    All element are the same color (2 options only), 
    Any other options, with the following constraints:
        Red elemets sets must have at least 3 elements.
        Grey sets can have any number of elemens.
        The new set can have m red sets and m-1 gray sets (n = m + m -1).

With the above definition Gemini produced a psudo code.


Answer:
16475640049

real    0m0.005s
user    0m0.001s
sys     0m0.004s

*/

#include <stdio.h>

// Helper function for binomial coefficient (nCk)
long unsigned int BinomialCoefficient(long unsigned int n, long unsigned int k) {
    long unsigned int result;

    if (k < 0 || k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n/2)
        k = n - k;

    // Calculate nCk efficiently (e.g., iterative or memoized)
    result = 1;
    for (long unsigned int i = 1; i <= k; i++)
        result = result * (n - i + 1) / i;

    return result;
}


// Helper function to count compositions of 'value' into 'parts' where each part is >= 'min_size'
// For indistinguishable items.
long unsigned int CountCompositions(long unsigned int value, long unsigned int parts, long unsigned int min_size) {
    long unsigned int adjusted_value;

    if (parts <= 0)
        return 0; // Must have at least one part
    if (value < parts*min_size)
        return 0; // Not enough value to satisfy min_size for all parts
    
    adjusted_value = value - (parts * min_size);
    
    // Number of compositions of 'adjusted_value' into 'parts' parts, each part >= 0
    // This is equivalent to choosing (parts - 1) "bars" from (adjusted_value + parts - 1) positions
    return BinomialCoefficient(adjusted_value + parts - 1, parts - 1);
}


// Main function to calculate total ways
long unsigned int CalculateOrderingWays(long unsigned int n) {
    long unsigned int total_ways = 0;

    // Case 1: All elements are the same color
    // 1a: All Grey (G G ... G)
    total_ways = total_ways + 1; // 1 way to order n identical grey items

    // 1b: All Red (R R ... R)
    if (n >= 3)
        total_ways = total_ways + 1; // 1 way to order n identical red items

    // Case 2: Mixed colors (contains both Red and Grey)
    // This implies that k (number of red items) is between 3 and n-1
    // And g (number of grey items) is between 1 and n-3
    
    for (long unsigned int k = 3; k <= n-1; k++) {
        // Iterate through possible numbers of red items
        long unsigned int g = n - k; // Number of grey items
        
        // Ensure there's at least one grey item for mixed colors
        if (g < 1)
            continue; // This k leads to no grey items, already handled by Case 1

        // Iterate through possible numbers of red blocks (m_R)
        // A red block must have at least 3 elements, so m_R * 3 <= k
        for (long unsigned int m_R = 1; m_R <= k/3; m_R++) {
            // Number of ways to partition 'k' red items into 'm_R' blocks, each >= 3
            long unsigned int num_red_block_compositions = CountCompositions(k, m_R, 3);
            
            // Only proceed 16475640049if red block compositions are possible
            if (num_red_block_compositions > 0) {
                
                // Calculate grey block compositions for different start/end patterns
                long unsigned int num_grey_block_compositions = 0;

                // Scenario D: Starts R, ends R (m_G = m_R - 1)
                // Requires at least one grey block (m_R - 1 >= 1 => m_R >= 2)
                if (m_R >= 2)
                    num_grey_block_compositions = num_grey_block_compositions + CountCompositions(g, m_R - 1, 1);

                // Scenario B & C: Starts R, ends G OR Starts G, ends R (m_G = m_R)
                // Two possibilities for these patterns
                num_grey_block_compositions = num_grey_block_compositions + (2 * CountCompositions(g, m_R, 1));

                // Scenario A: Starts G, ends G (m_G = m_R + 1)
                num_grey_block_compositions = num_grey_block_compositions + CountCompositions(g, m_R + 1, 1);
                
                // Add to total_ways for this k and m_R
                total_ways = total_ways + (num_red_block_compositions * num_grey_block_compositions);
            }
        }         
    }

    return total_ways;
}

int main () {
    long unsigned int n = 50;

    printf("%lu\n", CalculateOrderingWays(n));

    return 0;
}