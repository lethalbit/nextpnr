/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2025 Aki Van Ness <aki@lethalbit.net>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#if !defined(NEXTPNR_PYTHON_BINDINGS)
#define NEXTPNR_PYTHON_BINDINGS

#include <filesystem>
#include <optional>
#include <string_view>
#include <string>

#include <pybind11/pybind11.h>

#include "log.h"
#include "nextpnr.h"

NEXTPNR_NAMESPACE_BEGIN

namespace python {
	namespace fs = std::filesystem;
	namespace py = pybind11;

	// Initialize the embedded Python interpreter
	void initialize(std::optional<std::string_view> executable = std::nullopt);
	// Teardown the embedded Python interpreter
	void teardown();
	// Execute a Python source file from disk
	void exec_file(fs::path file);
	void exec_file(std::string& file);
	void exec_file(const char* file);
	// Execute standalone Python code
	void exec_code(std::string_view code);
	void exec_code(std::string& code);
	void exec_code(const char* code);

	// Unwind and dump a python traceback
	void dump_traceback(const py::error_already_set& exception);

	template<typename T>
	void inject_global(const char* name, T& obj) {
		try {
			auto py_object{py::cast(obj, py::return_value_policy::reference)};
			py::module_::import("__main__").attr(name) = py_object.ptr();
		} catch (py::error_already_set& err) {
			log("Unable to inject global object '%s' into Python runtime\n", name);
			dump_traceback(err);
			std::terminate();
		}
	}
}

NEXTPNR_NAMESPACE_END

#endif /* NEXTPNR_PYTHON_BINDINGS */
