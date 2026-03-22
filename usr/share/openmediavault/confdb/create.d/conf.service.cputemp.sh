#!/usr/bin/env dash
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

set -e

. /usr/share/openmediavault/scripts/helper-functions

# Build a script body from a command string.
# If the command is a path to an existing executable, read its content.
# Otherwise wrap the command in a minimal sh script.
make_script() {
    _cmd="${1}"
    if [ -f "${_cmd}" ] && [ -x "${_cmd}" ]; then
        cat "${_cmd}"
    else
        printf '#!/bin/sh\n%s\n' "${_cmd}"
    fi
}

if ! omv_config_exists "/config/services/cputemp"; then
    # Defaults
    OMV_CPU_TEMP_COMMAND=""
    OMV_CPU_TEMP_DIVISOR="1000"
    OMV_CPU_TEMP_COMMAND2=""
    OMV_CPU_TEMP_DIVISOR2="1000"
    OMV_CPU_TEMP_COMMAND3=""
    OMV_CPU_TEMP_DIVISOR3="1000"
    OMV_CPU_TEMP_COMMAND4=""
    OMV_CPU_TEMP_DIVISOR4="1000"

    # Import any existing settings from /etc/default/openmediavault
    if [ -r /etc/default/openmediavault ]; then
        . /etc/default/openmediavault
    fi

    # Sensor 1: use default thermal zone if no command was configured
    if [ -z "${OMV_CPU_TEMP_COMMAND}" ]; then
        SCRIPT1="$(printf '#!/bin/sh\ncat /sys/devices/virtual/thermal/thermal_zone0/temp\n')"
    else
        SCRIPT1="$(make_script "${OMV_CPU_TEMP_COMMAND}")"
    fi

    # Sensors 2-4: only populate if a command was explicitly configured
    if [ -n "${OMV_CPU_TEMP_COMMAND2}" ]; then
        SCRIPT2="$(make_script "${OMV_CPU_TEMP_COMMAND2}")"
    else
        SCRIPT2=""
    fi

    if [ -n "${OMV_CPU_TEMP_COMMAND3}" ]; then
        SCRIPT3="$(make_script "${OMV_CPU_TEMP_COMMAND3}")"
    else
        SCRIPT3=""
    fi

    if [ -n "${OMV_CPU_TEMP_COMMAND4}" ]; then
        SCRIPT4="$(make_script "${OMV_CPU_TEMP_COMMAND4}")"
    else
        SCRIPT4=""
    fi

    omv_config_add_node "/config/services" "cputemp"
    omv_config_add_key "/config/services/cputemp" "script" "${SCRIPT1}"
    omv_config_add_key "/config/services/cputemp" "scriptpath" "/usr/sbin/cpu-temp"
    omv_config_add_key "/config/services/cputemp" "divisor" "${OMV_CPU_TEMP_DIVISOR}"
    omv_config_add_key "/config/services/cputemp" "script2" "${SCRIPT2}"
    omv_config_add_key "/config/services/cputemp" "scriptpath2" "/usr/sbin/cpu-temp2"
    omv_config_add_key "/config/services/cputemp" "divisor2" "${OMV_CPU_TEMP_DIVISOR2}"
    omv_config_add_key "/config/services/cputemp" "script3" "${SCRIPT3}"
    omv_config_add_key "/config/services/cputemp" "scriptpath3" "/usr/sbin/cpu-temp3"
    omv_config_add_key "/config/services/cputemp" "divisor3" "${OMV_CPU_TEMP_DIVISOR3}"
    omv_config_add_key "/config/services/cputemp" "script4" "${SCRIPT4}"
    omv_config_add_key "/config/services/cputemp" "scriptpath4" "/usr/sbin/cpu-temp4"
    omv_config_add_key "/config/services/cputemp" "divisor4" "${OMV_CPU_TEMP_DIVISOR4}"
fi

exit 0
