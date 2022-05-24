1. usage
1-1. ./stockserver {port} PRODUCTION {thread_}
1-2. ./stockserver {port} BENCHMARK {thread_n}: STDOUT CLOSED
2. stock file have to be same path with executable file in default
3. If you wanna change stockfile path, modify macro STOCK_FILE_PATH in "common.h" and compile again