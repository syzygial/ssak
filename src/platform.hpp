#ifndef SSAK_PLATFORM_HPP
#define SSAK_PLATFORM_HPP

namespace ssak {
class Platform {
  public:
  virtual void wait_for_process() const = 0;
};

class Linux : public Platform {
  void wait_for_process() const override {}
};

} // namespace ssak
#endif