#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>


namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace logging = boost::log;
namespace expr = boost::log::expressions;
namespace attr = boost::log::attributes;

typedef sinks::synchronous_sink<sinks::text_file_backend> text_sink_t;

enum severity_level
{
	normal,
	notification,
	warning,
	error,
	critical
};

// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
	static const char* const str[] =
	{
		"normal",
		"notification",
		"warning",
		"error",
		"critical"
	};
	if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast<int>(lvl);
	return strm;
}


void init_logging()
{
	boost::shared_ptr<sinks::text_file_backend> backend =
		boost::make_shared<sinks::text_file_backend>(
			keywords::file_name = "%Y%m%d_%5N.log",
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0)
			);

	boost::shared_ptr<text_sink_t> sink(new text_sink_t(backend));
	logging::core::get()->add_sink(sink);

	sink->set_formatter(expr::stream
		<< expr::attr<unsigned int>("RecordID")
		<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%d.%m.%Y %H:%M:%S.%f") << "]"
		<< "[" << expr::attr<severity_level>("Severity") << "]"
		<< "[" << expr::attr<boost::posix_time::time_duration>("Uptime")
		<< "] ["
		<< expr::if_(expr::has_attr("Tag"))
		[
			expr::stream << expr::attr<std::string>("Tag")
			<< "] ["
		]
		<< expr::format_named_scope("Scope", keywords::format = "%n", keywords::iteration = expr::reverse) << "] "
		<< expr::smessage);

	attr::counter<unsigned int> RecordID(1);
	logging::core::get()->add_global_attribute("RecordID", RecordID);
	attr::local_clock TimeStamp;
	logging::core::get()->add_global_attribute("TimeStamp", TimeStamp);

	BOOST_LOG_SCOPED_THREAD_ATTR("Uptime", attr::timer());

	attr::named_scope Scope;
	logging::core::get()->add_thread_attribute("Scope", Scope);

	sink->set_filter(
		expr::attr<severity_level>("Severity").or_default(normal) >= warning
		|| expr::begins_with(expr::attr<std::string>("Tag").or_default(std::string()), "IMPORTANT")
		);

}