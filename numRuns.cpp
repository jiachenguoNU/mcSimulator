#include <iostream>
#include <vector>
#include <random>   // gives us the random tools: random_device, mt19937, uniform_int_distribution

// This program estimates the average number of "runs" in a sequence of n coin flips.
// A "run" is a streak of the same result. Example: H H T H -> runs are [HH][T][H] = 3 runs.
// We do it by simulation: flip n coins many times (trials) and average the run counts.
double simulate_runs(int n, int trials) {
    // --- Setting up the random number generator (2 pieces work together) ---
    //sd
    // mt19937 is the random number ENGINE (the "Mersenne Twister" algorithm). Given a
    // starting number called a "seed", it produces a long, fast stream of random-looking
    // numbers. The same seed always gives the same stream, so to get different results
    // each run we need a different seed each time.
    //
    // random_device is a source of "true" randomness from the operating system/hardware.
    // It is high quality but slow, so we use it just ONCE to pick the seed.
    // std::random_device{}() means: make a temporary random_device and call it once to
    // produce one random number, which we hand to the engine as its seed.
    std::mt19937 gen{std::random_device{}()};

    // A distribution reshapes the engine's huge raw numbers into the range we want.
    // uniform_int_distribution<...>{0, 1} gives 0 or 1 with equal probability -- a fair coin.
    // (0 = Tails, 1 = Heads). uint16_t is just a small unsigned integer type used to store it.
    std::uniform_int_distribution<std::int32_t> dist{0, 1}; // 0 代表 T, 1 代表 H

    // "long long" is an integer type, like "int", but it holds MUCH bigger numbers:
    //   - int       holds roughly +/- 2 billion      (about 2.1 x 10^9)
    //   - long long holds roughly +/- 9 quintillion  (about 9.2 x 10^18)
    // We sum a run count over every trial, so the running total can get large. Using
    // long long keeps it safe from "overflow" (wrapping around to a wrong/negative value)
    // even if trials becomes very big.
    long long total_runs{0};  //what is long long

    // Repeat the whole n-flip experiment `trials` times.
    for (int i{0}; i < trials; ++i) {
        int runs{1};                  // any non-empty sequence has at least 1 run
        int prev_flip{dist(gen)};     // dist(gen): pull one random 0/1 from the engine = first flip

        // Flip the remaining n-1 coins, comparing each to the previous one.
        for (int k{1}; k < n; ++k) {
            int current_flip{dist(gen)};
            if (current_flip != prev_flip) {
                ++runs; // 发生切换，游程数 +1  (the result changed, so a new run starts)
            }
            prev_flip = current_flip;
        }
        total_runs += runs;
    }

    // Average = total runs / number of trials.
    // static_cast<double> turns the integer total into a decimal first, so the division
    // keeps the fractional part (e.g. 5.48) instead of doing integer division.
    return static_cast<double>(total_runs) / trials;
}

int main() {
    int n{10};
    long long trials{1000}; // 模拟十万次  (number of simulation rounds)

    // Known theoretical answer for a fair coin: expected runs = (n + 1) / 2.
    // We compare our simulated average against this to check the simulation looks right.
    double expected{static_cast<double>(n + 1) / 2.0};

    std::cout << "Number of flips (n): " << n << '\n';
    std::cout << "Theoretical expected runs: " << expected << '\n';
    std::cout << "Simulated average runs:  " << simulate_runs(n, trials) << '\n';

    return 0;
}
