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

#ifndef _H_VISONE_RENDERABLE_H_
#define _H_VISONE_RENDERABLE_H_

#include <cstdio>
#include <cstdlib>

#include <string>

//****************************************************************************/
// namespace visone
//****************************************************************************/
namespace visone {

  //****************************************************************************/
  // renderable
  //****************************************************************************/
  class renderable {
    
    protected:
        
      bool isInited;
      bool isInitedToRender;
      
      std::string name;
      
      renderable(const std::string & _name = "") : isInited(false), isInitedToRender(false), name(_name) { }

      //****************************************************************************/
      // renderBegin
      //****************************************************************************/
      void renderBegin() {
        
        if(!isToInit()){
          fprintf(stderr, "The %s must be inited before render\n", name.c_str());
          abort();
        }

        if(!isInitedToRender) initToRender();
        
        glEnable(GL_DEPTH_TEST);

      }
      
      //****************************************************************************/
      // renderEnd
      //****************************************************************************/
      inline void renderEnd() {
     
        glDisable(GL_DEPTH_TEST);

      }
      
      virtual void initToRender() = 0;
    
      virtual bool isToInit() { return isInited; }
    
  };

} /* namespace visone */


#endif /* _H_VISONE_RENDERABLE_H_ */
