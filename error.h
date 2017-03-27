#ifndef ERROR_H
#define ERROR_H

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <exception>
#include <memory>

namespace error
{
	struct base : std::exception
	{
		virtual void format_error_message()const = 0;

		const char* what() const throw()
		{
			format_error_message();
			return error_message_buffer;
		}

		mutable char error_message_buffer[256];
	};

	struct with_errno
	{
		with_errno()
		{
			errno_value = 0;
		}

		void set_errno(int errno_value)
		{
			this->errno_value = errno_value;
		}

		int errno_value;
	};
	
	struct with_index
	{
		index_not_found()
		{
			index = 0;
		}
		void set_index(int i) { index = i; }

		int index;
	};

	struct array_index_out_of_bounds:
		base,
		with_index,
		with_errno
	{
		void format_error_message() const
		{
			if(errno_value != 0)
			{
				snprintf(error_message_buffer, sizeof(error_message_buffer),
					"Array index \"%d\" is out of array scope because \"%s\".",
					index, std::strerror(errno_value));
			}
			else
			{
				snprintf(error_message_buffer, sizeof(error_message_buffer),
					"Array index \"%d\" is out of array scope.",
					index);
			}
		}
	};

	struct with_iterator_range
	{
		with_iterator_range()
		{
			iterator_range = std::pair<int, int>(0, 0);
		}

		void set_iterator_range(int first, int last)
		{
			iterator_range.first = first;
			iterator_range.second = last;
		}

		std::pair<int, int> iterator_range;
	};

	struct invalid_iterator_range:
		base,
		with_iterator_range
	{
		void format_error_message() const
		{
			if(iterator_range.first > iterator_range.second)
			{
				snprintf(error_message_buffer, sizeof(error_message_buffer),
					"Iterator range position \"%d\" to position \"%d\" is invalid, negative range is not possible.",
					iterator_range.first, iterator_range.second);
			}
			else
			{
				snprintf(error_message_buffer, sizeof(error_message_buffer),
					"Iterator range position \"%d\" to position \"%d\" is invalid.",
					iterator_range.first, iterator_range.second);
			}
		}
	};

	struct with_node_name
	{
		with_node_name()
		{
			node_name = "";
		}
		void set_node_name(std::string s) { node_name = s; }
		std::string node_name;
	};

	struct node_not_in_graph:
		base,
		with_node_name
	{
		void format_error_message() const
		{
			snprintf(error_message_buffer, sizeof(error_message_buffer),
				"Node with name \"%s\" not found within graph.",
				node_name);
		}
	};

	struct with_edge
	{
		with_edge()
		{
			edge_origin = "";
			edge_dest = "";
			edge_cost = 0.0;
		}
		set_edge_origin(std::string s) { edge_origin = s; }
		set_edge_dest(std::string s) { edge_dest = s; }
		set_edge_cost(double c) { edge_cost = c; }

		std::string edge_origin, edge_dest;
		double edge_cost;
	};

	struct edge_not_in_node:
		base,
		with_edge
	{
		void format_error_message() const
		{
			snprintf(error_message_buffer, sizeof(error_message_buffer),
				"Edge originating at \"%s\" with destination \"%s\" and cost \"%d\" does not exist.",
				edge_origin, edge_dest, edge_cost);
		}
	};
};

#endif