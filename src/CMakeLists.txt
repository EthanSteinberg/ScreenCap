set(HDRS 
      )
 
set(SRCS
      main.cpp
      messageQueueLocked.cpp
      threadRunnerSimple.cpp
      screenCapturerImpl.cpp
      screenRecieverPng.cpp
      imageManagerSimple.cpp
)
 
add_executable(Test ${HDRS} ${SRCS})
target_link_libraries(Test ${GTK3_LIBRARIES} ${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${AVCODEC_LIBRARY} ${Boost_LIBRARIES})


