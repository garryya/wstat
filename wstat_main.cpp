/*
 * Author: garryya
 * Created on October 4, 2017, 5:41 PM
 */


//TODO: sort: possible memory corruption 
//TODO: sort: memory overhead
//TODO: sync locks ?
//TODO: log

#include "stdafx.h"

#include <iostream>
#include <iterator>
#include <set>
#include <algorithm>
#include <iomanip>

#include "wstat.h"


int main_console(int argc, char *argv[]) 
{
    if(argc <= 1)
    {
        std::cout << "No filename specified." << std::endl;
        return 1;
    }
    
    //std::cout << "* initializing ..." << std::endl;
    const char * szFilename = argv[1];
    static WStat wc;
    if(! wc.init(szFilename))
    {
        std::cout << "Cannot open " << szFilename << ":" << wc.get_error() << std::endl;
        return 1;
    }

    //std::cout << "* processing " << szFilename << " ..." << std::endl;
    if(! wc.process(true))
    {
        std::cout << "Processing failed:" << wc.get_error() << std::endl;
        return 1;
    }
    
    //std::cout << "* finished:" << std::endl;
    const WStat::TFreqs& freqs = wc.get_frequences();
    
    if(! freqs.empty())
    {
        WStat::TFreqRefs freqRefs;
        for(WStat::TFreqs::const_iterator i=freqs.begin(); i != freqs.end(); i++)
            if(! i->first.empty())
                freqRefs.push_back(&(*i)); 
        std::sort(freqRefs.begin(), freqRefs.end(), WStat::sort_by_freq);
    
        for(WStat::TFreqRefs::const_iterator i=freqRefs.begin(); i != freqRefs.end(); i++)
            std::cout << std::setw(32) << std::left << (*i)->first 
                        << std::setw(8) << std::right << (*i)->second << std::endl; 
    }   
    return 0;

	
}


int main(int argc, char *argv[]) 
{
   int ret = main_console(argc, argv);
   //std::getchar();
   return ret;
}
