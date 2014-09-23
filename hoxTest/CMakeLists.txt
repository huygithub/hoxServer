cmake_minimum_required(VERSION 2.8)
project(hoxtest)

include_directories(../plugins/common)

set(COMMON_HEADER_FILES ../plugins/common/AIEngineLib.h ../plugins/common/DefaultDelete.h)

add_executable(hoxtest
  src/TestPlayer.cpp
  src/session.cpp
  src/Player.cpp
  src/Manager.cpp
  src/hoxSocketAPI.cpp
  src/hoxLog.cpp
  src/hoxExcept.cpp
  src/hoxDebug.cpp
  src/hoxCommon.cpp
  src/hoxCommand.cpp
  src/AIPlayer.cpp
  src/main.cpp
  ${COMMON_HEADER_FILES}
)

target_link_libraries(hoxtest st config++ dl)
