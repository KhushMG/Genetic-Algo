#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>

struct Solution
{
  double rank, x, y, z;
  void fitness()
  {

    // How close to 0 is the expression below?
    double ans = (6 * x + (-1 * y) + std::pow(z, 200)) - 25;
    // The closer to 0, the higher the rank.
    rank = (ans == 0) ? 9999 : std::abs(1 / ans);
  }
};

int main()
{
  // Create initial random solutions

  std::random_device device;
  std::uniform_real_distribution<double> unif(-100, 100);
  std::vector<Solution> solutions;

  const int NUM_SOLUTIONS = 100000;
  for (int i = 0; i < NUM_SOLUTIONS; i++)
  {
    solutions.push_back(Solution{
        0, unif(device),
        unif(device),
        unif(device)});
  }

  while (true)
  {
    // Evaluate fitness of initial solutions
    for (auto &s : solutions)
    {
      s.fitness();
    }

    // Sort solutions based on rank
    std::sort(solutions.begin(), solutions.end(), [](const auto &lhs, const auto &rhs)
              { return lhs.rank > rhs.rank; });

    // Print top solutions
    std::for_each(solutions.begin(),
                  solutions.begin() + 10, [](const auto &s)
                  { std::cout << std::fixed << "Rank: " << static_cast<int>(s.rank)
                              << "\n x: " << s.x << " y: " << s.y << " z: " << s.z << "\n"; });

    // Select top solutions (best candidates)
    const int SAMPLE_SIZE = 1000;
    std::vector<Solution> sample;
    std::copy(
        solutions.begin(),
        solutions.begin() + SAMPLE_SIZE,
        std::back_inserter(sample));

    solutions.clear();
    // Mutate the top solutions by a certain % (in this case 1%)
    std::uniform_real_distribution<double> m(0.99, 1.01);
    std::for_each(sample.begin(), sample.end(), [&](auto &s)
                  {
      s.x *= m(device);
      s.y *= m(device);
      s.z *= m(device); });

    // Perform crossover (randomly recombine previous solutions into new solutions)
    std::uniform_int_distribution<int> cross(0, SAMPLE_SIZE - 1);
    for (int i = 0; i < NUM_SOLUTIONS; i++)
    {
      solutions.push_back(Solution{
          0,
          sample[cross(device)].x,
          sample[cross(device)].y,
          sample[cross(device)].z,
      });
    }
  }
}
