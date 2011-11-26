set(HDRS 
      )
 
set(SRCS
      main.cpp
      messageQueueLocked.cpp
      threadRunnerSimple.cpp
      screenCapturerImpl.cpp
      screenRecieverX264.cpp
      imageManagerComplex.cpp
      clockMono.cpp
      screenDumperX264.cpp
      signalHandlerImpl.cpp
)
 
add_executable(Test ${HDRS} ${SRCS})
target_link_libraries(Test ${GTK3_LIBRARIES} ${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${AVCODEC_LIBRARY} ${Boost_LIBRARIES})


