/*
https://projecteuler.net/problem=121


## Calculating winning probabilities

Let's break down the game turn by turn and calculate the probability of the player winning. The player wins if they have taken more blue discs than red discs at the end of four turns. This can only happen in one of two ways:

1.  **Exactly 3 blue discs and 1 red disc.**
2.  **Exactly 4 blue discs and 0 red discs.**

### Turn-by-Turn Probabilities

* **Start:** The bag contains 1 red (R) and 1 blue (B) disc.
* **Turn 1:**
    * Probability of picking a blue disc (P(B_1)) = 1/2
    * Probability of picking a red disc (P(R_1)) = 1/2
    * After the turn, a red disc is added. The bag now contains 2R and 1B.
* **Turn 2:**
    * Probability of picking a blue disc (P(B_2)) = 1/3
    * Probability of picking a red disc (P(R_2)) = 2/3
    * After the turn, a red disc is added. The bag now contains 3R and 1B.
* **Turn 3:**
    * Probability of picking a blue disc (P(B_3)) = 1/4
    * Probability of picking a red disc (P(R_3)) = 3/4
    * After the turn, a red disc is added. The bag now contains 4R and 1B.
* **Turn 4:**
    * Probability of picking a blue disc (P(B_4)) = 1/5
    * Probability of picking a red disc (P(R_4)) = 4/5

***

## Calculating the Winning Probabilities

### Case 1: 4 Blue discs, 0 Red discs

This outcome requires the player to draw a blue disc on every single turn. The probability is the product of the probabilities of drawing a blue disc at each turn:

* P({4 Blue}) = P(B_1) x P(B_2) x P(B_3) x P(B_4)
* P({4 Blue}) = (1/2) x (1/3) x (1/4) x (1/5) = 1/120

### Case 2: 3 Blue discs, 1 Red disc

This outcome requires the player to draw exactly one red disc and three blue discs over the four turns. We need to calculate the probability for each possible sequence and then add them up. The single red disc could be drawn on turn 1, 2, 3, or 4.

* **Red on Turn 1:** P(R_1, B_2, B_3, B_4) = (1/2) x (1/3) x (1/4) x (1/5) = 1/120
* **Red on Turn 2:** P(B_1, R_2, B_3, B_4) = (1/2) x (2/3) x (1/4) x (1/5) = 2/120
* **Red on Turn 3:** P(B_1, B_2, R_3, B_4) = (1/2) x (1/3) x (3/4) x (1/5) = 3/120
* **Red on Turn 4:** P(B_1, B_2, B_3, R_4) = (1/2) x (1/3) x (1/4) x (4/5) = 4/120

Now, we add the probabilities of these mutually exclusive events:
* P({3 Blue, 1 Red}) = 1/120 + 2/120 + 3/120 + 4/120 = 10/120

***

## Final Calculation

The total probability of winning is the sum of the probabilities of the two winning scenarios (4 blue discs or 3 blue discs):

* P({Winning}) = P({4 Blue}) + P({3 Blue, 1 Red})
* P({Winning}) = 1/120 + 10/120 = 11/120

The initial statement is therefore **true**. The probability is indeed exactly 11/120.

*********************
Calculating the maximum allowed prize:

### Expected Value

The **expected value** (E[X]) is the long-run average outcome of a random event. In this game, it represents the average amount the player is expected to win or lose over many games.

The formula for expected value is:
E[X] = \sum (x_i x P(x_i))
Where:
* x_i is the value of each possible outcome.
* P(x_i) is the probability of that outcome.

In this context, the expected payout from the banker's perspective is the probability of the player winning multiplied by the prize money.

* **Probability of winning:** As we've established, the probability of the player winning is **11/120**.
* **Prize money:** The statement says the prize includes the original £1, so a payout of £10 means the player's winnings are £9 plus their initial stake.

So, the banker's expected payout per game is:
E[{payout}] = ({probability of winning}) x ({prize money})
E[{payout}] = (11/120) x £10 = £110/120 = £11/12

This means that on average, the banker expects to pay out £11/12 per game.

---

### Banker's Profitability

The banker's profit is the amount they take in (£1) minus the expected payout.
* **Banker's Expected Profit** = £1 - £11/12 = £1/12

Since the banker's expected profit is positive (£1/12), they would, on average, make money from the game at a £10 prize.

If the banker set the prize at **£11**, the expected payout would be:
E[{payout}] = (11/120) x £11 = £121/120

In this scenario, the banker's expected profit would be:
* **Banker's Expected Profit** = £1 - £121/120 = -£1/120

A negative expected profit means the banker would, on average, lose money on the game.

Therefore, the maximum prize fund the banker should allocate to avoid an expected loss is **£10**, as this is the highest whole number prize that keeps their expected profit positive.

******
How to get to prize value?

The phrase "the maximum prize fund the banker should allocate for winning in this game would be £10 before they would expect to incur a loss" is a conclusion drawn from a calculation based on the principles of **expected value**. It's not a pre-written rule of a game, but rather an economic analysis of the game's payout structure.

To understand this statement, you need to follow the logic:

1.  **Banker's Goal**: The banker wants to make a profit. They collect £1 from each player. To not lose money on average, their average payout to winning players must be less than or equal to the £1 they collect.
2.  **Expected Payout**: The average payout is calculated as the probability of a player winning multiplied by the prize money.
    * Probability of winning = **11/120**.
    * Let the prize money be **X** pounds.
    * Expected Payout = (11/120) x X.
3.  **Break-Even Point**: The banker "expects to incur a loss" if their expected payout is greater than the £1 they collect from each player.
    * Expected Payout ≤ £1
    * (11/120) x X ≤ 1
4.  **Solving for X**: To find the maximum prize money (X) that the banker can offer without an expected loss, you solve the inequality:
    * 11X ≤ 120
    * X ≤ 120/11
    * X ≤ 10.90909...
5.  **Final Conclusion**: Since the prize must be a whole number of pounds, the largest whole number that satisfies the condition X ≤ 10.90909... is **£10**.

Any prize money of £11 or more would cause the expected payout to exceed £1, meaning the banker would, on average, lose money over many games. Therefore, the **maximum prize fund** the banker can allocate for a winning player while expecting to make a profit is **£10**.


I am using the GMP library to handle accurate fraction numbers

Prize: 2269

real    0m0.069s
user    0m0.060s
sys     0m0.003s

*/

#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#if DEBUG
#include <assert.h>
#endif

#define TURNS       15              // numbers of bets
#define PAY         1               // pay per turn

typedef struct {
    int32_t n;
    int32_t inds [TURNS];
} IND_COMB;

static mpq_t _wins [TURNS];         // propabilities to win per turn
static mpq_t _losses [TURNS];       // propabilities to lose per tuen;
static int32_t _indexes [TURNS];    // indexes to _wins & _losses

void init_wins_losses () {
    int32_t i, l, p;

    for (i = 0; i < TURNS; i++) {
        p = i + 2;
        l = p - 1;
        mpq_init(_wins[i]);
        mpq_init(_losses[i]);
        mpq_set_si(_wins[i], 1, p);
        mpq_set_si(_losses[i], l, p);
        _indexes[i] = i;
    }
}

void clear_wins_losses () {
    int32_t i;

    for (i = 0; i < TURNS; i++) {
        mpq_clear(_wins[i]);
        mpq_clear(_losses[i]);
    }    
}

/*
Return the minimum prize allocation
f - propability to win
p - payment per turn
*/ 
int32_t calc_prize (mpq_t f, int32_t p) {
    mpz_t dz, nz, pz;
    int32_t prize;

    mpz_init(dz);
    mpz_init(nz);
    mpz_init_set_si(pz, p);
    mpq_get_num(nz, f);      // x = f numerator
    mpq_get_den(dz, f);      // y = f denominator
    
    mpz_mul(pz, pz, dz);
    mpz_div(pz, pz, nz);
    
    prize = mpz_get_si(pz);

    mpz_clear(dz);
    mpz_clear(nz);
    mpz_clear(pz);

    return prize;
};


/* 
    generate combinarions selecting k items out of n
    s_i start of list. in this case, the list is the index
*/
#if DEBUG
void generate_combinations(int32_t s_i, int32_t c_c[], int32_t indexes[], int32_t c_c_len, int32_t k, int32_t n, IND_COMB *all_comb, int32_t *c_c_ind, int32_t max_comb) {
#else
void generate_combinations(int32_t s_i, int32_t c_c[], int32_t indexes[], int32_t c_c_len, int32_t k, int32_t n, IND_COMB *all_comb, int32_t *c_c_ind) {
#endif
    // Base Case: If the current combination has k elements, add it to the result
    if (c_c_len == k) {
        #if DEBUG
        assert(*c_c_ind < max_comb);
        assert(c_c_len <= TURNS);
        #endif
        all_comb[*c_c_ind].n = c_c_len;
        memcpy(all_comb[*c_c_ind].inds, c_c, c_c_len*sizeof(int32_t));
        (*c_c_ind)++;
        return;
    }

    // If we've gone past the last element or don't have enough elements left
    // to form a combination of size k
    if (s_i >= n)
        return;
        
    // Optimization: If there aren't enough remaining elements to form a combination of size k
    // (len(elements) - start_index) < (k - len(current_combination))
    // This optimization can be added for efficiency but is not strictly necessary for correctness.
    if ((n-s_i) < (k-c_c_len))
        return;


    // Recursive Step: Iterate through elements from start_index
    for (int32_t i = s_i; i < n; i++) {
        // 1. Include the current index
        c_c[c_c_len] = indexes[i];
        c_c_len++;

        // 2. Recurse with the next index (i + 1)
        #if DEBUG
        generate_combinations(i+1, c_c, indexes, c_c_len, k, n, all_comb, c_c_ind, max_comb);
        #else
        generate_combinations(i+1, c_c, indexes, c_c_len, k, n, all_comb, c_c_ind);
        #endif

        // 3. Backtrack: Remove the last added element to explore other possibilities
        c_c_len--;
    }

    return;
}


/*
    calculate a wining propability based on wins_ind combination
*/
void calc_win_prob (mpq_t *result, int32_t wins_ind[], int32_t wins_cnt) {
    int8_t indexes [TURNS];
    int32_t i;

    // reset indexes
    memset(indexes, 0, sizeof(indexes));

    // init result
    mpq_set_si(*result, 1, 1);

    // work on wins
    for (i = 0; i < wins_cnt; i++) {
        indexes[wins_ind[i]] = 1;
        mpq_mul(*result, *result, _wins[wins_ind[i]]);
    }

    // add losses
    for (i = 0; i < TURNS; i++) {
        if (indexes[i] == 0)
            mpq_mul(*result, *result, _losses[i]);
    }
}

// calculate binomial formula. limited to n<=30
int32_t ncm (uint32_t n, uint32_t m) {
    uint32_t r;

    r = 1;
    for (int32_t i = 1; i <= m; i++) {
        r *= (n - i + 1);
        r /= i;
    }

    return (int32_t)r;
}

int32_t main () {
    int32_t comb[TURNS];

    IND_COMB *all_comb;     // max combinations for 15C8. Other options are less
    int32_t n_comb;
    int32_t i, j, k, max_comb, min_wins;
    mpq_t prob, win_prob;
    int32_t prize;

    min_wins = TURNS / 2 + 1;           // minimum wins required in order to gain the prize
    max_comb = ncm(TURNS, min_wins);

    // init prob and win_prob
    mpq_init(prob);
    mpq_init(win_prob);
    mpq_set_si(win_prob, 0, 1);


    // allocate all_combs
    if ((all_comb=malloc(sizeof(IND_COMB)*max_comb)) == NULL) {
        perror("Failed to allocat RAM\n");
        exit(EXIT_FAILURE);
    }

    init_wins_losses();

    for (i = 0; i < TURNS; i++) {
        n_comb = 0;

        #if DEBUG
        generate_combinations(0, comb, _indexes, 0, i+min_wins, TURNS, all_comb, &n_comb, max_comb);
        #else
        generate_combinations(0, comb,  _indexes, 0, i+min_wins, TURNS, all_comb, &n_comb);
        #endif

        for (int i = 0; i < n_comb; i++) {
            // calculate propability
            calc_win_prob(&prob, all_comb[i].inds, all_comb[i].n);

            // add current win propability to total win propability
            mpq_add(win_prob, win_prob, prob);
        }
    }

    // calculate prize
    prize = calc_prize(win_prob, PAY);

    printf("Prize: %d\n", prize);

    mpq_clear(prob);
    mpq_clear(win_prob);
    clear_wins_losses();
    free(all_comb);

    return EXIT_SUCCESS;
}