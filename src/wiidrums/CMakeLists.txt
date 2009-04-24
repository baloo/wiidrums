# vim: ts=4 sw=4 expandtab:

INCLUDE(${QT_USE_FILE})

FIND_PACKAGE(Qwt5 REQUIRED)
FIND_PACKAGE(Wiiuse REQUIRED)



SET(WIIDRUMS_SRCS
    wiidrums
    wiimotemanager
    wiimote
    wiimotes
    gui/simplegui
    gui/wiimoteview
    )

SET(WIIDRUMS_HDRS
    wiimote.h
    wiimotes.h
    wiimotemanager.h
    gui/simplegui.h
    gui/wiimoteview.h
    )


qt4_automoc(${qtproject_SRCS})

QT4_WRAP_CPP(WIIDRUMS_CORE_MOC_SRCS 
    ${WIIDRUMS_HDRS})



INCLUDE_DIRECTORIES(
    .
    ${Qwt5_INCLUDE_DIR}
    ${Wiiuse_INCLUDE_DIR}
    )

LINK_LIBRARIES(
    ${QT_QTGUI_LIBRARY}
    ${QT_LIBRARIES}
    ${Qwt5_Qt4_LIBRARY}
    ${Wiiuse_LIBRARY}
    )



add_executable(
    wiidrums
    ${WIIDRUMS_SRCS} 
    ${WIIDRUMS_CORE_MOC_SRCS})


