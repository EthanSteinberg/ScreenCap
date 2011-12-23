set(HDRS 
      )
 
set(SRCS
      main.cpp
      configurationManagerImpl.cpp
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
target_link_libraries(Test ${AVCODEC_LIBRARY} ${Boost_LIBRARIES})


