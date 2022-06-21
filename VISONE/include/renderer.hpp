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

#ifndef _H_VISONE_RENDERER_H
#define _H_VISONE_RENDERER_H

#include <cstdlib>
#include <cstdio>

#include <vector>
#include <string>

#include <mpl/stdio.hpp>

#include "sun.hpp"
#include "observatory.hpp"
#include "satellite.hpp"

//****************************************************************************/
// namespace visone
//****************************************************************************/
namespace visone {

  //****************************************************************************/
  // renderer
  //****************************************************************************/
  class renderer : public ogl::glWindow {
    
  private:
  
    // Fov della camera
    GLfloat fov;
    
    std::string name;
    
  public:
    
    //****************************************************************************/
    // renderer
    //****************************************************************************/
    renderer(GLint _width = 800, GLint _height = 600 , GLfloat _fov = 45, glm::vec3 color = glm::vec3(0.1f)) : fov(_fov) {
            
      static int counter = 0;
      
      // Creo il titolo della finestra
      char title[1024] = "renderer";
      if(counter++ != 0) sprintf(title, "renderer %d", counter);

      name = title;
      
      // Creo la finestra
      //ogl::glWindow::create(_width, _height, title);
      ogl::glWindow::createOffscreen(width, height);
      
      // decido il colore dello sfondo
      setBackgournd(color);
      
      DEBUG_LOG("visone::renderer(" + name + ") creation on windowID " + std::to_string(id));
      
    }
    
    //****************************************************************************/
    // rendering
    //****************************************************************************/
    int rendering(const visone::sun & sun, const visone::observatory & observatory, visone::satellite & satellite, bool withExtra = false) {
      
      std::vector<double> lightcurve;
      
      return _rendering(sun, observatory, satellite, lightcurve, "", false, false, true, withExtra);
      
    }
    
    //****************************************************************************/
    // rendering
    //****************************************************************************/
    int rendering(const visone::sun & sun, const visone::observatory & observatory, visone::satellite & satellite, std::vector<double> & lightcurve, bool withExtra = false) {
            
      return _rendering(sun, observatory, satellite, lightcurve, "", true, false, false, withExtra);
      
    }
    
    //****************************************************************************/
    // rendering
    //****************************************************************************/
    int rendering(const visone::sun & sun, const visone::observatory & observatory, visone::satellite & satellite, std::vector<double> & lightcurve, const std::string & outputPath, bool withExtra = false) {
            
      return _rendering(sun, observatory, satellite, lightcurve, outputPath, true, true, false, withExtra);
      
    }
    
    //****************************************************************************/
    // debug
    //****************************************************************************/
    int debug(const visone::sun & sun, const visone::observatory & observatory, visone::satellite & satellite, std::vector<double> & lightcurve, const std::string & outputPath, bool withLightcurve, bool withImages, bool looping, bool withExtra) {
            
     return _rendering(sun, observatory, satellite, lightcurve, outputPath, withLightcurve, withImages, looping, withExtra);
      
    }
    
  private:
    
    //****************************************************************************/
    // _rendering
    //****************************************************************************/
    int _rendering(const visone::sun & sun, const visone::observatory & observatory, visone::satellite & satellite, std::vector<double> & lightcurve, std::string outputPath, bool withLightcurve, bool withImages, bool looping, bool withExtra) {
                 
      DEBUG_LOG("visone::renderer(" + name + ") rendering on windowID " + std::to_string(id));

      // zNear e zFar plane di opengl
      float zNear, zFar;
      
      // mi calcolo la distanza minima e massima tra telecamera e satellite
      observatory.getMinMaxDistance(satellite.getOrbit(), zNear, zFar);
      
      // aggiorno i piani in base alla granzezza del satellite
      zNear = zNear - satellite.getRadius();
      zFar  = zFar  + satellite.getRadius();
      
      // Aggiorno le specifiche delle telecamera
      updateCurrentCamera(fov, observatory.getPosition(0), ogl::glCamera::MODE::TARGET, satellite.getPosition(0));
      
      currentCamera->setzNearFar(zNear, zFar);
      
      // Capisco quanti frame devo simulare
      std::size_t simulationFrames = sun.lenght();
      
      // Mi servono per normalizare la curva di luce
      double maxValue = -DBL_MAX;
      double minValue =  DBL_MAX;
      
      // Alloco lo spazio per la curva di luce
      if(withLightcurve) {
        lightcurve.resize(simulationFrames);
      }
      
      // Se devo salvare le immagini
      if(withImages) {
        outputPath += "/frames";
        mpl::io::expandPath(outputPath);
        mpl::io::dirmk(outputPath);
      }

      // Ciclo su tutti i frame
      for(std::size_t frame=0; frame<simulationFrames; ++frame) {
        
        //printf("Frame %lu/%lu\n", frame, simulationFrames);
        
        // Inizializzo il rendering
        renderBegin();
              
        // Aggiorno la camera posizionata dove sta l'osservatorio e che punta al satellite
        currentCamera->setPosition(observatory.getPosition(frame));
        currentCamera->setTarget(satellite.getPosition(frame));
        
        // Disegno il satellite
        satellite.render(currentCamera, sun.getPosition(frame), observatory.getPosition(frame), frame, withExtra);

        // Se serve faccio e salvo l'immagine del frame
        if(withImages) {
          char filename[PATH_MAX];
          sprintf(filename, "%s/frame_%04lu.tif", outputPath.c_str(), frame);
          snapshot(filename);
        }
          
        // Se serve la curva di luce
        if(withLightcurve) {
          
          // Mi prendo il valore dei pixel media per colore saltando il nero
          glm::vec3 values = pixelsValue();
          
          // Porto da colori in scala di grigio (forse non e' sensato)
          lightcurve[frame] = (0.2126 * values.r) + (0.7152 * values.g) + (0.0722 * values.b);

          // Mi prendo il valore massimo e minimo che mi serve per la normalizazione
          if(maxValue < lightcurve[frame]) maxValue = lightcurve[frame];
          if(minValue > lightcurve[frame]) minValue = lightcurve[frame];
        
        }
      
        // Chiudo il rendering
        renderEnd();
        
        // se voglio fare il loop
        if(looping && frame == simulationFrames-1) frame = 0;
        
        if(shouldClose()) {
          destroy();
          return -1;
        }
        
      }
      
      // Se serve la curva di luce la normalizzo
      if(withLightcurve) {
        for(std::size_t frame=0; frame<simulationFrames; ++frame) {
          lightcurve[frame] = (lightcurve[frame]-minValue) / (maxValue-minValue);
        }
      }
      
      return 0;
        
    }
    
  };

} /* namespace visone */

#endif /* _H_VISONE_RENDERER_H */
