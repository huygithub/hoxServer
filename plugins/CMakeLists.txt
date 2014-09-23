cmake_minimum_required(VERSION 2.8)
project(plugins)

include_directories(common)

set(COMMON_HEADER_FILES common/AIEngineLib.h common/DefaultDelete.h)

add_library(AI_Folium SHARED
  AI_Folium/AI_Folium.cpp
  AI_Folium/engine_data.cpp
  AI_Folium/folHOXEngine.cpp
  AI_Folium/generator.cpp
  AI_Folium/history_data.cpp
  AI_Folium/xq.cpp
  AI_Folium/bitmap_data.cpp
  AI_Folium/folEngine.cpp
  AI_Folium/folSearch.cpp
  AI_Folium/hash.cpp
  AI_Folium/move.cpp
  AI_Folium/xq_data.cpp
  ${COMMON_HEADER_FILES}
)

add_library(AI_HaQiKiD SHARED
  AI_HaQiKiD/haqikidHOX.cpp
  AI_HaQiKiD/AI_HaQiKiD.cpp
  ${COMMON_HEADER_FILES}
)

add_library(AI_MaxQi SHARED
  AI_MaxQi/AI_MaxQi.cpp
  AI_MaxQi/MaxQi.cpp
  ${COMMON_HEADER_FILES}
)

add_library(AI_XQWLight SHARED
  AI_XQWLight/AI_XQWLight.cpp
  AI_XQWLight/XQWLight.cpp
  ${COMMON_HEADER_FILES}
)

add_library(AI_TSITO SHARED
  AI_TSITO/AI_TSITO.cpp
  AI_TSITO/Evaluator.cpp
  AI_TSITO/Move.cpp
  AI_TSITO/Options.cpp
  AI_TSITO/Transposition.cpp
  AI_TSITO/Board.cpp
  AI_TSITO/Lawyer.cpp
  AI_TSITO/OpeningBook.cpp
  AI_TSITO/Timer.cpp
  AI_TSITO/tsiEngine.cpp
  ${COMMON_HEADER_FILES}
)

