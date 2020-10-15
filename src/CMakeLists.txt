cmake_minimum_required(VERSION 3.10)

#find_package(nlohmann_json 3.2.0 REQUIRED)

# set the project name
project(TechQuest VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(TechQuest TechQuest.cpp server.cpp TQDataModels.cpp TQGameData.cpp TQCommandLine.cpp TQTools.cpp TQTools.h)

configure_file(TechQuestConfig.h.in TechQuestConfig.h)

target_include_directories(TechQuest PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
add_subdirectory( lib/json )
target_link_libraries( TechQuest PRIVATE nlohmann_json )
