cmake_minimum_required(VERSION 3.20)

project(stm32f103_robot_car LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3" CACHE STRING "Debug C flags" FORCE)
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE STRING "Debug assembly flags" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG" CACHE STRING "Release C flags" FORCE)
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE STRING "Release assembly flags" FORCE)

file(GLOB FIRMWARE_CORE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/firmware/Core/Src/*.c"
)
file(GLOB FIRMWARE_HAL_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/firmware/Drivers/STM32F1xx_HAL_Driver/Src/*.c"
)

add_executable(car_firmware
  ${FIRMWARE_CORE_SOURCES}
  ${FIRMWARE_HAL_SOURCES}
  firmware/Core/Startup/startup_stm32f103c8tx.s
)

set_target_properties(car_firmware PROPERTIES
  OUTPUT_NAME "car-1"
  SUFFIX ".elf"
)

target_include_directories(car_firmware PRIVATE
  firmware/Core/Inc
  firmware/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
  firmware/Drivers/STM32F1xx_HAL_Driver/Inc
  firmware/Drivers/CMSIS/Device/ST/STM32F1xx/Include
  firmware/Drivers/CMSIS/Include
)

target_compile_definitions(car_firmware PRIVATE
  USE_HAL_DRIVER
  STM32F103xB
  $<$<CONFIG:Debug>:DEBUG>
)

target_compile_options(car_firmware PRIVATE
  -mcpu=cortex-m3
  -mthumb
  -mfloat-abi=soft
  --specs=nano.specs
  $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
  $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
  $<$<COMPILE_LANGUAGE:C>:-Wall>
  "-ffile-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}=."
)

set(FIRMWARE_MAP "${CMAKE_CURRENT_BINARY_DIR}/car-1.map")
target_link_options(car_firmware PRIVATE
  -mcpu=cortex-m3
  -mthumb
  -mfloat-abi=soft
  "-T${CMAKE_CURRENT_SOURCE_DIR}/firmware/STM32F103C8TX_FLASH.ld"
  --specs=nosys.specs
  --specs=nano.specs
  -static
  -Wl,--gc-sections
  -Wl,--print-memory-usage
  "-Wl,-Map=${FIRMWARE_MAP}"
  -u
  _printf_float
)
target_link_libraries(car_firmware PRIVATE
  -Wl,--start-group
  c
  m
  -Wl,--end-group
)

set(FIRMWARE_HEX "${CMAKE_CURRENT_BINARY_DIR}/car-1.hex")
set(FIRMWARE_BIN "${CMAKE_CURRENT_BINARY_DIR}/car-1.bin")
set(FIRMWARE_LIST "${CMAKE_CURRENT_BINARY_DIR}/car-1.list")

add_custom_command(TARGET car_firmware POST_BUILD
  COMMAND "${ARM_NONE_EABI_OBJCOPY}" -O ihex "$<TARGET_FILE:car_firmware>" "${FIRMWARE_HEX}"
  COMMAND "${ARM_NONE_EABI_OBJCOPY}" -O binary "$<TARGET_FILE:car_firmware>" "${FIRMWARE_BIN}"
  COMMAND "${CMAKE_COMMAND}"
    "-DOBJDUMP=${ARM_NONE_EABI_OBJDUMP}"
    "-DELF=$<TARGET_FILE:car_firmware>"
    "-DOUTPUT=${FIRMWARE_LIST}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate-disassembly.cmake"
  COMMAND "${ARM_NONE_EABI_SIZE}" "$<TARGET_FILE:car_firmware>"
  BYPRODUCTS "${FIRMWARE_HEX}" "${FIRMWARE_BIN}" "${FIRMWARE_LIST}" "${FIRMWARE_MAP}"
  COMMENT "Generating flashable firmware artifacts"
  VERBATIM
)
