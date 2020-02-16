cmake_minimum_required (VERSION 3.1)
project(tsMuxerGUI)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.14.1/")
set(Qt5_DIR "/usr/local/Cellar/qt/5.14.1/")
set(Qt5Widgets_DIR "${Qt5_DIR}/lib/cmake/Qt5Widgets")
set(Qt5Network_DIR "${Qt5_DIR}/lib/cmake/Qt5Network")
set(Qt5LinguistTools_DIR "${Qt5_DIR}/lib/cmake/Qt5LinguistTools")


find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

set(tsmuxer_gui_sources
  main.cpp
  tsmuxerwindow.cpp
  muxForm.cpp
  tsmuxerwindow.ui
  muxForm.ui
  checkboxedheaderview.cpp
  images.qrc
)


add_executable(tsMuxerGUI ${tsmuxer_gui_sources})
target_link_libraries(tsMuxerGUI Qt5::Widgets Qt5::Multimedia)

if (WIN32)
  target_sources(tsMuxerGUI PRIVATE icon.rc)
elseif (APPLE)
  set(MACOSX_BUNDLE_ICON_FILE tsMuxerGUI.icns)
  set_source_files_properties(tsMuxerGUI.icns PROPERTIES
         MACOSX_PACKAGE_LOCATION "Resources")
  set_target_properties(tsMuxerGUI PROPERTIES MACOSX_BUNDLE TRUE)
  target_sources(tsMuxerGUI PRIVATE tsMuxerGUI.icns)
endif()
