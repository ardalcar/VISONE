/*
 * GNU GENERAL PUBLIC LICENSE
 *
 * Copyright (C) 2017
 * Created by Leonardo Parisi (leonardo.parisi[at]gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _H_VISONE_SUN_H_
#define _H_VISONE_SUN_H_

#include <cstdio>
#include <cstdlib>

#include <vector>

//****************************************************************************/
// namespace visone
//****************************************************************************/
namespace visone {

  //****************************************************************************/
  // class sun
  //****************************************************************************/
  class sun : public renderable {
    
  private:
    
    ogl::glLine   orbitLine;
    ogl::glSphere sphere;
    
    std::vector<glm::vec3> positions;
    
  public:
    
    //****************************************************************************/
    // sun() -
    //****************************************************************************/
    sun() : renderable("sun") { }
    sun(double jdStart, double jdEnd, double dt) : renderable("sun") { init(jdStart, jdEnd, dt); }
    
    //****************************************************************************/
    // init() -
    //****************************************************************************/
    void init(double jdStart, double jdEnd, double dt) {

      // vettore di appoggio con le posizioni
      std::vector<astro::SunState> states;
      
      // generero l'orbita
      astro::Sun::orbit(jdStart, jdEnd, dt, states, CRS::ECEF);
      
      // fattore di normalizazione per portare da km a ER
      float factor = 6371.0;
      
      // alloco la memoria
      positions.resize(states.size());
      
      // normalizzo e copio
      for(int i=0; i<states.size(); ++i) {
        
        positions[i].x = states[i].position[0] / factor;
        positions[i].y = states[i].position[1] / factor;
        positions[i].z = states[i].position[2] / factor;
        
        //positions[i] = glm::normalize(positions[i]);
              
      }
      
      isInited = true;
      
    }
      
    //****************************************************************************/
    // render() -
    //****************************************************************************/
    void render(ogl::glCamera * camera, size_t frame, bool withSphere = true, bool withOrbit = true) {
      
      renderable::renderBegin();
      
      if(withSphere) {
        sphere.translate(glm::normalize(positions[frame]) * 10.0f);
        sphere.render(camera);
      }

      if(withOrbit) orbitLine.render(camera);

      renderable::renderEnd();

    }
    
    //****************************************************************************/
    // getPosition() -
    //****************************************************************************/
    inline glm::vec3 getPosition(std::size_t index) const { return positions[index]; }
    
    //****************************************************************************/
    // lenght() -
    //****************************************************************************/
    inline std::size_t lenght() const { return positions.size(); }
    
    
  private:
    
    //****************************************************************************/
    // initToRender() -
    //****************************************************************************/
    void initToRender() {
            
      sphere.init(0.1, 32, 32, ogl::glObject::STYLE::SOLID, glm::vec3(1.0f, 0.8f, 0.35f));
      
      orbitLine.init(positions, glm::vec3(1.0f, 0.8f, 0.35f));
      
      isInitedToRender = true;
      
    }
    
  };

} /* namespace visone */

#endif /* _H_VISONE_SUN_H_ */

