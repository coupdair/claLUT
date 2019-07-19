#ifndef _DATA_GENERATOR_
#define _DATA_GENERATOR_

//CoolImage
#include "CImg.h"
using namespace cimg_library;

//thread lock
#include "CDataAccess.hpp"

template<typename Tdata, typename Taccess>
class CDataGenerator: public CDataAccess
{
public:

  CDataGenerator(std::vector<omp_lock_t*> &lock)
  : CDataAccess(lock)
  {
    debug=true;
    class_name="CDataGenerator";
    check_locks(lock);
  }//constructor
  virtual void iteration(CImg<Taccess> &access,CImgList<Tdata> &images, int n, int i)
  {
    if(debug)
    {
      lprint.print("",false);
      printf("4 B%02d #%04d: ",n,i);fflush(stdout);
      access.print("access",false);fflush(stderr);
      lprint.unset_lock();
    }
    //wait lock
    unsigned int c=0;
    laccess.wait_for_status(access[n],STATUS_FREE,STATE_FILLING, c);//free,filling

    //fill image
    images[n].fill(i);

    //set filled
    laccess.set_status(access[n],STATE_FILLING,STATUS_FILLED, class_name[5],i,n,c);//filling,filled
  }//iteration
};//CDataGenerator

#endif //_DATA_GENERATOR_

