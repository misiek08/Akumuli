/**
 * PRIVATE HEADER
 *
 * Utils
 *
 * Copyright (c) 2013 Eugene Lazin <4lazin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */


#pragma once

#include <apr_general.h>
#include <apr_mmap.h>
#include <log4cxx/logger.h>
#include <stdexcept>
#include <ostream>

namespace Akumuli
{
    /** Unsafe method marker */
    struct UnsafeTag {};

    /** APR error wrapper.
     *  Code must deal with APR error codes and only if
     *  it can't handle some error - it must throw AprException
     *  to panic!
     */
    struct AprException : public std::runtime_error
    {
        apr_status_t status;
        AprException(apr_status_t status, const char* message);
    };

    std::ostream& operator << (std::ostream& str, AprException const& except);


    /** Memory mapped file
      * maps all file on construction
      */
    class MemoryMappedFile
    {
        apr_pool_t* mem_pool_;  //< local memory pool
        apr_mmap_t *mmap_;
        apr_file_t *fp_;
        apr_finfo_t finfo_;
        static log4cxx::LoggerPtr s_logger_;
        apr_status_t status_;
    public:
        MemoryMappedFile(const char* file_name) noexcept;
        ~MemoryMappedFile();
        void* get_pointer() const noexcept;
        size_t get_size() const noexcept;
        apr_status_t flush() noexcept;
        bool is_bad() const noexcept;
        std::string error_message() const noexcept;
        void throw_if_bad();
        apr_status_t status_code() const noexcept;
    private:
        void free_resources(int cnt);
    };
}