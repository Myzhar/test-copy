#include <string.h> // memcpy
#include <vector>
#include <iostream>     // std::cout
#include <iomanip>      // std::setprecision
#include <math.h>
#include <thread>

#include "test-cpp.h"

template<typename T>
void test(int size, double& memcpy_msec, double& stdcopy_msec);

double calcPerc(double memcpy_msec, double stdcopy_msec);
void printResultLine(int size, double memcpy_msec, double stdcopy_msec);

int main(int argc, char *argv[])
{
    cout << " SIZE\tmemcpy [ms]\tstd::copy [ms]\tDIFF [ms]\tBETTER" << endl;
    cout << "=======================================================================" << endl;

    double memcpy_msec;
    double stdcopy_msec;

    for( int i=0; i<=10; i++ )
    {
        int size = pow(2,10+i);
        test<double>( size, memcpy_msec, stdcopy_msec );
        printResultLine( size*sizeof(double), memcpy_msec, stdcopy_msec);

        //cout << "-----------------------------------------------------------------------" << endl;
    }

    return 0;
}

template<typename T>
void test(int size, double &memcpy_msec, double &stdcopy_msec)
{
    int iterations = 10000;

    // >>>>> Initialization
    vector<T> src;
    src.resize(size);

    for( int i=0; i<size; i++ )
    {
        src[i]=i;
    }

    vector<T> dst;
    dst.resize(size);
    // <<<<< Initialization

    Timer elabTimer;

    // >>>>> Test memcpy
    for(int i=0; i<iterations; i++)
    {
        elabTimer.tic();
        memcpy( &(dst.data()[0]), &(src.data()[0]), size*sizeof(T) );
        double elapsed = elabTimer.toc();
        memcpy_msec += elapsed;

        //std::this_thread::sleep_for( std::chrono::milliseconds(5));
    }

    memcpy_msec =  memcpy_msec/iterations;
    // <<<<< Test memcpy

    // >>>>> Test std::copy
    for(int i=0; i<iterations; i++)
    {
        elabTimer.tic();
        std::copy( src.begin(), src.end(), dst.begin() );
        double elapsed = elabTimer.toc();
        stdcopy_msec += elapsed;

        //std::this_thread::sleep_for( std::chrono::milliseconds(5));
    }

    stdcopy_msec =  stdcopy_msec/iterations;
    // <<<<< Test std::copy
}

double calcPerc(double memcpy_msec, double stdcopy_msec)
{
    double ratio = stdcopy_msec/memcpy_msec;
    if(stdcopy_msec<memcpy_msec)
    {
        ratio = 1./ratio;
    }
    ratio -= 1.;

    return 100.*ratio;
}

void printResultLine( int size, double memcpy_msec, double stdcopy_msec)
{
    double perc = calcPerc( memcpy_msec, stdcopy_msec );

    std::string better = (memcpy_msec<stdcopy_msec)?"memcpy":"std::copy";

    std::cout << std::fixed;

    int size_str = size;
    int count = 0;
    while(size_str>=1024)
    {
        count++;
        size_str /=1024;
    }

    string unit;
    switch (count) {
    case 0:
        unit = "B";
        break;
    case 1:
        unit = "KB";
        break;
    case 2:
        unit = "MB";
        break;
    case 3:
        unit = "GB";
        break;
    default:
        break;
    }

    cout << std::setprecision(10) <<  " " << size_str << unit << "\t" << memcpy_msec << "\t" << stdcopy_msec
         << "\t" << std::setprecision(10) << /*perc*/ stdcopy_msec-memcpy_msec << "\t" <<  better << endl;
}
