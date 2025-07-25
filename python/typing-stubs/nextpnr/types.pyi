# nextpnr -- Next Generation Place and Route
#
# Copyright (C) 2025 Aki Van Ness <aki@lethalbit.net>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

from __future__ import annotations

from enum   import Enum, auto, unique
from typing import SupportsFloat, SupportsInt, TypeAlias, overload

__all__ = (
	'GraphicElement',
	'GraphicElementType',
	'GraphicElementStyle',
	'Loc',
	'BoundingBox',
	'PlaceStrength',
	'DelayPair',
	'DelayQuad',
	'PortType',
	'TimingPortClass',
	'ClockEdge',
	'ClockConstraint',
	'ClockFMax',
	'Context',
	'BaseCtx',
	'IdString',
)

class GraphicElement:
	@unique
	class Type(Enum):
		NONE = auto()
		LINE = auto()
		ARROW = auto()
		BOX = auto()
		CIRCLE = auto()
		LABEL = auto()
		LOCAL_ARROW = auto()
		LOCAL_LINE = auto()
		MAX = auto()

	@unique
	class Style(Enum):
		GRID = auto()
		FRAME = auto()
		HIDDEN = auto()
		INACTIVE = auto()
		ACTIVE = auto()
		HIGHLIGHTED0 = auto()
		HIGHLIGHTED1 = auto()
		HIGHLIGHTED2 = auto()
		HIGHLIGHTED3 = auto()
		HIGHLIGHTED4 = auto()
		HIGHLIGHTED5 = auto()
		HIGHLIGHTED6 = auto()
		HIGHLIGHTED7 = auto()
		SELECTED = auto()
		HOVER = auto()
		MAX = auto()

	type: Type
	text: str
	x1: SupportsFloat
	y1: SupportsFloat
	x2: SupportsFloat
	y2: SupportsFloat
	z: SupportsFloat

	@overload
	def __init__(self) -> None:
		...

	@overload
	def __init__(
		self, type: Type, style: Style,
		x1: SupportsFloat, y1: SupportsFloat, x2: SupportsFloat, y2: SupportsFloat, z: SupportsFloat
	) -> None:
		...

	def __repr__(self) -> str:
		...

GraphicElementType: TypeAlias = GraphicElement.Type
GraphicElementStyle: TypeAlias = GraphicElement.Style

class Loc:
	x: SupportsInt
	y: SupportsInt
	z: SupportsInt

	@overload
	def __init__(self) -> None:
		...

	@overload
	def __init__(self, x: SupportsInt, y: SupportsInt, z: SupportsInt) -> None:
		...

	def __eq__(self, other: Loc) -> bool:
		...

	def __ne__(self, other: Loc) -> bool:
		...

	def __hash__(self) -> int:
		...

	def __repr__(self) -> str:
		...

class BoundingBox:
	x0: SupportsInt
	y0: SupportsInt
	x1: SupportsInt
	y1: SupportsInt

	@overload
	def __init__(self) -> None:
		...

	@overload
	def __init__(self, x0: SupportsInt, y0: SupportsInt, x1: SupportsInt, y1: SupportsInt) -> None:
		...

	def distance(self, location: Loc) -> int:
		...

	def contains(self, x: SupportsInt, y: SupportsInt) -> bool:
		...

	def __repr__(self) -> str:
		...

@unique
class PlaceStrength(Enum):
	NONE = auto()
	WEAK = auto()
	STRONG = auto()
	PLACER = auto()
	FIXED = auto()
	LOCKED = auto()
	USER = auto()


class DelayPair:
	min_delay: SupportsInt
	max_delay: SupportsInt

	@overload
	def __init__(self) -> None:
		...

	@overload
	def __init__(self, delay: SupportsInt) -> None:
		...

	@overload
	def __init__(self, min: SupportsInt, max: SupportsInt) -> None:
		...

	def __add__(self, other: DelayPair) -> DelayPair:
		...

	def __iadd__(self, other: DelayPair) -> DelayPair:
		...

	def __sub__(self, other: DelayPair) -> DelayPair:
		...

	def __isub__(self, other: DelayPair) -> DelayPair:
		...

	def __repr__(self) -> str:
		...

class DelayQuad:
	rise: DelayPair
	fall: DelayPair

	@overload
	def __init__(self) -> None:
		...

	@overload
	def __init__(self, delay: SupportsInt) -> None:
		...

	@overload
	def __init__(self, min: SupportsInt, max: SupportsInt) -> None:
		...

	@overload
	def __init__(self, rise: DelayPair, fall: DelayPair) -> None:
		...

	@overload
	def __init__(self, min_rise: SupportsInt, max_rise: SupportsInt, min_fall: SupportsInt, max_fall: SupportsInt) -> None:
		...

	def min_rise_delay(self) -> int:
		...

	def max_rise_delay(self) -> int:
		...

	def min_fall_delay(self) -> int:
		...

	def max_fall_delay(self) -> int:
		...

	def min_delay(self) -> int:
		...

	def max_delay(self) -> int:
		...

	def delay_pair(self) -> DelayPair:
		...

	def __add__(self, other: DelayQuad) -> DelayQuad:
		...

	def __iadd__(self, other: DelayQuad) -> DelayQuad:
		...

	def __sub__(self, other: DelayQuad) -> DelayQuad:
		...

	def __isub__(self, other: DelayQuad) -> DelayQuad:
		...

	def __repr__(self) -> str:
		...

@unique
class PortType(Enum):
	IN = auto()
	OUT = auto()
	INOUT = auto()

@unique
class TimingPortClass(Enum):
	CLOCK_INPUT = auto()
	GEN_CLOCK = auto()
	REGISTER_INPUT = auto()
	REGISTER_OUTPUT = auto()
	COMB_INPUT = auto()
	COMB_OUTPUT = auto()
	STARTPOINT = auto()
	ENDPOINT = auto()
	IGNORE = auto()

@unique
class ClockEdge(Enum):
	RISING = auto()
	FALLING = auto()

class ClockConstraint:
	high: DelayPair
	low: DelayPair
	period: DelayPair

	def __init__(self, high: DelayPair, low: DelayPair, period: DelayPair) -> None:
		...

	def __repr__(self) -> str:
		...

class ClockFMax:
	achieved: float
	constraint: float

	def __init__(self, achieved: SupportsFloat, constraint: SupportsFloat) -> None:
		...

	def __repr__(self) -> str:
		...


class Context:
	...

class BaseCtx:
	...


class IdString:
	index: SupportsInt

	@overload
	def __init__(self) -> None:
		...

	@overload
	def __init__(self, index: SupportsInt) -> None:
		...

	def empty(self) -> bool:
		...

	def __eq__(self, other: IdString) -> bool:
		...

	def __ne__(self, other: IdString) -> bool:
		...

	def __contains__(self, other: IdString) -> bool:
		...

	def __str__(self) -> str:
		...

	def __repr__(self) -> str:
		...

	def __hash__(self) -> int:
		...
