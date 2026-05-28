function(cisastor_add_test)
    cmake_parse_arguments(ARG
        ""
        "NAME"
        "SRCS;IMPLS;LIBS"
        ${ARGV}
    )

    add_executable(${ARG_NAME}
        ${ARG_SRCS}
        ${ARG_IMPLS}
        $<TARGET_OBJECTS:cisastor_testing_entry>
    )

    target_link_libraries(${ARG_NAME}
        PRIVATE
        cisastor_testing
        ${ARG_LIBS}
    )

    add_test(
        NAME ${ARG_NAME}
        COMMAND ${ARG_NAME}
    )
endfunction()
