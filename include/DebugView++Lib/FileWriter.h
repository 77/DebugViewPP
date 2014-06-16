// (C) Copyright Gert-Jan de Vos and Jan Wilmans 2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// Repository at: https://github.com/djeedjay/DebugViewPP/

#pragma once

#include <boost/thread.hpp>

namespace fusion {
namespace debugviewpp {

class FileWriter
{
public:
	explicit FileWriter(const std::wstring& filename);
    ~FileWriter();

private:
	std::wstring m_filename;	
};

} // namespace debugviewpp 
} // namespace fusion
