
if (UNIX)
    add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif (UNIX)

include_directories("${CMAKE_SOURCE_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_executable(shuJudge main.cpp Submission.cpp util.cpp Test.cpp Handler.cpp)
set(Casablanca_LIBRARIES "-lboost_filesystem -lboost_system -lcrypto -lssl -lcpprest")
target_link_libraries(shuJudge ${Casablanca_LIBRARIES})

