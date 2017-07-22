#pragma once

#include <boost/log/core.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/shared_ptr.hpp>

void log_formatter(const boost::log::record_view &view, boost::log::formatting_ostream &os);

void init_logging();

enum log_level {
	ERROR,
	WARNING,
	INFO,
	NOTICE,
	DEBUG
};