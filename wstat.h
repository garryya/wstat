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

#ifndef WCOUNT_H
#define WCOUNT_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>


class WStat
{
public:
    
    typedef std::map<std::string, int> TFreqs;
    
private:
    
    bool m_bInited;
    std::string m_strError;
    std::ifstream m_file;

    TFreqs m_freqs;
    
public:
    WStat();
    virtual ~WStat();
    
    bool init(const char* szFilename);
    
    virtual bool process();

    const char* get_error() const { return m_strError.c_str(); }
    
    const TFreqs& get_frequences() const { return this->m_freqs; }
        
protected:
    void reset();
    void set_error(const char * szError);
    void update(const std::string& s);
    
};




#endif /* WCOUNT_H */

