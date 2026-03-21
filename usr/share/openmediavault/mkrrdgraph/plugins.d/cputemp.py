# -*- coding: utf-8 -*-
#
# Copyright (C) 2022-2026 openmediavault plugin developers
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
import os

import openmediavault.mkrrdgraph


class Plugin(openmediavault.mkrrdgraph.IPlugin):
    def create_graph(self, config):
        config.update(
            {
                'title_cputemp': 'CPU Temperature',
                'color_cputemp': '#ff1300',  # red
            }
        )
        image_filename = '{image_dir}/exec-cputemp-{period}.png'.format(**config)
        rrd_file = '{data_dir}/exec-cputemp/temperature-value.rrd'.format(**config)
        if not os.path.exists(rrd_file):
            openmediavault.mkrrdgraph.copy_placeholder_image(image_filename)
            return 0
        args = []
        # yapf: disable
        # pylint: disable=line-too-long
        # autopep8: off
        args.append(image_filename)
        args.extend(config['defaults'])
        args.extend(['--start', config['start']])
        args.extend(['--title', '"{title_cputemp}{title_by_period}"'.format(**config)])
        args.append('--slope-mode')
        args.extend(['--lower-limit', '0'])
        args.extend(['--vertical-label', 'Celsius'])
        args.append('DEF:tavg={data_dir}/exec-cputemp/temperature-value.rrd:value:AVERAGE'.format(**config))
        args.append('DEF:tmin={data_dir}/exec-cputemp/temperature-value.rrd:value:MIN'.format(**config))
        args.append('DEF:tmax={data_dir}/exec-cputemp/temperature-value.rrd:value:MAX'.format(**config))
        args.append('AREA:tavg{color_cputemp}:"Temperature"'.format(**config))
        args.append('GPRINT:tmin:MIN:"%4.1lf C Min"')
        args.append('GPRINT:tavg:AVERAGE:"%4.1lf C Avg"')
        args.append('GPRINT:tmax:MAX:"%4.1lf C Max"')
        args.append('GPRINT:tavg:LAST:"%4.1lf C Last\\l"')
        args.append('COMMENT:"{last_update}"'.format(**config))
        # autopep8: on
        # yapf: enable
        openmediavault.mkrrdgraph.call_rrdtool_graph(args)
        return 0
