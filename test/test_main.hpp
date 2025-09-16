#ifndef _TEST_MAIN
#define _TEST_MAIN

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class TestHarness {
  private:
  TestHarness() {}
  std::vector<int(*)()> test_list;
  public:
  static TestHarness *inst() {
    static std::unique_ptr<TestHarness> instance(new TestHarness);
    return instance.get();
  }
  void add_test(int (*test)()) {this->test_list.push_back(test);}
  int run_tests(std::ostream& o = std::cout ) {
    unsigned int tests_passed = 0;
    //for (auto test : this->test_list) {
    for (int i = 0; i < this->n_tests(); i++) {
      o << "Running test " << i+1 << "/" << this->n_tests() << " ... ";
      if (this->test_list[i]() == 0) {
        o << "PASSED" << std::endl;
        tests_passed++;
      }
      else {
        o << "FAILED" << std::endl;
      }
    }
    o << tests_passed << "/" << this->n_tests() << " tests passed ";
    if (tests_passed == this->n_tests()) {
      o << "✅" << std::endl;
      return 0;
    }
    else {
      o << "❌" << std::endl;
      return 1;
    }
  }
  unsigned int n_tests() {return this->test_list.size();}
};

// test_args.cpp prototypes
int test_args();

// test_config.cpp prototypes
int test_config();

// test_sqlite.cpp prototypes
int test_sqlite_conn();

// test_archive.cpp prototypes
int test_archive();

// test_util.cpp prototypes
int test_util();

// tools/test_scratch.cpp prototypes
int test_ssak_scratch();

// util/test_tmpfiles.cpp prototypes
int test_util_tmpfiles();

#define REGISTER_TEST_ARGS \
do {\
  TestHarness::inst()->add_test(test_args);\
} while(0)

#define REGISTER_TEST_CONFIG \
do {\
  TestHarness::inst()->add_test(test_config);\
} while(0)

#define REGISTER_TEST_SQLITE \
do {\
  TestHarness::inst()->add_test(test_sqlite_conn);\
} while(0)

#define REGISTER_TEST_ARCHIVE \
do {\
  TestHarness::inst()->add_test(test_archive);\
} while(0)

#define REGISTER_TEST_UTIL \
do {\
  TestHarness::inst()->add_test(test_util);\
} while(0)

#define REGISTER_TEST_TOOLS_SCRATCH \
do {\
  TestHarness::inst()->add_test(test_ssak_scratch);\
} while(0)

#define REGISTER_TEST_UTIL_TMPFILES \
do {\
  TestHarness::inst()->add_test(test_util_tmpfiles);\
} while(0)

#define REGISTER_TESTS \
do {\
  REGISTER_TEST_ARGS;\
  REGISTER_TEST_CONFIG;\
  REGISTER_TEST_SQLITE;\
  REGISTER_TEST_ARCHIVE;\
  REGISTER_TEST_UTIL;\
  REGISTER_TEST_TOOLS_SCRATCH;\
  REGISTER_TEST_UTIL_TMPFILES;\
} while(0)
#endif