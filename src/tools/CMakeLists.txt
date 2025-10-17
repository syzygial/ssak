add_executable(ssak ssak.cpp ../exp_templates.cpp)
target_link_libraries(ssak PRIVATE sqlite3)
#target_compile_options(ssak PRIVATE
#  $<IF:$<CONFIG:Release>,
#  -Wall
# -Wextra,
#  >
#)
install(TARGETS ssak RUNTIME)