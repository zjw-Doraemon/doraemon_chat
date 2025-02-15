cmake_minimum_required(VERSION 3.10)
project(GateServer)
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. DIR_SRCS)
add_executable(GateServer ${DIR_SRCS} )

# json ¿â
target_include_directories(GateServer PRIVATE C:/all_chat_vir/json_c++/include)
#target_link_libraries(GateServer "C:/all_chat_vir/json_c++/lib/json_vc71_libmt.lib")
target_link_libraries(GateServer  PRIVATE "C:/all_chat_vir/json_c++/lib/json_vc71_libmtd.lib")



#boost ¿â 
target_include_directories(GateServer PRIVATE C:/all_chat_vir/boost/include/boost-1_87)

target_link_libraries(GateServer PRIVATE
    C:/all_chat_vir/boost/lib/libboost_system-vc143-mt-gd-x64-1_87.lib
    C:/all_chat_vir/boost/lib/libboost_thread-vc143-mt-gd-x64-1_87.lib
    C:/all_chat_vir/boost/lib/libboost_filesystem-vc143-mt-gd-x64-1_87.lib
)

#grpc ¿â
target_include_directories(GateServer PRIVATE
    C:/all_chat_vir/grpc/third_party/re2
    C:/all_chat_vir/grpc/third_party/address_sorting/include
    C:/all_chat_vir/grpc/third_party/abseil-cpp
    C:/all_chat_vir/grpc/third_party/protobuf/src
    C:/all_chat_vir/grpc/include
)

target_link_directories(GateServer PRIVATE
    C:/all_chat_vir/grpc/visualpro/third_party/re2/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/types/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/synchronization/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/status/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/random/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/flags/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/debugging/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/container/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/hash/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/boringssl-with-bazel/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/numeric/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/time/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/base/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/abseil-cpp/absl/strings/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/protobuf/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/zlib/Debug
    C:/all_chat_vir/grpc/visualpro/Debug
    C:/all_chat_vir/grpc/visualpro/third_party/cares/cares/lib/Debug
)

target_link_libraries(GateServer PRIVATE
    libprotobufd.lib
    gpr.lib
    grpc.lib
    grpc++.lib
    grpc++_reflection.lib
    address_sorting.lib
    ws2_32.lib
    cares.lib
    zlibstaticd.lib
    upb.lib
    ssl.lib
    crypto.lib
    absl_bad_any_cast_impl.lib
    absl_bad_optional_access.lib
    absl_bad_variant_access.lib
    absl_base.lib
    absl_city.lib
    absl_civil_time.lib
    absl_cord.lib
    absl_debugging_internal.lib
    absl_demangle_internal.lib
    absl_examine_stack.lib
    absl_exponential_biased.lib
    absl_failure_signal_handler.lib
    absl_flags.lib
    absl_flags_config.lib
    absl_flags_internal.lib
    absl_flags_marshalling.lib
    absl_flags_parse.lib
    absl_flags_program_name.lib
    absl_flags_usage.lib
    absl_flags_usage_internal.lib
    absl_graphcycles_internal.lib
    absl_hash.lib
    absl_hashtablez_sampler.lib
    absl_int128.lib
    absl_leak_check.lib
    absl_leak_check_disable.lib
    absl_log_severity.lib
    absl_malloc_internal.lib
    absl_periodic_sampler.lib
    absl_random_distributions.lib
    absl_random_internal_distribution_test_util.lib
    absl_random_internal_pool_urbg.lib
    absl_random_internal_randen.lib
    absl_random_internal_randen_hwaes.lib
    absl_random_internal_randen_hwaes_impl.lib
    absl_random_internal_randen_slow.lib
    absl_random_internal_seed_material.lib
    absl_random_seed_gen_exception.lib
    absl_random_seed_sequences.lib
    absl_raw_hash_set.lib
    absl_raw_logging_internal.lib
    absl_scoped_set_env.lib
    absl_spinlock_wait.lib
    absl_stacktrace.lib
    absl_status.lib
    absl_strings.lib
    absl_strings_internal.lib
    absl_str_format_internal.lib
    absl_symbolize.lib
    absl_synchronization.lib
    absl_throw_delegate.lib
    absl_time.lib
    absl_time_zone.lib
    absl_statusor.lib
    re2.lib
)

# hreadis
target_include_directories(GateServer PRIVATE C:/all_chat_vir/redis/deps/hiredis)

target_link_libraries(GateServer PRIVATE 
    C:/all_chat_vir/redis/lib/hiredis.lib 
    C:/all_chat_vir/redis/lib/Win32_Interop.lib
)

#mysql

target_include_directories(GateServer PRIVATE C:/all_chat_vir/mysql_connector/mysql-connector-c++-9.2.0-winx64/include)
target_link_directories(GateServer PRIVATE C:/all_chat_vir/mysql_connector/mysql-connector-c++-9.2.0-winx64/lib64/debug/vs14)
target_link_libraries(GateServer PRIVATE mysqlcppconn.lib  mysqlcppconnx.lib)



# ¿½±´ÎÄ¼þ
file(COPY ${CMAKE_SOURCE_DIR}/config.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/mysqlcppconn-10-vs14.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/mysqlcppconnx-2-vs14.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
