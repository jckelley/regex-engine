#include <iostream>
#include <re2/re2.h>
#include <vector>
#include <fstream>
/*clock_gettime*/
#include <time.h>
/*timespec*/
#include <sys/time.h>

/*String functions*/
#include <string.h>
using namespace std;

//ms
long getDiff(timeval start, timeval stop) {
	return 1000 * (stop.tv_sec - start.tv_sec) + stop.tv_usec
			- start.tv_usec;
}

timeval getTime() {
	timeval time;
	gettimeofday(&time, NULL);
	return time;
}

//void test_fullmatch() {
//	string s, t;
//	string str("あぶらかたぶら");
//	re2::StringPiece input(str);
//	re2::RE2 re("(.ら).(.ぶ)");
//
//	if (re2::RE2::FullMatch(input, re, &s, &t))
//		cout << "s:" << s << " t:" << t << endl;
//	else
//		cout << "Not match." << endl;
//	if (re2::RE2::FullMatch(input, ".(.ら).(.ぶ).", &s, &t))
//		cout << "s:" << s << " t:" << t << endl;
//	else
//		cout << "Not match." << endl;
//
//}
//
//void test_partialmatch() {
//	string s, t;
//	string str("あぶらかたぶら");
//	re2::StringPiece input(str);
//	re2::RE2 re("(.ら).(.ぶ)");
//
//	if (re2::RE2::PartialMatch(input, re, &s, &t))
//		cout << "s:" << s << " t:" << t << endl;
//}
//
//void test_partialmatchn() {
//	string str("abcadca");
//	re2::RE2 re("((a.)(..))");
//	re2::StringPiece input(str);
//	int groupSize = re.NumberOfCapturingGroups();
//	vector < re2::RE2::Arg > argv(groupSize);
//	vector<re2::RE2::Arg*> args(groupSize);
//	vector < re2::StringPiece > ws(groupSize);
//	for (int i = 0; i < groupSize; ++i) {
//		args[i] = &argv[i];
//		argv[i] = &ws[i];
//	}
//	re2::RE2::PartialMatchN(input, re, &(args[0]), groupSize);
//	cout << groupSize << endl;
//	string s;
//	for (int i = 0; i < groupSize; ++i) {
//		cout << ws[i] << endl;
//	}
//}
//
//void test_findandconsume() {
//	string str("あぶらかたぶら");
//	RE2 re("(.ぶ)");
//	re2::StringPiece input(str);
//	string r;
//	while (re2::RE2::FindAndConsume(&input, re, &r)) {
//		cout << r << endl;
//	}
//}
//
//void test_replace() {
//	string s = "PerlRubyPython";
//	re2::RE2::Replace(&s, "P", "D");
//	cout << s << endl;
//}
//
//void test_globalreplace() {
//	string s = "PerlRubyPython";
//	re2::RE2::GlobalReplace(&s, "P", "D");
//	cout << s << endl;
//}
//
//void test_extract() {
//	string s;
//	RE2::Extract("foo@bar.com", "(.*)@([^.]*)",
//			"In domain \"\\2\", user \"\\1\" is exist!", &s);
//	cout << s << endl;
//}

string readInFile(string fileName) {
	//Fancy way to read an entire file in sequentially into a string
	//So we can work out of RAM rather than repeated I/Os
	ifstream in(fileName.c_str());
	string contents((istreambuf_iterator<char>(in)),
			istreambuf_iterator<char>());
	return contents;
}

int countRegexMatches(string corpus, string regex){
	int counter = 0;
	re2::StringPiece input(corpus);
	string match;
	while(re2::RE2::FindAndConsume(&input, regex, &match)){
		counter++;
	}
	return counter;
}
int main(int argc, char **argv) {
	if(argc!=3){
		printf("Insuff. Args!\n");
		exit(1);
	}
	string corpusFilename = argv[1];
	string regexFilename = argv[2];
	printf("Corpus is : %s\n", corpusFilename.c_str());
	printf("regex is : %s\n", regexFilename.c_str());
	string regex = readInFile(regexFilename);
	string corpus = readInFile(corpusFilename);
	int currIndex=0;
	while(currIndex> -1){
		int newCurrIndex = regex.find("\n",currIndex+1);
		if(newCurrIndex > -1){
			string regexIndiv = regex.substr(currIndex, newCurrIndex-currIndex);
			timeval start = getTime();
			int count = countRegexMatches(corpus,regexIndiv);
			timeval stop = getTime();
			printf("Took %ld ms with %d matches on %s!\n",getDiff(start,stop),count, regexIndiv.c_str());
		}

		currIndex = newCurrIndex;
	}
	return 0;
}
