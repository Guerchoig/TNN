#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// This function returns a uniformly random sample of size k from the
// first n elements of the input vector.
vector<int> reservoir_sample(vector<int> &v, int k) {
  // Initialize the reservoir with the first k elements of the input vector.
  vector<int> reservoir(k);
  for (int i = 0; i < k; i++) {
    reservoir[i] = v[i];
  }

  // For each element in the input vector after the first k elements,
  // randomly select an element from the reservoir with probability 1 / k
  // and replace it with the new element.
  for (int i = k; i < v.size(); i++) {
    int j = rand() % k;
    reservoir[j] = v[i];
  }

  return reservoir;
}

// int main() {
//   // Create a vector of integers.
//   vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

//   // Get a uniformly random sample of size 5 from the vector.
//   vector<int> sample = reservoir_sample(v, 5);

//   // Print the sample.
//   for (int i = 0; i < sample.size(); i++) {
//     cout << sample[i] << " ";
//   }
//   cout << endl;

//   return 0;
// }