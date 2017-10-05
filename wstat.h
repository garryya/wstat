/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   wcount.h
 * Author: garryya
 *
 * Created on October 3, 2017, 5:45 PM
 */

#ifndef WSTAT_H
#define WSTAT_H

#include <memory>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>


class WStat
{
public:
    
    typedef std::shared_ptr<std::string> TStringPtr;

    typedef std::map<std::string, int> TFreqs;
    typedef std::vector<const TFreqs::value_type*> TFreqRefs;
    
    typedef std::vector<TStringPtr> TChunks;
        
protected:
    
    bool m_bInited;
    std::string m_strError;
    std::ifstream m_file;

    TFreqs m_freqs;
    
    const size_t m_nThreads;
    const size_t m_nChunkSize;
    
public:
    WStat(size_t nThreads = 8, size_t nChunkSize = 256);
    virtual ~WStat();
    
    bool init(const char* szFilename);
    
    virtual bool process(bool bParallel);

    const char* get_error() const { return m_strError.c_str(); }
    
    const TFreqs& get_frequences() const { return this->m_freqs; }
        
    void update(const std::string& s);
    
    static void count_words(const std::string& text, WStat& ws);
    
    static bool sort_by_freq(const WStat::TFreqs::value_type* f1, const WStat::TFreqs::value_type* f2);
protected:
    void reset();
    void set_error(const char * szError);
    
};



#endif /* WSTAT_H */

