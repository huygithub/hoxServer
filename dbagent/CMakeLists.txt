cmake_minimum_required(VERSION 2.8)
project(dbagent)

add_executable(dbagent hoxUtil.cpp hoxTypes.cpp hoxSocketAPI.cpp hoxLog.cpp hoxExcept.cpp hoxDebug.cpp hoxDBAPI.cpp main.cpp)

target_link_libraries(dbagent pthread sqlite3)

install(TARGETS dbagent RUNTIME DESTINATION bin)
