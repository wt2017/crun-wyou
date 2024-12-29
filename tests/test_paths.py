#!/bin/env python3
# crun - OCI runtime written in C
#
# Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
# crun is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# crun is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with crun.  If not, see <http://www.gnu.org/licenses/>.

from tests_utils import *

def test_readonly_paths():
    conf = base_config()
    conf['root']['readonly'] = False
    conf['process']['args'] = ['/init', 'write', '/var/file', 'hello']
    conf['linux']['readonlyPaths'] = ['/var/file']
    add_all_namespaces(conf)
    try:
        run_and_get_output(conf)
    except Exception as e:
        if "Read-only file system" in e.output.decode():
            return 0
    return -1

def test_masked_paths():
    conf = base_config()
    conf['process']['args'] = ['/init', 'cat', '/var/file']
    conf['linux']['maskedPaths'] = ['/var/file']
    add_all_namespaces(conf)
    out, _ = run_and_get_output(conf, hide_stderr=True)
    if len(out) > 0:
        return -1
    return 0
    
all_tests = {
    "readonly-paths" : test_readonly_paths,
    "masked-paths" : test_masked_paths,
}

if __name__ == "__main__":
    tests_main(all_tests)
