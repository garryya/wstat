/*
 * 
 */

#include <iostream>
#include <iterator>
#include <algorithm>
#include <thread>
#include <deque>

#include "wstat.h"


WStat::WStat(size_t nThreads, size_t nChunkSize) 
    : m_bInited(false), m_nThreads(nThreads), m_nChunkSize(nChunkSize)
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

void WStat::count_words(const std::string& text, WStat& ws)
{
    std::string word;
    for(const char* s = text.c_str(); *s; s++)
    {
        char c = *s;
        if( ! isspace(c) )
            word += c;
        else if( word.empty() )
            continue;
        else
        {
            ws.update(word);
            word.clear();
        }
    }
    if( ! word.empty() )
        ws.update(word);
}


class TTPool
{
protected:
    
    size_t m_nThreads;
    WStat * m_pws;
    
    typedef std::vector<std::thread> TThreads;
    TThreads m_threads;
    
    typedef std::deque<std::string> TQueue;
    typedef std::shared_ptr<TQueue> QueuePtr; 
    QueuePtr m_ptrq;

public:

    static void t_count(QueuePtr ptrq, WStat* p_ws)
    {   
        while(true)
        {
            if( ptrq->empty() )
                return;
            const std::string& text = ptrq->front();
            ptrq->pop_front();
            p_ws->count_words(text, *p_ws);
        }
    }

    TTPool(size_t nThreads, WStat* pws) : m_nThreads(nThreads), m_pws(pws)
    {
        m_ptrq =  QueuePtr(new TQueue());
    }
    
    virtual ~TTPool() 
    {
    }

    void start()
    {
        for(size_t i=0; i<this->m_nThreads; i++)
            this->m_threads.push_back(std::thread(TTPool::t_count, this->m_ptrq, this->m_pws));
        for(TThreads::iterator i=this->m_threads.begin(); i != this->m_threads.end(); i++)
            i->join();
    }
    
    // TODO sync!!!
    void enqueue(const std::string& chunk)
    {
        this->m_ptrq->push_back(chunk);
    }
};


bool WStat::process(bool bParallel)
{
    try
    {
        this->reset();
        if( ! this->m_bInited )
        {
            this->set_error("Not initialized.");
            return false;
        }
        if( ! bParallel )
        {
            //TODO - read all into memory, not good..
            std::string line, text;
            while(std::getline(this->m_file, line))
                text += line + " ";
            count_words(text, *this);
        }
        else
        {
            TTPool tp(WStat::m_nThreads, this);
            TChunks chunks;
            while(! this->m_file.eof())
            {
                char c;
                std::string chunk;
                // read exactly N chars (chunk size) 
                while(chunk.size()<WStat::m_nChunkSize && this->m_file.get(c))
                    chunk += c;
                if( chunk.empty() )
                    continue;
                // then align to word boundary
                if( ! std::isspace(*chunk.rbegin()) ) 
                    while(this->m_file.get(c) && ! std::isspace(c))
                        chunk += c;
                chunks.push_back(chunk);
                //tp.enqueue(chunk);
                chunk.clear();
            }

            for(TChunks::const_iterator i=chunks.begin(); i != chunks.end(); ++i)
                count_words(*i, *this);
            
            //tp.start();
        }
        return true;
    }
    catch(std::exception& x)
    {
        this->set_error(x.what());        
        return false;
    }

}

