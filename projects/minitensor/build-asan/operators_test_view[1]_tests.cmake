add_test([=[TestOperators.TestClose]=]  /home/hsj/ai-compiler-year-one/projects/minitensor/build-asan/bin/operators_test_view [==[--gtest_filter=TestOperators.TestClose]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TestOperators.TestClose]=]  PROPERTIES WORKING_DIRECTORY /home/hsj/ai-compiler-year-one/projects/minitensor/build-asan SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  operators_test_view_TESTS TestOperators.TestClose)
