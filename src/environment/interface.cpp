//------------------------------------------------------------------------------
//
//   This file is part of the VAMPIRE open source package under the
//   Free BSD licence (see licence file for details).
//
//   (c) Sarah Jenkins 2016. All rights reserved.
//
//   Email: sj681@york.ac.uk
//
//------------------------------------------------------------------------------
//

// C++ standard library headers
#include <string>

// Vampire headers
#include "environment.hpp"
#include "errors.hpp"
#include "vio.hpp"

// environment module headers
#include "internal.hpp"

namespace env = environment::internal;

namespace environment{

   //---------------------------------------------------------------------------
   // Function to process input file parameters for environment module
   //---------------------------------------------------------------------------
   bool match_input_parameter(std::string const key, std::string const word, std::string const value, std::string const unit, int const line){

      // Check for valid key, if no match return false
      std::string prefix="environment";
      if(key!=prefix) return false;

      std::string test="enabled";
      if(word==test){
         enabled = true;
         return true;
      }

      test="system-size-x";
      if(word==test){
         double x=atof(value.c_str());
         vin::check_for_valid_value(x, word, line, prefix, unit, "length", 10, 1.0e8,"input","10 Angstroms - 10 millimetre");
         env::dim[0] =x;
         return true;
      }
      test="system-size-y";
      if(word==test){
         double y=atof(value.c_str());
         vin::check_for_valid_value(y, word, line, prefix, unit, "length", 10, 1.0e8,"input","10 Angstroms - 10 millimetre");
         return true;
      }
      test="system-size-z";
      if(word==test){
         double z=atof(value.c_str());
         vin::check_for_valid_value(z, word, line, prefix, unit, "length", 10, 1.0e8,"input","10 Angstroms - 10 millimetre");
         return true;
      }
      test="system-size";
      if(word==test){
         double d=atof(value.c_str());
         vin::check_for_valid_value(d, word, line, prefix, unit, "length", 10, 1.0e8,"input","10 Angstroms - 10 millimetre");
         env::dim[0] =d;
         env::dim[1] =d;
         env::dim[2] =d;
         return true;
      }

      test="cell-size";
      if(word==test){
         double d=atof(value.c_str());
         vin::check_for_valid_positive_value(d, word, line, prefix, unit, "length", 10, 1.0e6,"input","10 Angstroms - 0.1 millimetre");
         env::cell_size[0] =d;
         env::cell_size[1] =d;
         env::cell_size[2] =d;
         return true;
      }
      test="cell-size-x";
      if(word==test){
         double d=atof(value.c_str());
         vin::check_for_valid_positive_value(d, word, line, prefix, unit, "length", 10, 1.0e6,"input","10 Angstroms - 0.1 millimetre");
         env::cell_size[0] =d;
         return true;
      }
      test="cell-size-y";
      if(word==test){
         double d=atof(value.c_str());
         vin::check_for_valid_positive_value(d, word, line, prefix, unit, "length", 10, 1.0e6,"input","10 Angstroms - 0.1 millimetre");
         env::cell_size[1] =d;
         return true;
      }
      test="cell-size-z";
      if(word==test){
         double d=atof(value.c_str());
         vin::check_for_valid_positive_value(d, word, line, prefix, unit, "length", 10, 1.0e6,"input","10 Angstroms - 0.1 millimetre");
         env::cell_size[2] =d;
         return true;
      }

      test="exchange-constant";
      if(word==test){
         double exc=atof(value.c_str());
         vin::check_for_valid_positive_value(exc, word, line, prefix, unit, "exchange", 0,2e-15 ,"input","0 - 2e-15");
         env::A =-exc;

         return true;
      }

      test="uniaxial-anisotropy-constant";
      if(word==test){
         double k=atof(value.c_str());
         vin::check_for_valid_value(k, word, line, prefix, unit, "anisotropy", 0,1e-15 ,"input","0 - 1e-18");
         std::cout << k <<std::endl;
         env::ku =k;
         return true;
      }
      test="Ms";
      if(word==test){
         double A=atof(value.c_str());
         std::cout << A << std::endl;
         vin::check_for_valid_positive_value(A, word, line, prefix, unit, "magnetisation", 1e-35,1e-15 ,"input","1e-32 - 1e-18");
         env::Ms =A;
         return true;
      }
      //initialuse the number of atomsitic steps per micromagnetic step
      test="atomistic-steps-per-env-step";
      if(word==test){
         double dt=atof(value.c_str());
         vin::check_for_valid_positive_value(dt, word, line, prefix, unit, "none", 1, 100000,"input","1 step - 100,000 steps");
         environment::num_atomic_steps_env =dt;
         return true;
      }


      test="Tc";
      if(word==test){
         double T=atof(value.c_str());
         vin::check_for_valid_positive_value(T, word, line, prefix, unit, "none", 0, 10000,"input","0 - 10,000");
         env::Tc =T;
         return true;
      }

      test="damping-constant";
      if(word==test){
         double a=atof(value.c_str());
         vin::check_for_valid_positive_value(a, word, line, prefix, unit, "none", 0,1,"input","0- 1");
         env::alpha =a;
         return true;
      }
      test="gyromagnetic-ratio";
      if(word==test){
         double g=atof(value.c_str());
         vin::check_for_valid_positive_value(g, word, line, prefix, unit, "none", 0,1,"input","0- 1");
         env::gamma =g;
         return true;
      }
      test="initial-spin-direction";
      if(word==test){
         // first test for random spins
         test="random";
         if(value==test){
            env::random_spins=true;
         }
         else{
            // temporary storage container
            std::vector<double> u(3);
            // read values from string
            u=vin::DoublesFromString(value);
            vin::check_for_valid_value(u.at(0), word, line, prefix, unit, "none", 0,1,"input","0- 1");
            vin::check_for_valid_value(u.at(1), word, line, prefix, unit, "none", 0,1,"input","0- 1");
            vin::check_for_valid_value(u.at(2), word, line, prefix, unit, "none", 0,1,"input","0- 1");

            // Copy sanitised unit vector to material
            env::initial_spin[0]=u.at(0);
            env::initial_spin[1]=u.at(1);
            env::initial_spin[2]=u.at(2);

            // ensure random spins is unset
            env::random_spins=false;
         }
         // return
         return true;
      }

      test="atomistic-position";
      if(word==test){
         // temporary storage container
         std::vector<double> u(3);

         // read values from string
         u=vin::DoublesFromString(value);
         vin::check_for_valid_value(u.at(0), word, line, prefix, unit, "length", 0,100000,"input","0- 100,000");
         vin::check_for_valid_value(u.at(1), word, line, prefix, unit, "length", 0,100000,"input","0- 100,000");
         vin::check_for_valid_value(u.at(2), word, line, prefix, unit, "length", 0,100000,"input","0- 100,000");
         // Copy sanitised unit vector to material
         env::shift[0]=u.at(0);
         env::shift[1]=u.at(1);
         env::shift[2]=u.at(2);
         return true;
      }

      test="demag-field-update-rate";
      if(word==test){
         int dpur=atoi(value.c_str());
         vin::check_for_valid_int(dpur, word, line, prefix, 0, 1000000,"input","0 - 1,000,000");
         #ifdef FFT
         environment::demag_update_rate=dpur;
         #else
         std::cout << "FFT not enabled - no demag calculation" <<std::endl;
         #endif
         return true;
      }

      //--------------------------------------------------------------------
      // Keyword not found
      //--------------------------------------------------------------------
      return false;

   }

   //---------------------------------------------------------------------------
   // Function to process material parameters
   //---------------------------------------------------------------------------
   bool match_material_parameter(std::string const word, std::string const value, std::string const unit, int const line, int const super_index, const int sub_index){

      // add prefix string
      std::string prefix="material:";

      //--------------------------------------------------------------------
      // Keyword not found
      //--------------------------------------------------------------------
      return false;

   }

} // end of environment namespace
