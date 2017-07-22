#include "stdafx.h"
#include "log.h"
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

void log_formatter(const boost::log::record_view &view, boost::log::formatting_ostream &os)
{
	log_level l = view.attribute_values()["Severity"].extract<log_level>().get();
	std::string level = "DEBUG";
	switch (l) {
	case INFO:
		level = "INFO";
		break;
	case ERROR:
		level = "ERROR";
		break;
	case NOTICE:
		level = "NOTICE";
		break;
	case DEBUG:
		level = "DEBUG";
		break;
	case WARNING:
		level = "WARNING";
		break;
	}
	os << "[" << boost::posix_time::to_simple_string(view.attribute_values()["TimeStamp"].extract<boost::posix_time::ptime>().get())
		<< "]{" << level << "}: " <<
		view.attribute_values()["Message"].extract<std::string>();
}

void init_logging() {
	boost::log::add_common_attributes();
	typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > text_sink;
	boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

	//auto filestream = boost::make_shared< std::ostream >("CControl.log");
	//// Add a stream to write log to
	//sink->locked_backend()->add_stream(filestream);

	boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
	sink->locked_backend()->add_stream(stream);
	sink->set_formatter(&log_formatter);
	// Register the sink in the logging core
	boost::log::core::get()->add_sink(sink);
}