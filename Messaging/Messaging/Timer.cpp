#include "Timer.h"

#include <FL/Fl.H>

Timer::Timer(float _duration)
  : m_duration(_duration)
{
  Fl::add_timeout(_duration, do_tick, this);
}

void Timer::tick() { }

void Timer::do_tick(void* _userdata)
{
  Timer* self = (Timer*)_userdata;
  Fl::repeat_timeout(self->m_duration, do_tick, _userdata);
  self->tick();
}

