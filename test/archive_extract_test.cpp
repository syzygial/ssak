#include <iostream>

#include "../src/archive.hpp"

int main() {
  char archive_dat[139];
  FILE *f = fopen("test.tar.gz", "rb");
  fread(archive_dat, 1, 139, f);
  fclose(f);
  //ssak::archive_itr itr((void*)archive_dat, 139);
  //ssak::archive_itr itr("test.tar.gz");
  ssak::extract_archive(archive_dat, 139, "extract_out");
}