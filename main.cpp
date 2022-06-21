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

#include <cstdio>
#include <cstdlib>

//#define DEBUG_OGL
#define DEBUG_VISONE

#include "visone.hpp"

//****************************************************************************/
// main
//****************************************************************************/
int main(int argc, char *argv[]) {

  // Tempo di inzio e di fine in jDay
  double jdStart = astro::Date(29,9,2019,17,32,38).getJDay();
  double jdEnd   = astro::Date(29,9,2019,17,38,31).getJDay();
  
  // Tempo di integrazione in secondi
  double dt = 1;
  
  // Inizializzo il sole
  visone::sun sun(jdStart, jdEnd, dt);
  
  // Inizializzo la stazione a terra
  visone::observatory observatory(41.765239, 13.375041, 555, jdStart, jdEnd, dt); // Collepardo

  // TLE del satellite (Jason-2 TLE 29/9/2019)
  char TLE_line1[] = "1 33105U 08032A   19271.96118080 -.00000009 +00000-0 +21187-3 0  9990";
  char TLE_line2[] = "2 33105 066.0408 169.1252 0007897 266.0656 211.8718 12.87626577527874";
  
  // Definisco il satellite
  visone::satellite satellite;
  
  // Carico il modello del satellite
  satellite.init("~/Research/VISONE/models/Jason-2/Jason-2.obj", 0.1, TLE_line1, TLE_line2, jdStart, jdEnd, dt);
  
  // Velocita inziali del assetto del satellite
  glm::vec3 omega = glm::vec3(0.0, 0.0, 0.0);
  
  // Rotazione iniziale del assetto del satellite
  glm::quat rotation = glm::vec3(0.0, 0.0, 0.0);

  // Propago l'assetto del satellite
  satellite.initAttitude(omega, rotation, glm::vec3(1.0f), dt);
  
  if(sun.lenght() != observatory.lenght() || observatory.lenght() != satellite.lenght()){
    fprintf(stderr, "Le propagazioni non sono andate a bun fine\n");
    abort();
  }
    
  // Inizializzo il rendering
  std::vector<double> lightcurve;
  std::string outputPath = "~/Desktop/Jason/";
  bool withLightcurve    = false;
  bool withImages        = false;
  bool looping           = true;
  bool withExtra         = true;
  
  // Inizializo il renderer (va fatto come prima cosa)
  visone::renderer renderer(800, 600, 45);

  // Effettuo il rendering
  renderer.debug(sun, observatory, satellite, lightcurve, outputPath, withLightcurve, withImages, looping, withExtra);
  
  // Se serve mi salvo la curva di luce
  if(lightcurve.size() != 0) {
    FILE * output = mpl::io::open(outputPath + "lightcurve.dat", "w");
    for(size_t i=0; i<lightcurve.size(); ++i) {
      fprintf(output, "%lu %e\n", i, lightcurve[i]);
    }
    mpl::io::close(output);
  }
  
  return 0;

}
