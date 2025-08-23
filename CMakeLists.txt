cmake_minimum_required(VERSION 3.20)
project(VehicleSys LANGUAGES CXX)


find_package(Qt5 REQUIRED COMPONENTS Core Quick Widgets)
find_package(Qt5 QUIET COMPONENTS SerialBus Multimedia)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt5_add_resources(RESOURCES qml.qrc)

add_executable(VehicleSys 
    main.cpp 
    controllers/src/system.cpp
    controllers/headers/system.h
    controllers/src/hvachandler.cpp
    controllers/headers/hvachandler.h
    controllers/src/audiocontroller.cpp
    controllers/headers/audiocontroller.h
    controllers/src/canbuscontroller.cpp
    controllers/headers/canbuscontroller.h
    controllers/src/vehicledatacontroller.cpp
    controllers/headers/vehicledatacontroller.h
    controllers/src/mediacontroller.cpp
    controllers/headers/mediacontroller.h
    ${RESOURCES}
)

target_include_directories(VehicleSys PRIVATE controllers/headers)
target_link_libraries(VehicleSys Qt5::Quick Qt5::Widgets)

# Add SerialBus if available, otherwise define fallback
if(TARGET Qt5::SerialBus)
    target_link_libraries(VehicleSys Qt5::SerialBus)
    target_compile_definitions(VehicleSys PRIVATE HAVE_QT_SERIALBUS)
endif()

# Add Multimedia if available, otherwise define fallback
if(TARGET Qt5::Multimedia)
    target_link_libraries(VehicleSys Qt5::Multimedia)
    target_compile_definitions(VehicleSys PRIVATE HAVE_QT_MULTIMEDIA)
endif()
