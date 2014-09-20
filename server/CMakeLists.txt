cmake_minimum_required(VERSION 2.8)
project(server)

add_executable(hoxserver session.cpp hoxUtil.cpp hoxTypes.cpp hoxTable.cpp hoxSocketAPI.cpp hoxSessionMgr.cpp hoxSession.cpp hoxReferee.cpp hoxPlayer.cpp hoxMove.cpp hoxLog.cpp hoxFileMgr.cpp hoxExcept.cpp hoxDebug.cpp hoxDbClient.cpp main.cpp)

target_link_libraries(hoxserver st config++)

install(TARGETS hoxserver RUNTIME DESTINATION bin)
