struct Timer
{
  Timer(float _duration);

private:
  static void do_tick(void* _userdata);

  float m_duration;

  virtual void tick();
};

