/*
 * 
 */

#include <iostream>
#include <iterator>
#include <algorithm>

#include "wstat.h"


WStat::WStat() 
    : m_bInited(false)
{
}


WStat::~WStat() 
{
}


bool WStat::init(const char* szFilename)
{
    this->m_bInited = false;
    try
    {
        this->m_file.open(szFilename);
        this->m_bInited = ! this->m_file.fail(); 
    }
    catch(std::ios_base::failure& x)
    {
        this->set_error(x.what());
        return false;
    }
    return this->m_bInited;
}


void WStat::reset()
{
    this->m_strError.clear();
    this->m_freqs.clear();
}


void WStat::set_error(const char * szError)
{
    this->m_strError = szError;
}

void WStat::update(const std::string& _word)
{
    std::string word = _word;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    TFreqs::iterator i = this->m_freqs.find(word);
    if( i == this->m_freqs.end() )
        m_freqs[word] = 0;
    m_freqs[word]++;
}

bool WStat::process()
{
    this->reset();
    if( ! this->m_bInited )
    {
        this->set_error("Not initialized.");
        return false;
    }
    std::string word;
    char c;
    while(this->m_file.get(c))
    {
        if( isspace(c) )
        {
            if( ! word.empty() )
            {
                this->update(word);
                word.clear();
            }
        }
        else
            word += c;
    }
    return true;
}



