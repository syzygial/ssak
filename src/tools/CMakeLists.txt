add_executable(ssak ssak.cpp ../exp_templates.cpp ../archive.cpp)
target_link_libraries(ssak PRIVATE sqlite3)
target_link_libraries(ssak PRIVATE archive)
#target_compile_options(ssak PRIVATE
#  $<IF:$<CONFIG:Release>,
#  -Wall
# -Wextra,
#  >
#)
install(TARGETS ssak RUNTIME)