project(sdktools VERSION 1.0)

foreach(script gz pspgz)
   add_custom_target(tool_${script} ALL
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/gz/${script}.py
   )
   set_property(TARGET tool_${script} PROPERTY EXEC ${CMAKE_CURRENT_SOURCE_DIR}/gz/${script}.py)
endforeach()

foreach(script combine kirk make_ipl make_tm_ipl unpack_ipl)
   add_custom_target(ipltool_${script} ALL
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ipltools/${script}.py
   )
   set_property(TARGET ipltool_${script} PROPERTY EXEC ${CMAKE_CURRENT_SOURCE_DIR}/ipltools/${script}.py)
endforeach()

foreach(script pack)
   add_custom_target(tool_${script} ALL
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pack/${script}.py
   )
   set_property(TARGET tool_${script} PROPERTY EXEC ${CMAKE_CURRENT_SOURCE_DIR}/pack/${script}.py)
endforeach()
