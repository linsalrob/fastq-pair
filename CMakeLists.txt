cmake_minimum_required(VERSION 3.6)
project(fastq_pair)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c robstr.c fastq_pair.c is_gzipped.c is_gzipped.h)
add_executable(fastq_pair ${SOURCE_FILES})
install (TARGETS fastq_pair DESTINATION bin)
