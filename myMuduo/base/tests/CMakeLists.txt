add_executable(asyncLogger_test AsyncLogger_test.cpp)
target_link_libraries(asyncLogger_test myMuduo_base)

add_executable(atomic_unittest Atomic_unittest.cpp)
add_test(NAME atomic_unittest COMMAND atomic_unittest)

add_executable(blockingqueue_test BlockingQueue_test.cpp)
target_link_libraries(blockingqueue_test myMuduo_base)

add_executable(blockingqueue_bench BlockingQueue_bench.cpp)
target_link_libraries(blockingqueue_bench myMuduo_base)

add_executable(boundedblockingqueue_test BoundedBlockingQueue_test.cpp)
target_link_libraries(boundedblockingqueue_test myMuduo_base)

add_executable(date_unittest Date_unittest.cpp)
target_link_libraries(date_unittest myMuduo_base)
add_test(NAME date_unittest COMMAND date_unittest)

add_executable(exception_test Exception_test.cpp)
target_link_libraries(exception_test myMuduo_base)
add_test(NAME exception_test COMMAND exception_test)

add_executable(fileutil_test FileUtil_test.cpp)
target_link_libraries(fileutil_test myMuduo_base)
add_test(NAME fileutil_test COMMAND fileutil_test)

add_executable(fork_test Fork_test.cpp)
target_link_libraries(fork_test myMuduo_base)

if(ZLIB_FOUND)
  add_executable(gzipfile_test GzipFile_test.cpp)
  target_link_libraries(gzipfile_test myMuduo_base z)
  add_test(NAME gzipfile_test COMMAND gzipfile_test)
endif()

add_executable(logfile_test LogFile_test.cpp)
target_link_libraries(logfile_test myMuduo_base)

add_executable(Logger_test Logger_test.cpp)
target_link_libraries(Logger_test myMuduo_base)

add_executable(logstream_bench LogStream_bench.cpp)
target_link_libraries(logstream_bench myMuduo_base)

if(BOOSTTEST_LIBRARY)
add_executable(logstream_test LogStream_test.cpp)
target_link_libraries(logstream_test myMuduo_base boost_unit_test_framework)
add_test(NAME logstream_test COMMAND logstream_test)
endif()

add_executable(mutex_test Mutex_test.cpp)
target_link_libraries(mutex_test myMuduo_base)

add_executable(processinfo_test ProcessInfo_test.cpp)
target_link_libraries(processinfo_test myMuduo_base)

add_executable(singleton_test Singleton_test.cpp)
target_link_libraries(singleton_test myMuduo_base)

add_executable(singleton_threadlocal_test SingletonThreadLocal_test.cpp)
target_link_libraries(singleton_threadlocal_test myMuduo_base)

add_executable(thread_bench Thread_bench.cpp)
target_link_libraries(thread_bench myMuduo_base)

add_executable(thread_test Thread_test.cpp)
target_link_libraries(thread_test myMuduo_base)

add_executable(threadlocal_test ThreadLocal_test.cpp)
target_link_libraries(threadlocal_test myMuduo_base)

add_executable(threadlocalsingleton_test ThreadLocalSingleton_test.cpp)
target_link_libraries(threadlocalsingleton_test myMuduo_base)

add_executable(threadpool_test ThreadPool_test.cpp)
target_link_libraries(threadpool_test myMuduo_base)

add_executable(timestamp_unittest Timestamp_unittest.cpp)
target_link_libraries(timestamp_unittest myMuduo_base)
add_test(NAME timestamp_unittest COMMAND timestamp_unittest)

add_executable(timezone_unittest TimeZone_unittest.cpp)
target_link_libraries(timezone_unittest myMuduo_base)
add_test(NAME timezone_unittest COMMAND timezone_unittest)

