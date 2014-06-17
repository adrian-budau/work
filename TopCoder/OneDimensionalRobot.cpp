// Paste me into the FileEdit configuration dialog

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <iostream>
#include <deque>
#include <cassert>

using namespace std;

class OneDimensionalRobot {
  public:
    long long theSum( vector <string> commands1, vector <string> commands2, int minA, int maxA, int minB, int maxB ) {
        cerr << "test\n";
        string command;
        for (auto x : commands1)
            command += x;
        for (auto y : commands2)
            command += y;

        long long answer = 0;
        for (int A = -maxA; A <= -minA; ++A) {
            deque<int> positions;
            int where = 0;
            int real = 0;
            positions.push_back(maxB - minB + 1);

            for (auto ch : command)
                if (ch == 'L') {
                    if (where > A) {
                        --where;
                        --real;
                    } else {
                        if (positions.size() == 1)
                            continue;
                        auto a = positions.front();
                        positions.pop_front();
                        positions.front() += a;
                        --real;
                    }
                } else {
                    if (real == maxB)
                        continue;
                    if (where < minB) {
                        ++where;
                        ++real;
                        continue;
                    }

                    ++real;
                    int pointer;
                    for (pointer = 0; pointer < int(positions.size()); ++pointer)
                        if (positions[pointer] != 1)
                            break;
                    if (pointer < int(positions.size())) {
                        positions[pointer]--;
                        positions.push_front(1);
                    }
                }

            assert(where + int(positions.size()) == real + 1);
            for (int i = 0; i < int(positions.size()); ++i)
                answer += 1LL * positions[i] * (where + i);
        }

        return answer;
    }
};

// BEGIN CUT HERE
#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
namespace moj_harness {
	using std::string;
	using std::vector;
	int run_test_case(int);
	void run_test(int casenum = -1, bool quiet = false) {
		if (casenum != -1) {
			if (run_test_case(casenum) == -1 && !quiet) {
				std::cerr << "Illegal input! Test case " << casenum << " does not exist." << std::endl;
			}
			return;
		}
		
		int correct = 0, total = 0;
		for (int i=0;; ++i) {
			int x = run_test_case(i);
			if (x == -1) {
				if (i >= 100) break;
				continue;
			}
			correct += x;
			++total;
		}
		
		if (total == 0) {
			std::cerr << "No test cases run." << std::endl;
		} else if (correct < total) {
			std::cerr << "Some cases FAILED (passed " << correct << " of " << total << ")." << std::endl;
		} else {
			std::cerr << "All " << total << " tests passed!" << std::endl;
		}
	}
	
	int verify_case(int casenum, const long long &expected, const long long &received, std::clock_t elapsed) { 
		std::cerr << "Example " << casenum << "... "; 
		
		string verdict;
		vector<string> info;
		char buf[100];
		
		if (elapsed > CLOCKS_PER_SEC / 200) {
			std::sprintf(buf, "time %.2fs", elapsed * (1.0/CLOCKS_PER_SEC));
			info.push_back(buf);
		}
		
		if (expected == received) {
			verdict = "PASSED";
		} else {
			verdict = "FAILED";
		}
		
		std::cerr << verdict;
		if (!info.empty()) {
			std::cerr << " (";
			for (size_t i=0; i<info.size(); ++i) {
				if (i > 0) std::cerr << ", ";
				std::cerr << info[i];
			}
			std::cerr << ")";
		}
		std::cerr << std::endl;
		
		if (verdict == "FAILED") {
			std::cerr << "    Expected: " << expected << std::endl; 
			std::cerr << "    Received: " << received << std::endl; 
		}
		
		return verdict == "PASSED";
	}

	int run_test_case(int casenum__) {
		switch (casenum__) {
		case 0: {
			string commands1[]        = {"RRLRLLRRLL"};
			string commands2[]        = {};
			int minA                  = 2;
			int maxA                  = 2;
			int minB                  = 1;
			int maxB                  = 1;
			long long expected__      = -1;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}
		case 1: {
			string commands1[]        = {"RLRRLRLLRRLLLRLRLLRL"};
			string commands2[]        = {};
			int minA                  = 2;
			int maxA                  = 3;
			int minB                  = 1;
			int maxB                  = 2;
			long long expected__      = -9;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}
		case 2: {
			string commands1[]        = {"RLRRLRRRLLLLLRLRRLLLLRRRRLLRLLRLRRRLLRRLRLLRLLRRRL", "LRLRLRLLRLLLRRLLRLRRLLLRLLRLLRLLLLRRRLLRLRRRLLRRRR"};
			string commands2[]        = {};
			int minA                  = 3;
			int maxA                  = 5;
			int minB                  = 2;
			int maxB                  = 4;
			long long expected__      = 17;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}
		case 3: {
			string commands1[]        = {"LRRRRLLLRLRLLLRRLLLRRRLLLLLLRLLRLRRLLRLLLLLRRLLLLR",
 "RLLLLRRRLRLLRLRRLRLRRLLRRLRRRRLLLRRLLRRLRRRLLRLRLL",
 "RLRRRLLLRLRRLRLLLRLLLLRRRLLRLRRLRRRRRLRLLLLLRLLRLR",
 "LLRLRRRLRLLLRLLRRLRLRLRLRRRLLRRRLRRRLRLRLRRLLRLRLR",
 "LRRRRRLLLRLLRRLLRRRRLLLLRRRRLRRRLLLRRLLRRRRRRLLRLR",
 "LRLLRLRLLLLRLLLRLLRRRRLRRLLLLRRRLRRLRLRRLLLRLRLRLR"};
			string commands2[]        = {};
			int minA                  = 1;
			int maxA                  = 608;
			int minB                  = 1;
			int maxB                  = 608;
			long long expected__      = -1417089;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}
		case 4: {
			string commands1[]        = {"LRRRRRLRRRLLLRLLLLRLRLLLRLLRLLRRLRRLRRLLRLLLRLLLLL",
 "RRRLRLRRLLLRLLRRRRLLLRLRLRRLLLRLRRLLRLRLRLLLRLRLLR",
 "RLLLLLRLRRLLLRRRLLRLRLLRRLLLLRRLRLRLRRRLRRLRRLLRLL",
 "RRLRLRLRLLLLRRRRLLLRLLRLLLLRLLLRLLLRLRLRLRRRLRRRRR",
 "LRRLRLLRLLRRLRRLLRLRLRRLRRLLLLLLLRRRLRLLRRRRRRLRRR",
 "RLRLLLRLRLLRRLRLLLLLRLRRLRLLLRRRLRLRLRLLLRLRLLLRRL",
 "LRLRRLRRLLLRRLLLRRLRRRLLLLLLLLLLLLLRLLLRLLLRLLLRLL",
 "LLLRLRRRRLRRLLRLRLLLRLLLRRLRRLRLRRLLLLRRLRRRLLLRLR",
 "RLRLLRRRRRLLLRLRRLRLLRLRLLLRLLLRRLRLRLRRRLRRLLRRRL",
 "RLLLLRRRLRRLRRRLRRRLRLLRRRLLLRRLRLLLLLLLLRLLRLLRRL",
 "RRLLLRRLRLLRLLLRRRLRRLLLRLLRLRRRLRRRRRLRRRRRLRLLLR",
 "LLLLRLRLRLLLLRLLRRLLLRLLLRLLLLRLRRRLRRRLRLRLRLRLRR",
 "LLLLRLRLRLRLRLLLRLLRLLRLLLLLLLRRLRRRLRRLLLLLRLLLLL",
 "RRLLRLLLRRLRLLLLRRRRLLLLRLLRLLLLLRLLLLRRLRLLRLRLLR",
 "LRRLLLLRLLRRLRRRRRLLRLLLRLRLLLLLRLLRRLLLLLLRLRLLLR",
 "LRLLRRLLLRLRLLLLRLLRRLLLRLLLRLRLRRLRRRLLLLRRRLRLLL",
 "LLLRRLRLLLLRLRRLRLRLRRRRLLLLLLLRLLLRRLRRLLLLRLLLLR",
 "RRLRLLLRLRRLRRRLRLLRRLLLLLRLRLRRLRLLLLLLLLLRRLRRLL",
 "LRRLLLLRLLLRRRLLLRLLRRLLLRRLLLRRLLLRRLLRLRRRLLRLLL",
 "LRRRRLLRRLLLLLRLLLRLRRLLRLLLLLLLRRRLLLRLLLLLLLRLRR",
 "LLRRRLRLRLLRLRRLRLRRLLRLLLLLLRRLLRLRLRLLLLRLRLRRRR",
 "RRLRLLLLLRLRRLLLLLRLLLRLLLRLLRLLLLLRLLLRRLRRLLLLRR",
 "RLLLRLLLLLLLLLRRRLLLRRRLRRLRRRLLLRLRLLRLLLRLLRRRRL",
 "LRLLLRRRLRLRRLLRLLRLRRLRLRLLLLRLLLRLLLLRRRRLRLRLRR",
 "RLRRLLLLLRRLRLRLRRRLLLRLRRRLRLRLRLLRRLLLLRRLLRRRLL",
 "RLLRRLRRLRLRLRLLLLLLRLLLLRRLLRLRLRRLLRLRRLLRLRLLLR",
 "LRLLLLRLRLLLRLRLLRLLRRLLRLLLLLRRLRLRRLLLRLLLLLLRLL",
 "LRLRLRLLLLLLRRLLLRLLRLRLRRRRLLLLLRLLLLRLLRRLRLLLLL",
 "LLRLRRLLLRLLLRRLRLLRLRLLLLLLRLLRLRLRRLRLRLLLRRLLLR",
 "LLLLLLLLRRLLLLRLRRRLRRLLRRLRLLLLRLLRLRRLLLLLRRLLLL",
 "RRLLLLRRRRLLLLLRRLLLRLRLRRLLLLLLLRLRRRLRLLRLLLLLRL",
 "RLLRRLRRLRRLRLLRLLLRLRRRLRRLLLLLRRRLLRRLRLRLLRLLRL",
 "LRLRLLLRLLLRLRRRRRLRLRRLRLRLLLRRLRLLRLRLLLLRLLLLLR",
 "LLLLRRRRLLLRLLRRLLLLLRRLLLRLRRRLRLRLRRLLRLRRLLRLLL",
 "RLLRLLLRRLRRRRLLRLRRRRRLRLLRLLLLRRLLRLLLLRRLRLLLRL",
 "LRRLLLLRRLLRLLLLLRRLRLRRLRLLRRRRRLRLLLRLLRLRRLLLRR",
 "LLRRLLRLLRRLRRLRLRLLLRRRRLLLRLLRRLLLLRRLRLLRLLLLRR",
 "LRLLRRRRLLRLRLLLLLLLLLRLLRLLLRLRLRRRLLLLRRRRLRLLLR",
 "RRLLLLLLLLRRLLRRRLLRLLLLRLLLLLRLRLLLLRLRRRRRLRLLRL",
 "RRRLLRRLLLLLRRRLLLLLLLRLLLRRLLRLRRLLLRLRLLLLRLRLLL",
 "LLRRRRLRLRRRRRLRRRRRRRLLLLLLLLLLRRRRLLLRLRLLLLRRLL",
 "RLLLLRLLLRRLRRLRRLLRRLLLLLRLLRRRRRRLLLLRLLLLLRRLLL",
 "RLRLLRRRLLLLLRRRLLLRRLLLLLRRLLRRLRRRRLRRLLRRRLLLRL",
 "LLLRLLRLRRLLRRRLRRRLLLRLLLLLLRRLLRRRLLRRRRLRLLLRRL",
 "RLLLRLLRLLRLRLRLRLRRLLLLLLLRLRRLLRLRLRRRLLLRLLRRLR",
 "LLRLRLRLRLLLRRLLLRRLRRLLLLRRLLRLLLRLLRRLLRRRLRRLRL",
 "RRLRLRRLRRRLLLRRRLRRRLLLLLLLLLRRRRLRRLRRRLRLRRLRRL",
 "LLLRRLRLLRLRLLLRRLRRLLRLLLLRRLRRLRRLLLLRLLRLRLRRLL",
 "RLRLLLLLLRLRRLLRRRLLLRRLLRRLRLRRLLLRLLLLLRLRRLRLRR",
 "LLLLRRRRRLLLRLLRLRRLLRRRLRLLRLLLRLLLLRLRRRRLRRLLLL"};
			string commands2[]        = {"LLLLRRRLRLLLRRLRLRRLRLLLLLLLRRLRLRRLLRRLLRLRRRRLRL",
 "LLLRRLLRLRLLRLLLRLLLLRLLLLLRLLLLRRLRLRLRRLLLRRLLLL",
 "RRLLLRRRLRLLRLRLLRLLLRLLLRLLLLLLLRLLLLLLLLLRLLRLLR",
 "LLRLLRRRLLRLLRLRLRRLRLLLLLLLRLLLRRLRLLLLLLLLLLRRRL",
 "LLRRRRLLRLLLRRLLLLLLRLLLRRLLLRLRRRRLLLRRLLRLRLRLLL",
 "LLLRLLLRLLLLRLLLLLRRLLLRRLLLRLRRRRLLLRRLLLLLLLRLRL",
 "RLRRLRRRRRLRLRLLLRLRRRRLLRLRLLLLLLLLLLRRLLLLRRRLLL",
 "RRRLRLLRRRLLRRLLRLLLRLRLRRRLLRLLLLRLLRRRRLRRLLLRLL",
 "RLLRLLRLLLLLLRLLLRLLLLLRRRLRLRRRLLRRLRRRLLRRRRLLLR",
 "RRLRRRLLRLLLLRLRRLRLLLLLRLRRRLLLLLLRLLRLLRLLLRLLLL",
 "LLLRLLLRRLRLLLLLLLLRLLLLLLLLLRRLLLRLLRLLRLLLLLRLRR",
 "LRRRLLRRRRLRRLRLLRRRLLLLRRLRRLRRLRLLLLLLRLRRLLLRLL",
 "RLLLRRRRLLRLRLLRLLLRLLLRLLRRRLLRLLRRLLLLLLLRRRRRRL",
 "LLRRRLRLLRRRLRLLLRLRLLLLLRRLLRLLRLLLRLRLRLLRLLRLRR",
 "LLRLRRRRLLRRLLLRRLLRLLLLLLLRRLRLRLLRRLLLLRRRLLRRLR",
 "LRLLRLLRLLRLLLLLRLLRLLRLLRLRLLRLLLRLRLRLLLLLRLLLRR",
 "LRRRLRLLRLLRLLRRRLRRLRLLLRLRLLLLLLLLLRLRLRRRRRLRLL",
 "LLLLLLRLRLLLLRRRLRLLRRRRLRLRLLLLLRRLRRLLRRLLLRLLRL",
 "LRLRLLLLLLRLLLLLLLLLRLLLRLLRLLRLRRRRLRRRRLRLRLLLLL",
 "RLLLLLLRLRLLRRLRLLRLLRLLRLRLRRRLLRRLRLRLRRLLRRLRLR",
 "LRRRLRLLRLLLLLRLRLRLLLRLLLLLLRRLRLLLRLRLLRLLLLLRRL",
 "LRLLLRRLRLLLRLRRLLRLRLLLLLLRLLLLRRRLLLLRRRRLLRRRRL",
 "LRRLRLRLRRRRRRLRLLLRRRLLLRLLRRLLRLLRRLLRLLLRRRRLRL",
 "LLRLLRLRLRLLLRRLLLRLRLLLRLRRRLRLRLLLRRLLLRLRRLRLLR",
 "RRLLLLLLLLLLRRRLLLRRLLLLLRRLLLLRLRRLRLRLLLLLRRLLRL",
 "LRRRLLRLLLRRRLLLLRLRLLRRLLLRRRRRLLLLRRRLLLRRRLLLRL",
 "LRRLLLRLLRRLLLRLLRLRLRRLLLLRRRLLRLLLRLRLLRLLRLLLLL",
 "LRLLRRLLLLLLRRRLLRLLLLLLLLRRRLLLRLLRRRRLLLLLLLRLLL",
 "LLLRLLRRLLRLLRLRLLRRRLLLLLLRRLLLLLLLLLLLRLLLLLLRLR",
 "LLLLLRRRRLLLLLRRRRLLRLRRLLLLLLLLRRLLLRRRLRRLLRLRRL",
 "RLRLRLRRLLRLLRRRRLLRLLLLLRLRLLRLLLLLRLRRRRRLRRRLRR",
 "RLRRLRRRLLRLLRLRLRLLLLRLRLLLRLRRRLLLRRLLRRLLRLRRLL",
 "RLLLRRRRRLLLLLLLLLRRLRRLLLRLLLRLLLLRRLRLLLLLRLLLRR",
 "LLLRRRLLLLLLLLLRRLRLRLLLLRLLRLRRLLRRRLRRRRLRRRLLLL",
 "LRRLLLLLRLRLRLLRRRRLLLLRLRRLLRRRRLRLRLRRRRRRLRLLRL",
 "RRRLLRLLRLLRLRRLRRLLLLLRRLLLRLLRRLLLLRRRLRLRLRRLRL",
 "LRLRLLRLLRRRRLLRRRLLLLRRLRLLLLLLRRRLLRRLLLLLLRRRLL",
 "RRRRRLRLRLLRLLLRLRLRLLLLRLLLLRLRLLRLLRLLRLLRRRLLRL",
 "RLRRRLLLRLRLRRLLRLLLLLLLLLLRRLRRLRLLRRLRRLLRLRLLLL",
 "RRLRLRLRRLLLLLLRRLLLRRLRLLLLLLLLLLRLRLLLRLLRRLLLLR",
 "RLLLRLRRLLRLLRRRRLLRLLRLRLLLRLRLRRLLLLRLRLLRRRLLLL",
 "RLRLLRLLRRRLLLLRLLLRLLRLRRLLLLRRLRRRLRRRLLRLRRRRLR",
 "LLRLRLRLLRLRRRRRRLLLLLLLRLLLRLRLRLLRRRLRRLLLRLLRRL",
 "RLLRLLRRLRRLLLRLLRLLLLRRLLRLRRRLRRLRRLLRRRRRLLRLRL",
 "LRRLLLLLLLLLLRLLLLLRRLLLRRRRLRLRRLLLRRRRRRLRRRLRLL",
 "RRRLRLLLLLLLLRRLLRLRLRLRLLLLLRRLLRLLLLLRLLRRLLLRLL",
 "RLLLLRLRLLLLLRRLLLLRRLLRRLLLLLLRLLRLRLRLLRLLLLRLRR",
 "LLLRLRLRRRLRLLLRRLLLRLRLLRLRLLLLLLLLLRRLRLLLLRRRLR",
 "LRLRLLLLLRRLLRLRLLRLLLLRRLRRRLRRLRRRRLLLRRRLLRRLLL",
 "LRLLRRLRRLLLLRLLLLLLLLLLRLLLLLRRRLRLLLLLLLRLLRLLRR"};
			int minA                  = 1;
			int maxA                  = 5000;
			int minB                  = 1;
			int maxB                  = 5000;
			long long expected__      = -19478907170LL;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}

		// custom cases

/*      case 5: {
			string commands1[]        = ;
			string commands2[]        = ;
			int minA                  = ;
			int maxA                  = ;
			int minB                  = ;
			int maxB                  = ;
			long long expected__      = ;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}*/
/*      case 6: {
			string commands1[]        = ;
			string commands2[]        = ;
			int minA                  = ;
			int maxA                  = ;
			int minB                  = ;
			int maxB                  = ;
			long long expected__      = ;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}*/
/*      case 7: {
			string commands1[]        = ;
			string commands2[]        = ;
			int minA                  = ;
			int maxA                  = ;
			int minB                  = ;
			int maxB                  = ;
			long long expected__      = ;

			std::clock_t start__      = std::clock();
			long long received__      = OneDimensionalRobot().theSum(vector <string>(commands1, commands1 + (sizeof commands1 / sizeof commands1[0])), vector <string>(commands2, commands2 + (sizeof commands2 / sizeof commands2[0])), minA, maxA, minB, maxB);
			return verify_case(casenum__, expected__, received__, clock()-start__);
		}*/
		default:
			return -1;
		}
	}
}


#include <cstdlib>
int main(int argc, char *argv[]) {
	if (argc == 1) {
		moj_harness::run_test();
	} else {
		for (int i=1; i<argc; ++i)
			moj_harness::run_test(std::atoi(argv[i]));
	}
}
// END CUT HERE
