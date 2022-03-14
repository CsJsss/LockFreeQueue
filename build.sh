# check LockFreeQueue
g++ -pthread LockFreeQueue.h  setting.h check.cpp -o check
echo "running check LockFreeQueue\n"
./check
rm ./check
echo "finish check LockFreeQueue\n"
echo "====================================================================="


# benchmark of LockFreeQueue
g++ -pthread LockFreeQueue.h  setting.h benchmark_lockfree.cpp -o benchmark_lockfree
echo "\nrunning benchmark LockFreeQueue\n"
./benchmark_lockfree
rm ./benchmark_lockfree
echo "\nfinish benchmark LockFreeQueue\n"
echo "====================================================================="


# benchmark of mutexQueue
g++ -pthread LockFreeQueue.h  setting.h benchmark_mutex.cpp -o benchmark_mutex
echo "running benchmark mutexQueue\n"
./benchmark_mutex
rm ./benchmark_mutex
echo "\nfinish benchmark mutexQueue\n"
echo "====================================================================="