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

#ifndef _H_VISONE_OBSERVATORY_H_
#define _H_VISONE_OBSERVATORY_H_

#include <cstdio>
#include <cstdlib>

#include <vector>

//****************************************************************************/
// namespace visone
//****************************************************************************/
namespace visone {

  //****************************************************************************/
  // class observatory
  //****************************************************************************/
  class observatory : public renderable {
    
  private:
    
    ogl::glLine   orbitLine;
    ogl::glSphere sphere;
    
    std::vector<glm::vec3> positions;
    
  public:
    
    //****************************************************************************/
    // observatory() -
    //****************************************************************************/
    observatory() : renderable("observatory") { }
    observatory(double latitude, double longitude, double height, double jdStart, double jdEnd, double dt) : renderable("observatory") {
      init(latitude, longitude, height, jdStart, jdEnd, dt);
    }
    
    
    //****************************************************************************/
    // init() -
    //****************************************************************************/
    void init(double latitude, double longitude, double height, double jdStart, double jdEnd, double dt) {
      
      // vettore di appoggio con le posizioni
      std::vector<astro::ObservatoryState> states;
      
      // creo la stazione
      astro::Observatory station(latitude, longitude, height);

      // creo l'obita
      station.orbit(jdStart, jdEnd, dt, states, CRS::ECEF);

      // fattore di normalizazione per portare in ER
      float factor = 6371.0;
      
      // alloco la memoria
      positions.resize(states.size());
      
      // normalizzo e copio
      for(int i=0; i<states.size(); ++i) {
        
        positions[i].x = states[i].position[0] / factor;
        positions[i].y = states[i].position[1] / factor;
        positions[i].z = states[i].position[2] / factor;
        
      }
          
      isInited = true;
      
    }
    
    //****************************************************************************/
    // render() -
    //****************************************************************************/
    void render(const ogl::glCamera * camera, size_t frame, bool withSphere = true, bool withOrbit = true) {
      
      renderable::renderBegin();

      if(withSphere) {
        sphere.translate(positions[frame]);
        sphere.render(camera);
      }
      
      if(withOrbit) orbitLine.render(camera);
      
      renderable::renderEnd();

    }
    
    //****************************************************************************/
    // Position functions
    //****************************************************************************/
    inline glm::vec3 getPosition(std::size_t index) const { return positions[index]; }
    
    //****************************************************************************/
    // lenght() -
    //****************************************************************************/
    inline std::size_t lenght() const { return positions.size(); }
    
    //****************************************************************************/
    // getMinMaxDistance() -
    //****************************************************************************/
    void getMinMaxDistance(const std::vector<glm::vec3> & objPositions, float & min, float & max) const {
      
      if(!isInited){
        fprintf(stderr, "The observatory must be inited\n");
        abort();
      }
      
      if(positions.size() != objPositions.size()){
        fprintf(stderr, "The must be of the same size\n");
        abort();
      }
      
      min =  FLT_MAX;
      max = -FLT_MAX;
      
      for(size_t i=0; i<positions.size(); ++i){
        
        float dist = glm::distance(positions[i], objPositions[i]);
              
        if(dist < min) min = dist;
        if(dist > max) max = dist;
        
      }
            
    }
    
    
  private:
    
    //****************************************************************************/
    // initToRender() -
    //****************************************************************************/
    void initToRender() {
            
      sphere.init(0.01, 32, 32, ogl::glObject::STYLE::SOLID, glm::vec3(0.0f, 1.0f, 0.0f));
      
      orbitLine.init(positions, glm::vec3(0.0f, 1.0f, 0.0f));
      
      isInitedToRender = true;

    }
    
  };

} /* namespace visone */

#endif /* _H_VISONE_OBSERVATORY_H_ */
