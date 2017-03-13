#
#   This file is part of the
#    _                     _  __ 
#   (_) __ ___      _____ | |/ _|
#   | |/ _` \ \ /\ / / _ \| | |_ 
#   | | (_| |\ V  V / (_) | |  _|
#  _/ |\__,_| \_/\_/ \___/|_|_|  
# |__/                           
# 
#   project : https://github.com/jacmoe/jawolf
#
#   Copyright 2016 Jacob Moen
#
#
cmake_minimum_required(VERSION 2.8.12)
project(neogardens)

#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

include(CMakeToolsHelpers OPTIONAL)


# Dependencies
# ------------

find_package(OpenGL REQUIRED)

# Paths
# -----

set(COMMON_INC "../common/include")

# libs
# -----------------

add_subdirectory(1pal)
#add_subdirectory()
#add_subdirectory()
