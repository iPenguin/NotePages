set(np_nix  )

add_executable(${EXE_NAME} ${np_srcs} ${np_ui_h} ${np_moc_srcs} ${np_rcc_srcs}
            ${version_file} ${np_nix})
            
target_link_libraries(${EXE_NAME} ${QT_LIBRARIES})

install(TARGETS ${EXE_NAME} BUNDLE DESTINATION . COMPONENT Runtime RUNTIME DESTINATION bin COMPONENT Runtime)