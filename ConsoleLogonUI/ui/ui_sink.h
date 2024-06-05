#pragma once
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fwd.h"

struct log_message
{
	std::string msg;
	spdlog::level::level_enum level;
};

namespace log_global
{
	inline bool should_console_scroll_down = false;
	inline std::vector<log_message> logs;
}

template<typename Mutex>
class ui_sink : public spdlog::sinks::base_sink<Mutex>
{
public:
	explicit ui_sink() {}
protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		log_global::should_console_scroll_down = true;

		spdlog::memory_buf_t formatted;
		spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

		log_global::logs.push_back({
			fmt::to_string(formatted),
			msg.level
			});
	}

	void flush_() override
	{
	}
};

#include "spdlog/details/null_mutex.h"
#include <mutex>
using ui_sink_mt = ui_sink<std::mutex>;
using ui_sink_st = ui_sink<spdlog::details::null_mutex>;