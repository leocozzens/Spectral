cmake_minimum_required(VERSION 3.27.9)

function(DisableSettings SETTINGS)
    foreach(SETTING ${SETTINGS})
        option(${SETTING} OFF)
    endforeach()
endfunction()

function(EnableSettings SETTINGS)
    foreach(SETTING ${SETTINGS})
        option(${SETTING} ON)
    endforeach()
endfunction()