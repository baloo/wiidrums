# vim: ts=4 sw=4 expandtab:

INCLUDE(${QT_USE_FILE})

FIND_PACKAGE(Qwt5 REQUIRED)
FIND_PACKAGE(Wiiuse REQUIRED)
FIND_PACKAGE(Phonon REQUIRED)



SET(WIIDRUMS_SRCS
    wiidrums
    wiimote/wiimotemanager
    wiimote/wiimote
    wiimote/wiimotes
    audio/audioobject
    audio/genericaudioobject
    gui/simplegui
    gui/wiimoteview
    misc/plotlist
    )

SET(WIIDRUMS_HDRS
    wiimote/wiimote.h
    wiimote/wiimotes.h
    wiimote/wiimotemanager.h
    audio/genericaudioobject.h
    audio/audioobject.h
    gui/simplegui.h
    gui/wiimoteview.h
    misc/plotlist.h
    )


qt4_automoc(${qtproject_SRCS})

QT4_WRAP_CPP(WIIDRUMS_CORE_MOC_SRCS 
    ${WIIDRUMS_HDRS})


INCLUDE_DIRECTORIES(
    .
    wiimote
    ${Qwt5_INCLUDE_DIR}
    ${Wiiuse_INCLUDE_DIR}
    ${PHONON_INCLUDES}
    )

LINK_LIBRARIES(
    ${PHONON_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_LIBRARIES}
    ${Qwt5_Qt4_LIBRARY}
    ${Wiiuse_LIBRARY}
    )



add_executable(
    wiidrums
    ${WIIDRUMS_SRCS} 
    ${WIIDRUMS_CORE_MOC_SRCS})


