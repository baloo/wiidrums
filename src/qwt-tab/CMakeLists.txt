include(${QT_USE_FILE})

FIND_PACKAGE(Qwt5 REQUIRED)



set(QWTTAB_SRCS
    tabdialog
    plot
    main
    )

set(QWTTAB_HDRS
    tabdialog.h
    plot.h
    )


qt4_automoc(${qtproject_SRCS})

QT4_WRAP_CPP(QWTTAB_CORE_MOC_SRCS 
    ${QWTTAB_HDRS})



INCLUDE_DIRECTORIES(
    ${Qwt5_INCLUDE_DIR}
    )

LINK_LIBRARIES(
    ${QT_QTGUI_LIBRARY}
    ${QT_LIBRARIES}
    ${Qwt5_Qt4_LIBRARY}
    )



add_executable(
    qwt-tab
    ${QWTTAB_SRCS} 
    ${QWTTAB_CORE_MOC_SRCS})


