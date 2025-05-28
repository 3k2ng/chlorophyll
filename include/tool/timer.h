#pragma once

class Timer {
  public:
    Timer(const float &cooldown, const bool &reset = true)
        : cooldown_{cooldown}, timer_{reset ? cooldown : 0} {}

    void update(const float delta) { timer_ -= delta; }
    void reset() { timer_ = cooldown_; }
    bool finished() const { return timer_ <= 0; }

  private:
    float cooldown_, timer_;
};
