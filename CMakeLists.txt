# Generated from codeeditor.pro.

cmake_minimum_required(VERSION 3.14)
project(WindowBuilder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/widgets/codeeditor")


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


qt_add_executable(${PROJECT_NAME}
    sources/codeeditor.cpp
    includes/codeeditor.h
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    includes
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
