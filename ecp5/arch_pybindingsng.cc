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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "archdefs.h"
#include "arch.h"
#include "nextpnr.h"
#include "log.h"

NEXTPNR_NAMESPACE_BEGIN

#define STRINGIFY_(STR) #STR
#define STRINGIFY(STR) STRINGIFY_(STR)

namespace python {
	namespace py = pybind11;

	void initialize_arch_api(py::module_& m_arch, py::list& m_arch_all, py::class_<Context>& context) {

		/* `nextpnr.ecp5.ConstID` */
		auto const_ids{py::enum_<ConstIds>(m_arch, "ConstID")
			.value("ID_NONE", ID_NONE)
#define X(TOK)	.value(STRINGIFY(TOK), ConstIds::ID_##TOK)
#include "constids.inc"
#undef X
			.value("DB_CONST_ID_COUNT", DB_CONST_ID_COUNT)
		};
		m_arch_all.append("ConstID");

		/* `nextpnr.ecp5.Location` */
		auto location{py::class_<Location>(m_arch, "Location")
			.def_readwrite(
				"x", &Location::x,
				py::doc("")
			).def_readwrite(
				"y", &Location::y,
				py::doc("")
			).def(
				py::init<>(),
				py::doc("")
			).def(
				py::init<int16_t, int16_t>(),
				py::arg("x"), py::arg("y"),
				py::doc("")
			).def(
				py::self == py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self != py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self < py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self + py::self, py::arg("other"),
				py::doc("")
			).def("__repr__", [](const Location& self) {
				return stringf("Location(x=%d, y=%d)", self.x, self.y);
			}).def("__hash__", [](const Location& self) {
				return self.hash();
			})};
		location.doc() = "";
		m_arch_all.append("Location");

		/* `nextpnr.ecp5.BerlID` */
		auto bel_id{py::class_<BelId>(m_arch, "BelID")
			.def_readwrite(
				"location", &BelId::location,
				py::doc("")
			)
			.def_readwrite(
				"index", &BelId::index,
				py::doc("")
			).def(py::init([]() -> BelId {
					return {};
				}),
				py::doc("")
			).def(py::init([](Location location, int16_t index) -> BelId {
					return { location, index };
				}),
				py::arg("location"), py::arg("index"),
				py::doc("")
			).def(
				py::self == py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self != py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self < py::self, py::arg("other"),
				py::doc("")
			).def("__repr__", [](const BelId& self) {
				return stringf(
					"BelId(location=Location(x=%d, y=%d), index=%d)",
					self.location.x, self.location.y, self.index
				);
			}).def("__hash__", [](const BelId& self) {
				return self.hash();
			})};
		bel_id.doc() = "";
		m_arch_all.append("BelID");

		/* `nextpnr.ecp5.BelPin` */
		auto bel_pin{py::class_<BelPin>(m_arch, "BelPin")
			.def_readwrite(
				"bel", &BelPin::bel,
				py::doc("")
			).def_readonly(
				"pin", &BelPin::pin,
				py::doc("")
			).def(py::init([]() -> BelPin {
					return {};
				}),
				py::doc("")
			).def(py::init([](BelId bel, IdString pin) -> BelPin {
					return { bel, pin };
				}),
				py::arg("bel"), py::arg("pin"),
				py::doc("")
			).def("__repr__", [](const BelId& self){
				return stringf("BelId()");
			})};
		bel_pin.doc() = "";
		m_arch_all.append("BelPin");

		/* `nextpnr.ecp5.WireID` */
		auto wire_id{py::class_<WireId>(m_arch, "WireID")
			.def_readwrite(
				"location", &WireId::location,
				py::doc("")
			)
			.def_readwrite(
				"index", &WireId::index,
				py::doc("")
			).def(py::init([]() -> WireId {
					return {};
				}),
				py::doc("")
			).def(py::init([](Location location, int16_t index) -> WireId {
					return { location, index };
				}),
				py::arg("location"), py::arg("index"),
				py::doc("")
			).def(
				py::self == py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self != py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self < py::self, py::arg("other"),
				py::doc("")
			).def("__repr__", [](const WireId& self) {
				return stringf(
					"WireID(location=Location(x=%d, y=%d), index=%d)",
					self.location.x, self.location.y, self.index
				);
			}).def("__hash__", [](const WireId& self) {
				return self.hash();
			})};
		wire_id.doc() = "";
		m_arch_all.append("WireID");

		/* `nextpnr.ecp5.PipID` */
		auto pip_id{py::class_<PipId>(m_arch, "PipID")
			.def_readwrite(
				"location", &PipId::location,
				py::doc("")
			)
			.def_readwrite(
				"index", &PipId::index,
				py::doc("")
			).def(py::init([]() -> PipId {
					return {};
				}),
				py::doc("")
			).def(py::init([](Location location, int16_t index) -> PipId {
					return { location, index };
				}),
				py::arg("location"), py::arg("index"),
				py::doc("")
			).def(
				py::self == py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self != py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self < py::self, py::arg("other"),
				py::doc("")
			).def("__repr__", [](const PipId& self) {
				return stringf(
					"PipId(location=Location(x=%d, y=%d), index=%d)",
					self.location.x, self.location.y, self.index
				);
			}).def("__hash__", [](const PipId& self) {
				return self.hash();
			})};
		wire_id.doc() = "";
		m_arch_all.append("PipID");

		/* `nextpnr.ecp5.PipMap` */
		auto pip_map{py::class_<PipMap>(m_arch, "PipMap")
			.def_readwrite(
				"pip", &PipMap::pip,
				py::doc("")
			).def_readonly(
				"strength", &PipMap::strength,
				py::doc("")
			).def(py::init([]() -> PipMap {
					return {};
				}),
				py::doc("")
			).def(py::init([](PipId pip, PlaceStrength strength) -> PipMap {
				return { pip, strength };
				}),
				py::doc("")
			).def("__repr__", [](const PipMap& self){
				return stringf("PipMap()");
			})};
		pip_map.doc() = "";
		m_arch_all.append("PipMap");

		/* `nextpnr.ecp5.GroupID` */
		auto group_id{py::class_<GroupId>(m_arch, "GroupID")
			.def_readwrite(
				"location", &GroupId::location,
				py::doc("")
			).def_readwrite(
				"type", &GroupId::type,
				py::doc("")
			).def(py::init([]() -> GroupId {
					return {};
				}),
				py::doc("")
			).def(py::init([](decltype(GroupId::type) type, Location location) -> GroupId {
					return { type, location };
				}),
				py::arg(""), py::arg(""),
				py::doc("")
			).def(
				py::self == py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self != py::self, py::arg("other"),
				py::doc("")
			).def("__repr__", [](const GroupId& self) {
				return stringf(
					"GroupID(type=%d, location=Location(x=%d, y=%d))",
					self.type, self.location.x, self.location.y
				);
			}).def("__hash__", [](const GroupId& self) {
				return self.hash();
			})};
		group_id.doc() = "";
		m_arch_all.append("GroupID");

		/* `nextpnr.ecp5.GroupID.Type` */
		auto group_id_type{py::enum_<decltype(GroupId::type)>(group_id, "Type")
			.value("NONE", decltype(GroupId::type)::TYPE_NONE)
			.value("SWITCHBOX", decltype(GroupId::type)::TYPE_SWITCHBOX)};
		/* `nextpnr.ecp5.GroupIDType` (alias) */
		m_arch.attr("GroupIDType") = group_id_type;
		m_arch_all.append("GroupIDType");

		/* `nextpnr.ecp5.DecalID` */
		auto decal_id{py::class_<DecalId>(m_arch, "DecalID")
			.def_readwrite(
				"type", &DecalId::type,
				py::doc("")
			).def_readwrite(
				"location", &DecalId::location,
				py::doc("")
			).def_readwrite(
				"z", &DecalId::z,
				py::doc("")
			).def_readwrite(
				"active", &DecalId::active,
				py::doc("")
			).def(py::init([]() -> DecalId {
					return {};
				}),
				py::doc("")
			).def(py::init([](decltype(DecalId::type) type, Location location, uint32_t z, bool active) -> DecalId {
					return { type, location, z, active };
				}),
				py::arg("type"), py::arg("location"), py::arg("z"), py::arg("active"),
				py::doc("")
			).def(
				py::self == py::self, py::arg("other"),
				py::doc("")
			).def(
				py::self != py::self, py::arg("other"),
				py::doc("")
			).def("__repr__", [](const DecalId& self) {
				return stringf(
					"DecalId(type=%d, location=Location(x=%d, y=%d), z=%d, active=%d)",
					self.type, self.location.x, self.location.y, self.z, self.active
				);
			}).def("__hash__", [](const DecalId& self) {
				return self.hash();
			})};
		decal_id.doc() = "";
		m_arch_all.append("DecalID");

		/* `nextpnr.ecp5.DecalID.Type` */
		auto decal_id_type{py::enum_<decltype(DecalId::type)>(decal_id, "Type")
			.value("NONE", decltype(DecalId::type)::TYPE_NONE)
			.value("BEL", decltype(DecalId::type)::TYPE_BEL)
			.value("WIRE", decltype(DecalId::type)::TYPE_WIRE)
			.value("PIP", decltype(DecalId::type)::TYPE_PIP)
			.value("GROUP", decltype(DecalId::type)::TYPE_GROUP)};
		/* `nextpnr.ecp5.DecalIDType` (alias) */
		m_arch.attr("DecalIDType") = decal_id_type;
		m_arch_all.append("DecalIDType");

		/* `nextpnr.ecp5.DecalXY` */
		auto decal_xy{py::class_<DecalXY>(m_arch, "DecalXY")
			.def_readwrite(
				"decal", &DecalXY::decal,
				py::doc("")
			).def_readwrite(
				"x", &DecalXY::x,
				py::doc("")
			).def_readwrite(
				"y", &DecalXY::y,
				py::doc("")
			).def(py::init([]() -> DecalXY {
					return {};
				}),
				py::doc("")
			).def(py::init([](DecalId decal, float x, float y) -> DecalXY {
					return { decal, x, y };
				}),
				py::arg("decal"), py::arg("x"), py::arg("y"),
				py::doc("")
			).def(
				py::self == py::self,
				py::doc("")
			)};
		decal_xy.doc() = "";
		m_arch_all.append("DecalXY");

		/* `nextpnr.ecp5.ArchNetInfo` */
		auto arch_net_info{py::class_<ArchNetInfo>(m_arch, "ArchNetInfo")
			.def_readwrite(
				"is_global", &ArchNetInfo::is_global,
				py::doc("")
			).def(py::init([]() -> ArchNetInfo {
					return {};
				}),
				py::doc("")
			).def(py::init([](bool is_global) -> ArchNetInfo {
				return { is_global };
				}),
				py::arg("is_global"),
				py::doc("")
			).def("__repr__", [](const ArchNetInfo& self) {
				return stringf("ArchNetInfo(is_global=%d)", self.is_global);
			})};
		arch_net_info.doc() = "";
		m_arch_all.append("ArchNetInfo");

// 		/* `nextpnr.ecp5.ArchCellInfo` */
// 		auto arch_cell_info{py::class_<ArchCellInfo>(module, "ArchCellInfo")
// 			.def_readwrite(
// 				"comb_info", &ArchCellInfo::combInfo,
// 				py::doc("")
// 			).def_readwrite(
// 				"ff_info", &ArchCellInfo::ffInfo,
// 				py::doc("")
// 			).def_readwrite(
// 				"ram_info", &ArchCellInfo::ramInfo,
// 				py::doc("")
// 			).def_readwrite(
// 				"mult_info", &ArchCellInfo::multInfo,
// 				py::doc("")
// 			)
// 		};
// 		arch_cell_info.doc() = "";
// 		all.append("ArchCellInfo");
//
// 		/* `nextpnr.ecp5.ArchCellInfo.CombFlags` */
// 		auto arch_cell_info_comb_flags{py::enum_<ArchCellInfo::CombFlags>(arch_cell_info, "CombFlags")
// 			.value("NONE", ArchCellInfo::CombFlags::COMB_NONE)
// 			.value("CARRY", ArchCellInfo::CombFlags::COMB_CARRY)
// 			.value("LUTRAM", ArchCellInfo::CombFlags::COMB_LUTRAM)
// 			.value("MUX5", ArchCellInfo::CombFlags::COMB_MUX5)
// 			.value("MUX6", ArchCellInfo::CombFlags::COMB_MUX6)
// 			.value("RAM_WCKINV", ArchCellInfo::CombFlags::COMB_RAM_WCKINV)
// 			.value("RAM_WREINV", ArchCellInfo::CombFlags::COMB_RAM_WREINV)
// 			.value("RAMW_BLOCK", ArchCellInfo::CombFlags::COMB_RAMW_BLOCK)};
// 		/* `nextpnr.ecp5.ArchCellInfoCombFlags` (alias) */
// 		module.attr("ArchCellInfoCombFlags") = arch_cell_info_comb_flags;
// 		all.append("ArchCellInfoCombFlags");
//
// 		/* `nextpnr.ecp5.ArchCellInfo.FFFlags` */
// 		auto arch_cell_info_ff_flags{py::enum_<ArchCellInfo::FFFlags>(arch_cell_info, "FFFlags")
// 			.value("NONE", ArchCellInfo::FFFlags::FF_NONE)
// 			.value("CLKINV", ArchCellInfo::FFFlags::FF_CLKINV)
// 			.value("CEINV", ArchCellInfo::FFFlags::FF_CEINV)
// 			.value("CECONST", ArchCellInfo::FFFlags::FF_CECONST)
// 			.value("LSRINV", ArchCellInfo::FFFlags::FF_LSRINV)
// 			.value("GSREN", ArchCellInfo::FFFlags::FF_GSREN)
// 			.value("ASYNC", ArchCellInfo::FFFlags::FF_ASYNC)
// 			.value("M_USED", ArchCellInfo::FFFlags::FF_M_USED)};
// 		/* `nextpnr.ecp5.ArchCellInfoFFFlags` (alias) */
// 		module.attr("ArchCellInfoFFFlags") = arch_cell_info_ff_flags;
// 		all.append("ArchCellInfoFFFlags");
//
// 		/* `nextpnr.ecp5.ArchCellInfo.CombInfo` */
// 		auto arch_cell_info_comb_info{py::class_<decltype(ArchCellInfo::combInfo)>(arch_cell_info, "CombInfo")
// 			.def_readwrite(
// 				"flags", &decltype(ArchCellInfo::combInfo)::flags,
// 				py::doc("")
// 			).def_readwrite(
// 				"ram_wck", &decltype(ArchCellInfo::combInfo)::ram_wck,
// 				py::doc("")
// 			).def_readwrite(
// 				"ram_wre", &decltype(ArchCellInfo::combInfo)::ram_wre,
// 				py::doc("")
// 			).def_readwrite(
// 				"mux_fxad", &decltype(ArchCellInfo::combInfo)::mux_fxad,
// 				py::doc("")
// 			)
// 		};
// 		arch_cell_info_comb_info.doc() = "";
// 		/* `nextpnr.ecp5.ArchCellInfoCombInfo` (alias) */
// 		module.attr("ArchCellInfoCombInfo") = arch_cell_info_comb_info;
// 		all.append("ArchCellInfoCombInfo");
//
// 		/* `nextpnr.ecp5.ArchCellInfo.FFInfo` */
// 		auto arch_cell_info_ff_info{py::class_<decltype(ArchCellInfo::ffInfo)>(arch_cell_info, "FFInfo")
// 			.def_readwrite(
// 				"flags", &decltype(ArchCellInfo::ffInfo)::flags,
// 				py::doc("")
// 			).def_readwrite(
// 				"clk_sig", &decltype(ArchCellInfo::ffInfo)::clk_sig,
// 				py::doc("")
// 			).def_readwrite(
// 				"lsr_sig", &decltype(ArchCellInfo::ffInfo)::lsr_sig,
// 				py::doc("")
// 			).def_readwrite(
// 				"ce_sig", &decltype(ArchCellInfo::ffInfo)::ce_sig,
// 				py::doc("")
// 			).def_readwrite(
// 				"di_sig", &decltype(ArchCellInfo::ffInfo)::di_sig,
// 				py::doc("")
// 			)
// 		};
// 		arch_cell_info_ff_info.doc() = "";
// 		/* `nextpnr.ecp5.ArchCellInfoFFInfo` (alias) */
// 		module.attr("ArchCellInfoFFInfo") = arch_cell_info_ff_info;
// 		all.append("ArchCellInfoFFInfo");
//
// 		/* `nextpnr.ecp5.ArchCellInfo.RAMInfo` */
// 		auto arch_cell_info_ram_info{py::class_<decltype(ArchCellInfo::ramInfo)>(arch_cell_info, "RAMInfo")
// 			.def_readwrite(
// 				"is_pdp", &decltype(ArchCellInfo::ramInfo)::is_pdp,
// 				py::doc("")
// 			).def_readwrite(
// 				"is_output_a_registered", &decltype(ArchCellInfo::ramInfo)::is_output_a_registered,
// 				py::doc("")
// 			).def_readwrite(
// 				"is_output_b_registered", &decltype(ArchCellInfo::ramInfo)::is_output_b_registered,
// 				py::doc("")
// 			).def_readwrite(
// 				"regmode_timing_id", &decltype(ArchCellInfo::ramInfo)::regmode_timing_id,
// 				py::doc("")
// 			)
// 		};
// 		arch_cell_info_ram_info.doc() = "";
// 		/* `nextpnr.ecp5.ArchCellInfoRAMInfo` (alias) */
// 		module.attr("ArchCellInfoRAMInfo") = arch_cell_info_ram_info;
// 		all.append("ArchCellInfoRAMInfo");
//
// 		/* `nextpnr.ecp5.ArchCellInfo.MultInfo` */
// 		auto arch_cell_info_mult_info{py::class_<decltype(ArchCellInfo::multInfo)>(arch_cell_info, "MultInfo")
// 			.def_readwrite(
// 				"is_clocked", &decltype(ArchCellInfo::multInfo)::is_clocked,
// 				py::doc("")
// 			).def_readwrite(
// 				"timing_id", &decltype(ArchCellInfo::multInfo)::timing_id,
// 				py::doc("")
// 			)
// 		};
// 		arch_cell_info_mult_info.doc() = "";
// 		/* `nextpnr.ecp5.ArchCellInfoMultInfo` (alias) */
// 		module.attr("ArchCellInfoMultInfo") = arch_cell_info_mult_info;
// 		all.append("ArchCellInfoMultInfo");
	}
}

NEXTPNR_NAMESPACE_END
