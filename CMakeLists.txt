project(meetup_raffle)
cmake_minimum_required(VERSION 3.8)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(libs/beast) 
 
add_executable(meetup_raffle main.cpp)
target_include_directories(meetup_raffle PUBLIC 
						libs 
						libs/beast/include
						${BOOST_BEAST_INCLUDES}
						${OPENSSL_INCLUDE_DIR})

target_link_libraries(meetup_raffle ${Boost_LIBRARIES} 
									${OPENSSL_LIBRARIES})

if(UNIX OR CMAKE_COMPILER_IS_GNUCXX)
   add_definitions("-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
else(MSVC)
   add_definitions("/W4 /WX")
   # add_definitions(/D_CRT_SECURE_NO_WARNINGS)
   # add_definitions(/D_SCL_SECURE_NO_WARNINGS)
   add_definitions(/DNOMINMAX)
endif() 
