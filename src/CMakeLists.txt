find_package(Boost REQUIRED COMPONENTS filesystem)

add_library(submission SHARED Submission.cpp util.cpp Test.cpp)


if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_executable(shuJudge main.cpp Submission.cpp util.cpp Test.cpp)
    target_link_libraries(shuJudge LINK_PUBLIC ${Boost_LIBRARIES})
else ()
    message("Boost not Find")
endif ()



