foreach(variable IN ITEMS OBJDUMP ELF OUTPUT)
  if(NOT DEFINED ${variable} OR "${${variable}}" STREQUAL "")
    message(FATAL_ERROR "${variable} is required")
  endif()
endforeach()

execute_process(
  COMMAND "${OBJDUMP}" -h -S "${ELF}"
  OUTPUT_FILE "${OUTPUT}"
  RESULT_VARIABLE result
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "arm-none-eabi-objdump failed with exit code ${result}")
endif()
