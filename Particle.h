#include "Arduino.h"

/*
 * The particle exists inside the game space between 0 and 1000
 * 
 * However, to do good physics with integers, we need more resolution than that,
 * so the internal physics (x, vx, gravity, friction) are done in a 10x larger space
 */


#define PARTICLE_GRAVITY 2
#define PARTICLE_FRICTION 0.99
#define PARTICLE_DEATH_RATE 1

class Particle
{
  public:
    void Spawn(int pos);
    void Tick(int USE_GRAVITY);
    void Kill();
    bool Alive();
    int _pos;
    uint8_t _hue;
  private:
    void ClampValues();
    int _life;
    int _alive;
    
    int _x;  // position inside 10x larger space
    int _vx; // speed inside 10x larger space
};

void Particle::Spawn(int pos){
  _pos = pos;
  _x = pos * 10;
  _vx = random(-250, 250);
  
  _alive = 1;
  _life = 40 + (abs(_vx) / 6); // Fastest live longest

  _hue = random(256);
}

void Particle::Tick(int USE_GRAVITY){
  if(!_alive) return;
  _life -= PARTICLE_DEATH_RATE;
  _alive = _life > 0;
  

  if(USE_GRAVITY) 
    _vx -= PARTICLE_GRAVITY;
  _vx *= PARTICLE_FRICTION;
  _x += _vx;
  _pos = _x / 10;

  ClampValues();
}

// Tricky, remember position is in game space but _x and _vx are in physics space
void Particle::ClampValues() {
  if(_pos < 0) {
    _pos = 0;
    _x = 0;
    _vx *= -0.5;
  }

  if(_pos > 1000) { // 1000 is magic number for max world size
    _pos = 1000;
    _x = _pos * 10;
    _vx *= -0.5;
  }
}

bool Particle::Alive(){
  return _alive;
}

void Particle::Kill(){
  _alive = 0;
}


