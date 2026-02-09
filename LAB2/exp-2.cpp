#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <algorithm>
#include <iomanip>

using namespace std;

const int MATCH = 2;
const int MISMATCH = -1;
const int GAP = -1;

// Run Smith-Waterman with given thread count
double run_smith_waterman(int size, int threads, int &max_score_out) {

    string seq1(size, 'A');
    string seq2(size, 'A');

    int n = seq1.length();
    int m = seq2.length();

    vector<vector<int>> H(n + 1, vector<int>(m + 1, 0));

    int max_score = 0;

    double start = omp_get_wtime();

    for (int diag = 2; diag <= n + m; diag++) {

        #pragma omp parallel for num_threads(threads) reduction(max:max_score)
        for (int i = max(1, diag - m); i <= min(n, diag - 1); i++) {

            int j = diag - i;

            int score_diag = H[i-1][j-1] +
                (seq1[i-1] == seq2[j-1] ? MATCH : MISMATCH);

            int score_up = H[i-1][j] + GAP;
            int score_left = H[i][j-1] + GAP;

            H[i][j] = max({0, score_diag, score_up, score_left});

            max_score = max(max_score, H[i][j]);
        }
    }

    double end = omp_get_wtime();

    max_score_out = max_score;

    return end - start;
}

int main() {

    int size = 10000;   
    int max_score;

    cout << "Smith-Waterman Parallel Scaling\n";
    cout << "Matrix size: " << size << " x " << size << "\n\n";

    double T1 = run_smith_waterman(size, 1, max_score);

    cout << "Threads   Time(s)   Speedup   Efficiency\n";
    cout << "------------------------------------------\n";

    cout << fixed << setprecision(6);

    cout << 1 << "         "
         << T1 << "      "
         << 1.0 << "      "
         << 1.0 << endl;

    for (int t = 2; t <= 8; t *= 2) {

        double Tp = run_smith_waterman(size, t, max_score);

        double speedup = T1 / Tp;
        double efficiency = speedup / t;

        cout << t << "         "
             << Tp << "      "
             << speedup << "      "
             << efficiency << endl;
    }

    cout << "\nMaximum Local Alignment Score: " << max_score << endl;

    return 0;
}

