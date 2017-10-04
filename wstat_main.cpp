/*
 * File:   main.cpp
 * Author: garryya
 *
 * Created on October 3, 2017, 5:41 PM
 */


//TODO: sort: memory overhead maybe
//TODO: frequencies
//TODO: use iterator


#include <iostream>
#include <iterator>
#include <set>
#include <algorithm>
#include <vector>
#include <iomanip>

#include "wstat.h"

typedef std::pair<const char*, int> T;

bool sort_by_freq(const T& f1, const T& f2)
{
      return f1.second >= f2.second;
}


int main_console(int argc, char *argv[]) 
{
    if(argc <= 1)
    {
        std::cout << "No filename specified." << std::endl;
        return 1;
    }
    
    std::cout << "* initializing ..." << std::endl;
    const char * szFilename = argv[1];
    static WStat wc;
    if(! wc.init(szFilename))
    {
        std::cout << "Cannot open " << szFilename << ":" << wc.get_error() << std::endl;
        return 1;
    }

    std::cout << "* processing " << szFilename << " ..." << std::endl;
    if(! wc.process())
    {
        std::cout << "Processing failed:" << wc.get_error() << std::endl;
        return 1;
    }
    
    std::cout << "* finished:" << std::endl;
    const WStat::TFreqs& freqs = wc.get_frequences();
    
    if( ! freqs.empty() )
    {
        std::vector<T> fs;
        for(WStat::TFreqs::const_iterator i=freqs.begin(); i != freqs.end(); i++)
            if( ! i->first.empty() )
            fs.push_back(T(i->first.c_str(), i->second)); 

        std::sort(fs.begin(), fs.end(), sort_by_freq);
    
        for( std::vector<T>::const_iterator i=fs.begin(); i != fs.end(); i++)
            std::cout << std::setw(32) << std::left << (*i).first 
                        << std::setw(8) << std::right << (*i).second << std::endl; 
    }   
    return 0;
}


int main(int argc, char *argv[]) 
{
   return main_console(argc, argv);
}
