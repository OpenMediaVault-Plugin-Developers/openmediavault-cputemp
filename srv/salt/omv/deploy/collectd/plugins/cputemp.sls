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

{% set config = salt['omv_conf.get']('conf.service.cputemp') %}

configure_collectd_conf_cputemp_plugin:
  file.managed:
    - name: "/etc/collectd/collectd.conf.d/cputemp.conf"
    - contents: |
        LoadPlugin exec
        <Plugin exec>
            Exec "nobody" "/usr/share/openmediavault-cputemp/scripts/collectd-cputemp"
        </Plugin>

configure_cputemp_conf:
  file.managed:
    - name: "/etc/openmediavault/cputemp.conf"
    - contents: |
        SCRIPT1={{ config.scriptpath | tojson }}
        DIVISOR1={{ config.divisor }}
        SCRIPT2={{ config.scriptpath2 | tojson }}
        DIVISOR2={{ config.divisor2 }}
        SCRIPT3={{ config.scriptpath3 | tojson }}
        DIVISOR3={{ config.divisor3 }}
        SCRIPT4={{ config.scriptpath4 | tojson }}
        DIVISOR4={{ config.divisor4 }}

configure_cputemp_script1:
  file.managed:
    - name: {{ config.scriptpath }}
    - source:
      - salt://{{ tpldir }}/files/cpu-temp.j2
    - template: jinja
    - context:
        script: {{ config.script | tojson }}
    - mode: 755

{% if config.script2 %}
configure_cputemp_script2:
  file.managed:
    - name: {{ config.scriptpath2 }}
    - source:
      - salt://{{ tpldir }}/files/cpu-temp.j2
    - template: jinja
    - context:
        script: {{ config.script2 | tojson }}
    - mode: 755
{% else %}
remove_cputemp_script2:
  file.absent:
    - name: {{ config.scriptpath2 }}
{% endif %}

{% if config.script3 %}
configure_cputemp_script3:
  file.managed:
    - name: {{ config.scriptpath3 }}
    - source:
      - salt://{{ tpldir }}/files/cpu-temp.j2
    - template: jinja
    - context:
        script: {{ config.script3 | tojson }}
    - mode: 755
{% else %}
remove_cputemp_script3:
  file.absent:
    - name: {{ config.scriptpath3 }}
{% endif %}

{% if config.script4 %}
configure_cputemp_script4:
  file.managed:
    - name: {{ config.scriptpath4 }}
    - source:
      - salt://{{ tpldir }}/files/cpu-temp.j2
    - template: jinja
    - context:
        script: {{ config.script4 | tojson }}
    - mode: 755
{% else %}
remove_cputemp_script4:
  file.absent:
    - name: {{ config.scriptpath4 }}
{% endif %}
