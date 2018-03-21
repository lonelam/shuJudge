
add_library(submission SHARED Submission.cpp util.cpp Test.cpp)
add_executable(shuJudge main.cpp Submission.cpp util.cpp Test.cpp)