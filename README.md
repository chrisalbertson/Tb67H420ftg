# Tb67H420ftg
Arduino library for Pololu/Toshiba tb67h420ftg motor driver breakout board.

Provides the following:
  Tb67h420ftg()
  Tb67h420ftg(byte directA, byte directB)
  void go(byte motor, byte direct, int pwm)
  void clearFault(byte motor)
  void brake(byte motor)
  void coast(byte motor)
  int  getStatus()
  void setInstalledDirection(byte motor, byte direct)


