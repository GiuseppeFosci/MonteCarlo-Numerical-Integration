cmake_minimum_required(VERSION 3.16)
project(mc_integrator_qt_quick VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Trova le librerie necessarie ---
# AGGIUNTO: "Concurrent" alla lista dei componenti
find_package(Qt6 COMPONENTS Quick Qml Charts Widgets Concurrent REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(MUPARSER REQUIRED muparser)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(mc_app_qt
    include/IntegrationController.h
    include/MonteCarloIntegrator.h
    include/PlainMonteCarlo.h
    include/StratifiedMonteCarlo.h
    include/Result.h
    include/Function.h
    src/main.cpp
    src/IntegrationController.cpp
    src/PlainMonteCarlo.cpp
    src/StratifiedMonteCarlo.cpp
    resources.qrc
)

target_include_directories(mc_app_qt PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${MUPARSER_INCLUDE_DIRS}
)

# --- Collegamento delle librerie ---
target_link_libraries(mc_app_qt PUBLIC
    Qt6::Quick
    Qt6::Qml
    Qt6::Charts
    Qt6::Widgets
    Qt6::Concurrent # AGGIUNTO: Link alla libreria Concurrent
    ${MUPARSER_LIBRARIES}
)