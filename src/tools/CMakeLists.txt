add_executable(ssak ssak.cpp ../exp_templates.cpp)
target_link_libraries(ssak PRIVATE sqlite3)
#add_executable(ssak-archive ssak_archive.cpp)