cmake_minimum_required(VERSION 2.8.3)
project(robotiq_85_driver)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS rospy roscpp std_srvs robotiq_85_msgs)


catkin_package(
#  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp rospy std_srvs robotiq_85_msgs
)

## Uncomment if the package has a setup.py
catkin_python_setup()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(server src/gripper_services.cpp)
add_dependencies(server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(server
  ${catkin_LIBRARIES}
)


catkin_install_python(PROGRAMS bin/robotiq_85_driver bin/robotiq_85_test 
                      DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
                      
