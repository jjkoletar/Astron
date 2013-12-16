# Filename: CompositeSources.cmake
#
# Looks at all the sources and generates _compositeN.cpp and modifies the list.
#
# Usage:
#   composite_sources(target sources_var)
#
# Example:
#   set(FOO_SOURCES a.cpp b.cpp c.cpp)
#   composite_sources(foo FOO_SOURCES)
#   add_executable(foo ${FOO_SOURCES})
#
# Configuration Variables
#    COMPOSITE_SOURCE_LIMIT      - Setting this to a value >= 1 will enable unity builds.
#                                  A high value will speed up the build but use more RAM.
#                                  If the variable is not defined, the default is 10.
#    COMPOSITE_SOURCE_EXTENSIONS - Only files of these extensions will be added to composite files.
#                                  Defaults to c;cpp;cxx
#    COMPOSITE_OUTPUT_EXTENSION  - Output composite files with this extension. Default: cpp;
#    COMPOSITE_GENERATOR         - The filepath to the MakeComposite.cmake script.
#                                  Default: ${CMAKE_SOURCE_DIR}/cmake/scripts/MakeComposite.cmake
#


set(COMPOSITE_SOURCE_EXTENSIONS "cpp;c" CACHE STRING
  "Only files of these extensions will be added to composite files.")



function(composite_sources target sources_var)
  if(NOT DEFINED COMPOSITE_SOURCE_LIMIT)
    set(COMPOSITE_SOURCE_LIMIT "10")
  endif()

  if(NOT DEFINED COMPOSITE_SOURCE_EXTENSIONS)
    set(COMPOSITE_SOURCE_EXTENSIONS "c;cpp;cxx")
  endif()

  if(NOT DEFINED COMPOSITE_OUTPUT_EXTENSION)
  endif()

  if(NOT DEFINED COMPOSITE_GENERATOR)
    set(COMPOSITE_GENERATOR "${CMAKE_SOURCE_DIR}/cmake/scripts/MakeComposite.cmake")
  endif()

  # How many sources were specified?
  set(orig_sources ${${sources_var}})
  set(sources ${orig_sources})
  list(LENGTH sources num_sources)

  if(num_sources LESS 2 OR ${COMPOSITE_SOURCE_LIMIT} LESS 2)
    # It's silly to do this for a single source.
    return()
  endif()

  set(composite_files "")
  set(composite_sources "")

  while(num_sources GREATER 0)
    # Pop the first element
    list(GET sources 0 source)
    list(REMOVE_AT sources 0)
    list(LENGTH sources num_sources)

    # Check if we can safely add this to a composite file.
    get_source_file_property(generated "${source}" GENERATED)
    get_source_file_property(is_header "${source}" HEADER_FILE_ONLY)

    if(NOT generated AND NOT is_header)
      # Add it to composite_sources.
      list(APPEND composite_sources ${source})
      list(LENGTH composite_sources num_composite_sources)

      if(num_sources EQUAL 0 OR NOT num_composite_sources LESS ${COMPOSITE_SOURCE_LIMIT})

        # It's pointless to make a composite source from just one file.
        if(num_composite_sources GREATER 1)

          # Figure out the name of our composite file.
          list(LENGTH composite_files index)
          math(EXPR index "1+${index}")
          set(composite_file "${CMAKE_CURRENT_BINARY_DIR}/${target}_composite${index}.${COMPOSITE_OUTPUT_EXTENSION}")
          list(APPEND composite_files "${composite_file}")

          # Set HEADER_FILE_ONLY to prevent it from showing up in the
          # compiler command, but still show up in the IDE environment.
          set_source_files_properties(${composite_sources} PROPERTIES HEADER_FILE_ONLY ON)

          # We'll interrogate the composite files, so exclude the original sources.
          set_source_files_properties(${composite_sources} PROPERTIES WRAP_EXCLUDE YES)

          # Finally, add the target that generates the composite file.
          add_custom_command(
            OUTPUT "${composite_file}"
            COMMAND ${CMAKE_COMMAND}
              -DCOMPOSITE_FILE="${composite_file}"
              -DCOMPOSITE_SOURCES="${composite_sources}"
              -P "${COMPOSITE_GENERATOR}"
            DEPENDS ${composite_sources})          

          # Reset for the next composite file.
          set(composite_sources "")
        endif()
      endif()
    endif()
  endwhile()