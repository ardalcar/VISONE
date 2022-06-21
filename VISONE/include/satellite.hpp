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

#ifndef _H_VISONE_SATELLITE_H_
#define _H_VISONE_SATELLITE_H_

#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>

//****************************************************************************/
// namespace visone
//****************************************************************************/
namespace visone {

  //****************************************************************************/
  // class satellite
  //****************************************************************************/
  class satellite : public renderable {
    
    private:
      
      // Oggetti di rendering
      ogl::glAxes axes;
      ogl::glLine orbitLine;
      //ogl::glSphere sphere;
      ogl::glModel model;
      //ogl::glLine lineC; // Linea blue con la camera
      ogl::glLine lineS; // Linea gialla con il sole
      ogl::glLine lineR; // Linea Bianca con il centro della terra
      
      // Dati del satellite
      std::vector<glm::vec3> velocities;
      std::vector<glm::vec3> positions;
      std::vector<glm::vec3> angles;
      
      // Variabili per la gestione del TLE
      std::string TLE_line1;
      std::string TLE_line2;
      
      // Variabili per la gestione del pannelli
      bool havePanels;
      float panelAngle;
      std::vector<bool> solarPanelMesh;
      glm::vec3 panelCenter;
      glm::mat4 modelPanels;
      
      // Proprieta' del modello
      double modelRadius;
      
      // Variabili per il controllo del'inizializzazione
      bool isInitedModel;
      bool isInitedTLE;
      bool isInitedOrbit;
      bool isInitedAttitude;
      
    public:
      
      //****************************************************************************/
      // satellite() -
      //****************************************************************************/
      satellite() : renderable("satellite"), isInitedModel(false), isInitedTLE(false), isInitedOrbit(false), isInitedAttitude(false), havePanels(false) {
        
        orbitLine.setName("orbitLine");
        
        lineR.setName("lineR");
        lineS.setName("lineS");
        //lineC.setName("lineC");
        
      }
      
      //****************************************************************************/
      // satellite() -
      //****************************************************************************/
      satellite(const std::string & modelPath, GLfloat modelFactor, const std::string & TLE_line1, const std::string & TLE_line2, double jdStart, double jdEnd, double dt)  : renderable("satellite"), havePanels(false) {
        
        orbitLine.setName("orbitLine");
        
        lineR.setName("lineR");
        lineS.setName("lineS");
        //lineC.setName("lineC");
        
        init(modelPath, modelFactor, TLE_line1, TLE_line2, jdStart, jdEnd, dt);
        
      }
      
      //****************************************************************************/
      // init() -
      //****************************************************************************/
      void init(const std::string & modelPath, GLfloat modelFactor, const std::string & _TLE_line1, const std::string & _TLE_line2, double jdStart, double jdEnd, double dt) {
                
        initTLE(_TLE_line1, _TLE_line2);
        
        initOrbit(jdStart, jdEnd, dt);
        
        initModel(modelPath, modelFactor);
        
        isInited = true;
        
      }
      
      //****************************************************************************/
      // initModel() -
      //****************************************************************************/
      void initModel(const std::string & modelPath, GLfloat modelFactor) {
        
        model.init(modelPath, modelFactor);

        modelRadius = model.getRadius();
        
        //sphere.init(modelRadius, 12, 12, ogl::glObject::STYLE::WIREFRAME);
        
        axes.init(modelRadius);
        
        orbitLine.setColor(glm::vec3(1.0,0.0,0.0));
        
        lineR.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        
        lineS.setColor(glm::vec3(1.0f, 0.8f, 0.35f));
        
        //lineC.setColor(glm::vec3(0.0f, 0.5f, 1.0f));
        
        isInitedModel = true;
        
      }
      
      //****************************************************************************/
      // initTLE() -
      //****************************************************************************/
      void initTLE(const std::string & _TLE_line1, const std::string & _TLE_line2) {
        
        TLE_line1 = _TLE_line1;
        TLE_line2 = _TLE_line2;
        
        isInitedTLE = true;
        
      }
      
      //****************************************************************************/
      // initOrbit
      //****************************************************************************/
      void initOrbit(const std::string & _TLE_line1, const std::string & _TLE_line2, double jdStart, double jdEnd, double dt) {
        
        initTLE(_TLE_line1, _TLE_line2);
        
        _initOrbit(jdStart, jdEnd, dt);
        
      }
      
      //****************************************************************************/
      // initOrbit
      //****************************************************************************/
      void initOrbit(double jdStart, double jdEnd, double dt) {
        
        _initOrbit(jdStart, jdEnd, dt);
        
      }
      
      //****************************************************************************/
      // initAttitude
      //****************************************************************************/
      void initAttitude(const glm::vec3 & omega, const glm::quat & quaternion, const glm::vec3 & inertia, float dt) {
        
        // inizializzo l'assetto di partenza
        astro::attitude_t attitude0(omega.x, omega.y, omega.z, quaternion.w, quaternion.x, quaternion.y, quaternion.z, inertia.x, inertia.y, inertia.z);
        
        std::vector<astro::attitude_t> states;
        
        size_t lenght = positions.size();
        
        astro::attitude::compute(attitude0, lenght, dt, states);
        
        angles.resize(lenght, glm::vec3(0));
        
        // copio
        for(int i=0; i<lenght; ++i) {
          
          astro::quaternion_t q = states[i].q();
          
          // NOTE: glm quat constructur is glm::quat(w,x,y,z)
          angles[i] = glm::eulerAngles(glm::quat(q.q4, q.q1, q.q2, q.q3));
          
        }
        
        isInitedAttitude = true;
        
      }
      
      //****************************************************************************/
      // render() -
      //****************************************************************************/
      void render(ogl::glCamera * camera, const glm::vec3 & lightPosition, const glm::vec3 & cameraPosition, size_t frame, bool withExtra = true) {
        
        renderable::renderBegin();
     
        if(withExtra) {
          
          // Disegno l'orbita
          orbitLine.render(camera);
          
#ifdef DEBUG_VISONE
          glCheckError();
#endif
          
          // disegno la sfera intorno al satellite
          //sphere.translate(positions[frame]);
          //sphere.render(camera);
          
          // Linea Bianca con il centro della terra
          lineR.update({glm::vec3(0.0), positions[frame]});
          lineR.render(camera);
          
#ifdef DEBUG_VISONE
          glCheckError();
#endif
          // Linea gialla con il sole
          lineS.update({lightPosition, positions[frame]});
          lineS.render(camera);
          
#ifdef DEBUG_VISONE
          glCheckError();
#endif
          // Linea blue con la camera
          //lineC.update({cameraPosition, positions[frame]});
          //lineC.render(camera);

#ifdef DEBUG_VISONE
          glCheckError();
#endif
          // Disegno gli assi del satellite
          axes.translate(positions[frame]);
          axes.rotate(angles[frame]);
          axes.render(camera);

#ifdef DEBUG_VISONE
          glCheckError();
#endif
          
        }
        
        // Update della posizione ed assetto del satellite
        model.translate(positions[frame]);
        model.rotate(angles[frame]);
        
        // Aggiorno la posizione del sole
        //FIXME: non so perche non passo la direzione
        model.setLight(lightPosition, lightPosition);
        
        // Disegno il satellite
        modelRender(camera);

#ifdef DEBUG_VISONE
        glCheckError();
#endif
        renderable::renderEnd();
        
      }
      
      //****************************************************************************/
      // Position velocity angles functions
      //****************************************************************************/
      inline glm::vec3 getPosition(std::size_t index) const { return positions[index];  }
      inline glm::vec3 getVelocity(std::size_t index) const { return velocities[index]; }
      inline glm::quat getAngles(std::size_t index)   const { return angles[index];     }
      
      //****************************************************************************/
      // getOrbit
      //****************************************************************************/
      inline const std::vector<glm::vec3> & getOrbit() const { return positions; }
      
      //****************************************************************************/
      // getRadius
      //****************************************************************************/
      inline float getRadius() const { return modelRadius; }
      
      //****************************************************************************/
      // orbitLenght() -
      //****************************************************************************/
      inline std::size_t lenght() const { return positions.size(); }
      
      //****************************************************************************/
      // Solar Panel Functions -
      //****************************************************************************/
      inline void  setPanelAngle(double _panelAngle) { panelAngle = _panelAngle; }
      inline float getPanelAngle() const { return panelAngle; }

      /***************************************************************************************/
      // setSolarPanelMeshNames()
      /***************************************************************************************/
      void setSolarPanelMeshNames(const std::vector<std::string> & names) {
        
        havePanels = true;
        
        solarPanelMesh.resize(model.size(), false);
        
        panelCenter = glm::vec3(0.0);
        
        std::size_t panelMeshNum = 0;
        
        for(std::size_t i=0; i<model.size(); ++i){
          
          if(std::find(names.begin(), names.end(),model[i].name) != names.end()) {
            
            solarPanelMesh[i] = true;
            
            glm::vec3 center; glm::vec3 size; float radius;
            
            model[i].bounds(center, size, radius);
            
            panelCenter += center;
            
            ++panelMeshNum;
            
          }
          
        }
        
        panelCenter /= (float) panelMeshNum;
        
      }
      
      /***************************************************************************************/
      // setLight() -
      /***************************************************************************************/
      void setLight(const glm::vec3 & _position, const glm::vec3 & _direction) { model.setLight(_position, _direction); }
      
    
    private:
      
      /***************************************************************************************/
      // modelRender() -
      /***************************************************************************************/
      void modelRender(ogl::glCamera * camera, bool withMaterials = true) {
        
        if(havePanels) {
          
          model.renderBegin(camera, withMaterials);
          
          const ogl::glShader & shader = model.getShader();
          
          shader.setUniform("model", model.getModelMatrix());
          
          // Disegno prima i pannelli
          for(std::size_t i=0; i<model.size(); ++i) {
            if(!solarPanelMesh[i]) model[i].render(shader, withMaterials);
          }
          
          glm::mat4 tp = glm::translate(glm::mat4(), -panelCenter);
          glm::mat4 tt = glm::translate(glm::mat4(), +panelCenter);
          glm::mat4 rp = glm::orientate4(glm::vec3(0.0, glm::radians(panelAngle), 0.0));
          glm::mat4 tb = glm::translate(glm::mat4(), model.getTranslation());
          glm::mat4 rb = glm::orientate4(model.getRotation());
          
          glm::mat4 modelPanels = tb * rb * tt * rp * tp;
          
          shader.setUniform("model", modelPanels);
          
          // poi disegno il corpo
          for(std::size_t i=0; i<model.size(); ++i) {
            if(solarPanelMesh[i]) model[i].render(shader, withMaterials);
          }
          
          model.renderEnd();
          
        } else {
          
          model.render(camera, withMaterials);
          
        }
        
      }
      
      /***************************************************************************************/
      // _initOrbit() -
      /***************************************************************************************/
      void _initOrbit(double jdStart, double jdEnd, double dt) {
        
        if(!isInitedTLE){
          fprintf(stderr, "The satellite must be inited before propagated the orbit\n");
          abort();
        }
        
        // vettore di appoggio con le posizioni
        std::vector<astro::SatelliteState> states;
        
        // creo il satellite
        astro::Satellite satellite(TLE_line1.c_str(), TLE_line2.c_str());
        
        // creo l'orbita
        satellite.orbit(jdStart, jdEnd, dt, states, CRS::ECEF);
        
        // fattore di normalizazione per portare in ER
        float factor = 6371.0;
        
        // alloco la memoria
        positions.resize(states.size());
        velocities.resize(states.size());
        
        // normalizzo e copio
        for(int i=0; i<states.size(); ++i) {
          
          positions[i].x = states[i].position[0] / factor;
          positions[i].y = states[i].position[1] / factor;
          positions[i].z = states[i].position[2] / factor;
          
          velocities[i].x = states[i].velocity[0] / factor;
          velocities[i].y = states[i].velocity[1] / factor;
          velocities[i].z = states[i].velocity[2] / factor;
          
        }
        
        orbitLine.update(positions);
        
        isInitedOrbit = true;
        
      }
      
      //****************************************************************************/
      // isToInit() -
      //****************************************************************************/
      bool isToInit() {
          
        DEBUG_LOG("visone::satellite::isToInit(" + name + ")");

        if(isInited) return isInited;

        if(!isInitedTLE) {
          fprintf(stderr, "The satellite TLE must be inited before rendering\n");
          abort();
        }
     
        if(!isInitedOrbit) {
          fprintf(stderr, "The satellite orbit must be inited before rendering\n");
          abort();
        }
     
        if(!isInitedAttitude) {
          fprintf(stderr, "The satellite attitude must be inited before rendering\n");
          abort();
        }
     
        if(!isInitedModel) {
          fprintf(stderr, "The satellite model must be inited before rendering\n");
          abort();
        }
        
        isInited = true;
        
        return isInited;
        
      }
    
      //****************************************************************************/
      // initToRender() -
      //****************************************************************************/
      void initToRender() {
                   
        DEBUG_LOG("visone::satellite::initToRender(" + name + ")");

        //sphere.initInGpu();
        
        //lineR.initInGpu();
        
        //lineS.initInGpu();
        
        //lineC.initInGpu();
        
        orbitLine.initInGpu();
        
        axes.initInGpu();
        
        model.initInGpu();
        
        isInitedToRender = true;
        
      }
    
  };

} /* namespace visone */

#endif /* _H_VISONE_SATELLITE_H_ */




