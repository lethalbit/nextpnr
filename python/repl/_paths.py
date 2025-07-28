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

from os       import getenv
from os       import name as os_name
from pathlib  import Path
from typing   import Literal
from warnings import warn

__all__ = (
	'NEXTPNR_REPL_CACHE',
	'NEXTPNR_REPL_CONFIG',
	'NEXTPNR_REPL_STATE',
	'NEXTPNR_REPL_DATA',
)

try:
	from platformdirs import user_cache_path, user_config_path, user_data_path, user_state_path
except ImportError:
	warn(
		'Python module `platformdirs` not installed, falling back to best-effort guesses '
		'for REPL config and history files',
		ImportWarning
	)

	def _construct_path(base: Path | str, appname: str | None, version: str | None, ensure_exists: bool = False) -> Path:
		if not isinstance(base, Path):
			base = Path(base)

		if appname:
			base = base / appname
			if version:
				base = base / version

		if ensure_exists:
			base.mkdir(exist_ok = True, parents = True)
		return base

	def user_cache_path(
		appname: str | None = None,
		appauthor: str | None | Literal[False] = None,
		version: str | None = None,
		opinion: bool = True,  # noqa: FBT001, FBT002
		ensure_exists: bool = False,  # noqa: FBT001, FBT002
	) -> Path:
		homedir = Path.home()
		match os_name:
			case 'posix':
				return _construct_path(getenv('XDG_CACHE_HOME', homedir / '.cache'), appname, version, ensure_exists)
			case 'darwin':
				return _construct_path(homedir / 'Library' / 'Caches', appname, version, ensure_exists)
			case 'nt':
				return _construct_path(getenv('APPDATA', homedir), appname, version, ensure_exists)

		return _construct_path(homedir, appname, version, ensure_exists)

	def user_config_path(
		appname: str | None = None,
		appauthor: str | None | Literal[False] = None,
		version: str | None = None,
		roaming: bool = False,  # noqa: FBT001, FBT002
		ensure_exists: bool = False,  # noqa: FBT001, FBT002
	) -> Path:
		homedir = Path.home()
		match os_name:
			case 'posix':
				return _construct_path(getenv('XDG_CONFIG_HOME', homedir / '.config'), appname, version, ensure_exists)
			case 'darwin':
				return _construct_path(homedir / 'Library' / 'Application Support', appname, version, ensure_exists)
			case 'nt':
				return _construct_path(getenv('APPDATA', homedir), appname, version, ensure_exists)

		return _construct_path(homedir, appname, version)

	def user_data_path(
		appname: str | None = None,
		appauthor: str | None | Literal[False] = None,
		version: str | None = None,
		roaming: bool = False,  # noqa: FBT001, FBT002
		ensure_exists: bool = False,  # noqa: FBT001, FBT002
	) -> Path:
		homedir = Path.home()
		match os_name:
			case 'posix':
				return _construct_path(getenv('XDG_DATA_HOME', homedir / '.local' / 'share'), appname, version, ensure_exists)
			case 'darwin':
				return _construct_path(homedir / 'Library' / 'Application Support', appname, version, ensure_exists)
			case 'nt':
				return _construct_path(getenv('APPDATA', homedir), appname, version, ensure_exists)

		return _construct_path(homedir, appname, version, ensure_exists)

	def user_state_path(
		appname: str | None = None,
		appauthor: str | None | Literal[False] = None,
		version: str | None = None,
		roaming: bool = True,  # noqa: FBT001, FBT002
		ensure_exists: bool = False,  # noqa: FBT001, FBT002
	) -> Path:
		homedir = Path.home()
		match os_name:
			case 'posix':
				return _construct_path(getenv('XDG_STATE_HOME', homedir / '.local' / 'state'), appname, version, ensure_exists)
			case 'darwin':
				return _construct_path(homedir / 'Library' / 'Application Support', appname, version, ensure_exists)
			case 'nt':
				return _construct_path(getenv('APPDATA', homedir), appname, version, ensure_exists)

		return _construct_path(homedir, appname, version, ensure_exists)

NEXTPNR_REPL_CACHE = user_cache_path(appname = 'nextpnr', appauthor = 'YosysHQ', ensure_exists = True)
NEXTPNR_REPL_STATE = user_state_path(appname = 'nextpnr', appauthor = 'YosysHQ', ensure_exists = True)
NEXTPNR_REPL_CONFIG = user_config_path(appname = 'nextpnr', appauthor = 'YosysHQ', ensure_exists = True)
NEXTPNR_REPL_DATA = user_data_path(appname = 'nextpnr', appauthor = 'YosysHQ', ensure_exists = True)
