namespace ssak {
  void create_archive(const char *dirname, const char *archive_name);
  void* create_archive(const char *dirname, size_t *archive_sz) // in memory
}