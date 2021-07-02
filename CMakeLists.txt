# Generated from codeeditor.pro.

cmake_minimum_required(VERSION 3.14)
project(WindowBuilder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/widgets/codeeditor")


set(${PROJECT_NAME}_UI "" CACHE INTERNAL "list of all ui files to include in project" FORCE)
list(APPEND ${PROJECT_NAME}_UI
    ui/mainwindow.ui
    )

set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets)
unset(REQUIRED_LIBS_QUALIFIED)

foreach (QT_LIB ${REQUIRED_LIBS})
    list(APPEND REQUIRED_LIBS_QUALIFIED Qt${QT_VERSION}::${QT_LIB})
endforeach()


# Include Qt libraries
find_package(Qt${QT_VERSION} COMPONENTS
  ${REQUIRED_LIBS}
  REQUIRED
  )

qt_wrap_ui(${${PROJECT_NAME}_UI} ${${PROJECT_NAME}_UI})

qt_add_executable(${PROJECT_NAME}
    sources/codeeditor.cpp
    includes/codeeditor.h
    sources/mainwindow.cpp
    includes/mainwindow.h
    sources/rpgNoteHighlighter.cpp
    includes/rpgNoteHighlighter.h
    ${${PROJECT_NAME}_UI}
    main.cpp
)


target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_BINARY_DIR}
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${REQUIRED_LIBS_QUALIFIED}
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

