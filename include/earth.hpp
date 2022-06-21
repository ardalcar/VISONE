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

#ifndef _H_VISONE_EARTH_H_
#define _H_VISONE_EARTH_H_

#include <cstdio>
#include <cstdlib>

//****************************************************************************/
// namespace visone
//****************************************************************************/
namespace visone {

  //****************************************************************************/
  // class earth
  //****************************************************************************/
  class earth : public renderable {
    
  private:
    
    ogl::glSphere sphere;
    ogl::glSphere center;
    
    ogl::glAxes axes;
    
  public:
    
    //****************************************************************************/
    // earth() -
    //****************************************************************************/
    earth() : renderable("earth") { isInited = true; }

    //****************************************************************************/
    // render() -
    //****************************************************************************/
    void render(const ogl::glCamera * camera) {

      renderable::renderBegin();
      
      sphere.render(camera);
      
      center.render(camera);
      
      axes.render(camera);
      
      renderable::renderEnd();

    }
    
  private:
    
    //****************************************************************************/
    // initToRender
    //****************************************************************************/
    void initToRender() {
            
      sphere.init(1.0, 32, 32, ogl::glObject::STYLE::WIREFRAME);
      
      center.init(0.01, 12, 12, ogl::glObject::STYLE::SOLID, glm::vec3(1.0,0.0,0.0));

      axes.init();
      
      isInitedToRender = true;
      
    }
    
  };

} /* namespace visone */

#endif /* _H_VISONE_EARTH_H_ */


