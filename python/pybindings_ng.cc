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

#include <array>
#include <filesystem>
#include <optional>
#include <string_view>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "pybindings_ng.h"

#include "context.h"
#include "basectx.h"
#include "nextpnr_base_types.h"
#include "nextpnr_types.h"
#include "idstring.h"

#include "log.h"
#include "nextpnr.h"
#include "version.h"

NEXTPNR_NAMESPACE_BEGIN

#define STRINGIFY_(STR) #STR
#define STRINGIFY(STR) STRINGIFY_(STR)

namespace py = pybind11;

namespace python {
	void initialize(std::optional<std::string_view> executable) {
		std::array<const char*, 1> py_args{{
			"nextpnr"
		}};

		py::initialize_interpreter(true, py_args.size(), py_args.data());
		// This import triggers the loading and registration of all our types
		py::module_::import("nextpnr");
	}

	void teardown() {
		py::finalize_interpreter();
	}

	void exec_file(fs::path file) {
		if (!fs::exists(file)) {
			log_error("Fatal error: file not found %s\n", file.c_str());
			exit(1);
		}

		auto scope{py::module_::import("__main__").attr("__dict__")};
		py::eval_file(file.c_str(), scope);
	}

	void exec_file(std::string& file) {
		exec_file(fs::path(file));
	}

	void exec_file(const char* file) {
		exec_file(fs::path(file));
	}

	void exec_code(std::string_view code) {
		auto scope{py::module_::import("__main__").attr("__dict__")};
		py::exec(code, scope);
	}

	void exec_code(std::string& code) {
		auto scope{py::module_::import("__main__").attr("__dict__")};
		py::exec(code, scope);
	}

	void exec_code(const char* code) {
		auto scope{py::module_::import("__main__").attr("__dict__")};
		py::exec(code, scope);
	}

	void dump_traceback(const py::error_already_set& exception) {
		const auto type{exception.type()};
		const auto value{exception.value()};
		const auto trace{exception.trace()};

		auto message{std::string("")};

		if (!type.is_none()) {
			if (py::hasattr(type, "__name__")) {
				message.append(type.attr("__name__").cast<std::string>());
			} else {
				message.append(py::str(type));
			}
			message.append(": ");
		}

		if (!value.is_none()) {
			message.append(py::str(value));
			message.append("\n");
		}

		if (!trace.is_none()) {
			auto format_tb{py::module_::import("traceback").attr("format_tb")};
			// py::str stack{py::str("\n") + format_tb(trace)};
			// message.append(stack);
		}

		log("%s", message.c_str());
	}
}

/*
	NOTE(aki):

	The way this is done may seem a bit odd, and not C++ like, and that's because this module is
	implemented in such a way as if it was a normal pure Python module, so we follow the pythonic
	conventions for naming, ordering of things, and such.

	This makes the API a little more natural to use from Python, and will also hopefully not cause
	any possible lints from things like flake8 or ruff if they were to be run on type stubs generated
	from this module.

	We also use nested modules here to try to keep things a bit more logically organized, the general
	layout is like so:

		* `nextpnr`
			* `__version__`
			* `__doc__`
			* `types`
				* ...
			* `<ARCH>`
				* ...
*/

/* nextpnr module */
PYBIND11_EMBEDDED_MODULE(nextpnr, m) {
	/* ==== Root Module Globals ==== */
	m.doc() = "Next-generation Python API for nextpnr";

	m.attr("__version__") = GIT_DESCRIBE_STR;
	m.attr("ARCH") = STRINGIFY(ARCHNAME);

	/* ==== nextpnr types ==== */
	auto m_types{m.def_submodule("types", "Core nextpnr types")};

	auto m_types_all{py::list()};

	/* `nextpnr.types.GraphicElement` */
	auto graphic_element{py::class_<GraphicElement>(m_types, "GraphicElement")
		.def_readwrite(
			"type", &GraphicElement::type,
			py::doc("")
		).def_readwrite(
			"text", &GraphicElement::text,
			py::doc("")
		).def_readwrite(
			"x1", &GraphicElement::x1,
			py::doc("")
		).def_readwrite(
			"y1", &GraphicElement::y1,
			py::doc("")
		).def_readwrite(
			"x2", &GraphicElement::x2,
			py::doc("")
		).def_readwrite(
			"y2", &GraphicElement::y2,
			py::doc("")
		).def_readwrite(
			"z", &GraphicElement::z,
			py::doc("")
		).def(
			py::init<>(), py::doc("")
		).def(
			py::init<GraphicElement::type_t, GraphicElement::style_t, float, float, float, float, float>(),
			py::arg("type"), py::arg("style"),
			py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"), py::arg("z"),
			py::doc("")
		).def(
			"__repr__", [](const GraphicElement& self) {
				return stringf(
					"GraphicElement(text='%s', x1=%d, y1=%d, x2=%d, y2=%d, z=%d)",
					self.text.c_str(), self.x1, self.y1, self.x2, self.y2, self.z
				);
		})};
	graphic_element.doc() = "";
	m_types_all.append("GraphicElement");

	/* `nextpnr.types.GraphicElement.Type` */
	auto graphic_element_type{py::enum_<GraphicElement::type_t>(graphic_element, "Type")
		.value("NONE", GraphicElement::TYPE_NONE)
		.value("LINE", GraphicElement::TYPE_LINE)
		.value("ARROW", GraphicElement::TYPE_ARROW)
		.value("BOX", GraphicElement::TYPE_BOX)
		.value("CIRCLE", GraphicElement::TYPE_CIRCLE)
		.value("LABEL", GraphicElement::TYPE_LABEL)
		.value("LOCAL_ARROW", GraphicElement::TYPE_LOCAL_ARROW)
		.value("LOCAL_LINE", GraphicElement::TYPE_LOCAL_LINE)
		.value("MAX", GraphicElement::TYPE_MAX)};
	/* `nextpnr.types.GraphicElementType` (alias) */
	m_types.attr("GraphicElementType") = graphic_element_type;
	m_types_all.append("GraphicElementType");

	/* `nextpnr.types.GraphicElement.Style` */
	auto graphic_element_style{py::enum_<GraphicElement::style_t>(graphic_element, "Style")
		.value("GRID", GraphicElement::STYLE_GRID)
		.value("FRAME", GraphicElement::STYLE_FRAME)
		.value("HIDDEN", GraphicElement::STYLE_HIDDEN)
		.value("INACTIVE", GraphicElement::STYLE_INACTIVE)
		.value("ACTIVE", GraphicElement::STYLE_ACTIVE)
		.value("HIGHLIGHTED0", GraphicElement::STYLE_HIGHLIGHTED0)
		.value("HIGHLIGHTED1", GraphicElement::STYLE_HIGHLIGHTED1)
		.value("HIGHLIGHTED2", GraphicElement::STYLE_HIGHLIGHTED2)
		.value("HIGHLIGHTED3", GraphicElement::STYLE_HIGHLIGHTED3)
		.value("HIGHLIGHTED4", GraphicElement::STYLE_HIGHLIGHTED4)
		.value("HIGHLIGHTED5", GraphicElement::STYLE_HIGHLIGHTED5)
		.value("HIGHLIGHTED6", GraphicElement::STYLE_HIGHLIGHTED6)
		.value("HIGHLIGHTED7", GraphicElement::STYLE_HIGHLIGHTED7)
		.value("SELECTED", GraphicElement::STYLE_SELECTED)
		.value("HOVER", GraphicElement::STYLE_HOVER)
		.value("MAX", GraphicElement::STYLE_MAX)};
	/* `nextpnr.types.GraphicElementStyle` (alias) */
	m_types.attr("GraphicElementStyle") = graphic_element_style;
	m_types_all.append("GraphicElementStyle");

	/* `nextpnr.types.Loc` */
	auto loc{py::class_<Loc>(m_types, "Loc")
		.def_readwrite(
			"x", &Loc::x,
			py::doc("")
		).def_readwrite(
			"y", &Loc::y,
			py::doc("")
		).def_readwrite(
			"z", &Loc::z,
			py::doc("")
		).def(
			py::init<>(), py::doc("")
		).def(
			py::init<int, int, int>(),
			py::arg("x"), py::arg("y"), py::arg("z"),
			py::doc("")
		).def(
			py::self == py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self != py::self, py::arg("other"),
			py::doc("")
		)
		.def("__hash__", &Loc::hash)
		.def("__repr__", [](const Loc& self){
			return stringf("Loc(x=%d, y=%d, z=%d)", self.x, self.y, self.z);
		})};
	loc.doc() = "";
	m_types_all.append("Loc");

	/* `nextpnr.types.BoundingBox` */
	auto bounding_box{py::class_<BoundingBox>(m_types, "BoundingBox")
		.def_readwrite(
			"x0", &BoundingBox::x0,
			py::doc("")
		).def_readwrite(
			"y0", &BoundingBox::y0,
			py::doc("")
		).def_readwrite(
			"x1", &BoundingBox::x1,
			py::doc("")
		).def_readwrite(
			"y1", &BoundingBox::y1,
			py::doc("")
		).def(
			py::init<>(), py::doc("")
		).def(
			py::init<int, int, int, int>(),
			py::arg("x0"), py::arg("y0"), py::arg("x1"), py::arg("y1"),
			py::doc("")
		).def(
			"distance", &BoundingBox::distance,
			py::doc("")
		).def(
			"contains", &BoundingBox::contains,
			py::doc("")
		).def("__repr__", [](const BoundingBox& self){
			return stringf("BoundingBox(x0=%d, y0=%d, x1=%d, y1=%d)", self.x0, self.y0, self.x1, self.y1);
		})};
	bounding_box.doc() = "";
	m_types_all.append("BoundingBox");

	/* `nextpnr.types.PlaceStrength` */
	auto place_strength{py::enum_<PlaceStrength>(m_types, "PlaceStrength")
		.value("NONE", PlaceStrength::STRENGTH_NONE)
		.value("WEAK", PlaceStrength::STRENGTH_WEAK)
		.value("STRONG", PlaceStrength::STRENGTH_STRONG)
		.value("PLACER", PlaceStrength::STRENGTH_PLACER)
		.value("FIXED", PlaceStrength::STRENGTH_FIXED)
		.value("LOCKED", PlaceStrength::STRENGTH_LOCKED)
		.value("USER", PlaceStrength::STRENGTH_USER)};
	m_types_all.append("PlaceStrength");

	/* `nextpnr.types.DelayPair` */
	auto delay_pair{py::class_<DelayPair>(m_types, "DelayPair")
		.def_readwrite(
			"min_delay", &DelayPair::min_delay,
			py::doc("")
		).def_readwrite(
			"max_delay", &DelayPair::max_delay,
			py::doc("")
		).def(
			py::init<>(), py::doc("")
		).def(
			py::init<delay_t>(),
			py::arg("delay"),
			py::doc("")
		).def(
			py::init<delay_t, delay_t>(),
			py::arg("min"), py::arg("max"),
			py::doc("")
		).def(
			py::self + py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self += py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self - py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self -= py::self, py::arg("other"),
			py::doc("")
		).def("__repr__", [](const DelayPair& self){
			return stringf("DelayPair(min=%d, max=%d)", self.min_delay, self.max_delay);
		})};
	delay_pair.doc() = "";
	m_types_all.append("DelayPair");

	/* `nextpnr.types.DelayQuad` */
	auto delay_quad{py::class_<DelayQuad>(m_types, "DelayQuad")
		.def_readwrite(
			"rise", &DelayQuad::rise,
			py::doc("")
		).def_readwrite(
			"fall", &DelayQuad::fall,
			py::doc("")
		).def(
			py::init<>(), py::doc("")
		).def(
			py::init<delay_t>(),
			py::arg("delay"),
			py::doc("")
		).def(
			py::init<delay_t, delay_t>(),
			py::arg("min"), py::arg("max"),
			py::doc("")
		).def(
			py::init<DelayPair, DelayPair>(),
			py::arg("rise"), py::arg("fall"),
			py::doc("")
		).def(
			py::init<delay_t, delay_t, delay_t, delay_t>(),
			py::arg("min_rise"), py::arg("max_rise"), py::arg("min_fall"), py::arg("max_fall"),
			py::doc("")
		).def(
			"min_rise_delay", &DelayQuad::minRiseDelay,
			py::doc("")
		).def(
			"max_rise_delay", &DelayQuad::maxRiseDelay,
			py::doc("")
		).def(
			"min_fall_delay", &DelayQuad::minFallDelay,
			py::doc("")
		).def(
			"max_fall_delay", &DelayQuad::maxFallDelay,
			py::doc("")
		).def(
			"min_delay", &DelayQuad::minDelay,
			py::doc("")
		).def(
			"max_delay", &DelayQuad::maxDelay,
			py::doc("")
		).def(
			"delay_pair", &DelayQuad::delayPair,
			py::doc("")
		).def(
			py::self + py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self += py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self - py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self -= py::self, py::arg("other"),
			py::doc("")
		).def("__repr__", [](const DelayQuad& self){
			return stringf(
				"DelayQuad(rise=DelayPair(min=%d, max=%d), fall=DelayPair(min=%d, max=%d))",
				self.rise.min_delay, self.rise.max_delay, self.fall.min_delay, self.fall.max_delay
			);
		})};
	delay_quad.doc() = "";
	m_types_all.append("DelayQuad");

	/* `nextpnr.types.PortType` */
	auto port_type{py::enum_<PortType>(m_types, "PortType")
		.value("IN", PortType::PORT_IN)
		.value("OUT", PortType::PORT_OUT)
		.value("INOUT", PortType::PORT_INOUT)};
	m_types_all.append("PortType");

	/* `nextpnr.types.TimingPortClass` */
	auto timing_port_class{py::enum_<TimingPortClass>(m_types, "TimingPortClass")
		.value("CLOCK_INPUT", TimingPortClass::TMG_CLOCK_INPUT)
		.value("GEN_CLOCK", TimingPortClass::TMG_GEN_CLOCK)
		.value("REGISTER_INTPUT", TimingPortClass::TMG_REGISTER_INPUT)
		.value("REGISTER_OUTPUT", TimingPortClass::TMG_REGISTER_OUTPUT)
		.value("COMB_INPUT", TimingPortClass::TMG_COMB_INPUT)
		.value("COMB_OUTPUT", TimingPortClass::TMG_COMB_OUTPUT)
		.value("STARTPOINT", TimingPortClass::TMG_STARTPOINT)
		.value("ENDPOINT", TimingPortClass::TMG_ENDPOINT)
		.value("IGNORE", TimingPortClass::TMG_IGNORE)};
	m_types_all.append("TimingPortClass");

	/* `nextpnr.types.ClockEdge` */
	auto clock_edge{py::enum_<ClockEdge>(m_types, "ClockEdge")
		.value("RISING", ClockEdge::RISING_EDGE)
		.value("FALLING", ClockEdge::FALLING_EDGE)};
	m_types_all.append("ClockEdge");

	/* `nextpnr.types.ClockConstraint` */
	auto clock_constraint{py::class_<ClockConstraint>(m_types, "ClockConstraint")
		.def_readwrite(
			"high", &ClockConstraint::high,
			py::doc("")
		).def_readwrite(
			"low", &ClockConstraint::low,
			py::doc("")
		).def_readwrite(
			"period", &ClockConstraint::period,
			py::doc("")
		).def(py::init([](DelayPair high, DelayPair low, DelayPair period)  -> ClockConstraint{
				return { high, low, period };
			}),
			py::arg("high"), py::arg("low"), py::arg("period"),
			py::doc("")
		).def("__repr__", [](const ClockConstraint& self){
			return stringf(
				"ClockConstraint(high=DelayPair(min=%d, max=%d), low=DelayPair(min=%d, max=%d), period=DelayPair(min=%d, max=%d))",
				self.high.min_delay, self.high.max_delay,
				self.low.min_delay, self.low.max_delay,
				self.period.min_delay, self.period.max_delay
			);
		})};
	clock_constraint.doc() = "";
	m_types_all.append("ClockConstraint");

	/* `nextpnr.types.ClockFmax` */
	auto clock_fmax{py::class_<ClockFmax>(m_types, "ClockFMax")
		.def_readwrite(
			"achieved", &ClockFmax::achieved,
			py::doc("")
		).def_readwrite(
			"constraint", &ClockFmax::constraint,
			py::doc("")
		).def(py::init([](float achieved, float constraint) -> ClockFmax {
				return { achieved, constraint };
			}),
			py::arg("achieved"), py::arg("constraint"),
			py::doc("")
		).def("__repr__", [](const ClockFmax& self) {
			return stringf("ClockFMax(achieved=%f, constraint=%f)", self.achieved, self.constraint);
		})};
	clock_fmax.doc() = "";
	m_types_all.append("ClockFMax");

	/*
		TODO(aki): Things that need typing that we've not done
		- IdString
		- CellInfo
		- ArchCellInfo
		- PortInfo
		- TimingClockingInfo
		- PseudoCell
		- RegionPlug
		- ClockEvent
		- ClusterId
		- Property
		- NetInfo
		- ClockPair
		- CriticalPath
		- NetSinkTiming
		- TimingResult
		- dict<T, T>
		- pool<T>
		- HierarchicalPort
		- HierarchicalCell
		- BaseClusterInfo
	*/

	/* TODO(aki): ContextStuff */
	auto id_string{py::class_<IdString>(m_types, "IdString")
		.def_readwrite(
			"index", &IdString::index,
			py::doc("")
		).def(py::init([]() -> IdString {
				return {};
			}),
			py::doc("")
		).def(py::init([](int index) -> IdString {
				return IdString(index);
			}),
			py::arg("index"),
			py::doc("")
		).def(
			"empty", &IdString::empty,
			py::doc("")
		).def(
			py::self == py::self, py::arg("other"),
			py::doc("")
		).def(
			py::self != py::self, py::arg("other"),
			py::doc("")
		).def("__contains__", [](const IdString& self, const IdString& other){
			return other.in(self);
		}).def("__hash__", [](const IdString& self) {
			return self.hash();
		})};
	id_string.doc() = "";
	m_types_all.append("IdString");

	auto context{py::class_<Context>(m_types, "Context")};
	m_types_all.append("Context");

	auto base_ctx{py::class_<BaseCtx>(m_types, "BaseCtx")};
	m_types_all.append("BaseCtx");

	m_types.attr("__all__") = m_types_all;

	/* ==== <ARCH> submodule ==== */
	auto m_arch = m.def_submodule(STRINGIFY(ARCHNAME), "Python API for " STRINGIFY(ARCHNAME));
	python::initialize_arch_api(m_arch);
}

NEXTPNR_NAMESPACE_END
