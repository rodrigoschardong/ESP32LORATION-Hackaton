# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v4.4.2/components/bootloader/subproject"
  "C:/Users/samur/Documents/hackaton/ESP32LORATION-Hackaton/build/bootloader"
  "C:/Users/samur/Documents/hackaton/ESP32LORATION-Hackaton/build/bootloader-prefix"
  "C:/Users/samur/Documents/hackaton/ESP32LORATION-Hackaton/build/bootloader-prefix/tmp"
  "C:/Users/samur/Documents/hackaton/ESP32LORATION-Hackaton/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/samur/Documents/hackaton/ESP32LORATION-Hackaton/build/bootloader-prefix/src"
  "C:/Users/samur/Documents/hackaton/ESP32LORATION-Hackaton/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/samur/Documents/hackaton/ESP32LORATION-Hackaton/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
